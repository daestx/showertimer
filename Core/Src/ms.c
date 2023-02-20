/*
 * ms.c
 *
 *  Created on: 24.01.2023
 *      Author: daest
 */


#include "adc.h"
#include "dma.h"
#include "gpio.h"

#include "ms.h"
#include "led.h"
#include "adc.h"
#include "temp.h"
#include "hall.h"
#include "wup.h"
#include "timer.h"
#include "meas.h"

static struct{
   uint8_t ms_state;
   uint8_t ms_last_state;
   uint8_t ms_mode;
   uint8_t ms_last_mode;
}ms_states;


static TIMER_TIM ms_tim_1;  // timer 1
static TIMER_TIM ms_tim_2;  // timer 2

#define MS_BLINK_OFF 0
#define MS_BLINK_ON  1

uint8_t ms_ledTimer(uint32_t duration, uint8_t led, uint8_t blink){
   static uint8_t last_mode;  // nothing to do with ms_states
   static uint8_t last_state; // used locally only to recognize state changes
   static uint8_t toggle;  // status of a blinking LED

   // with every mode or state change, timer shall be reset
   if(last_mode != ms_states.ms_mode || last_state !=  ms_states.ms_state){

      last_mode = ms_states.ms_mode;
      last_state =  ms_states.ms_state;
      // reset timers
      timer_reset(&ms_tim_1);
      timer_reset(&ms_tim_2);
      // and initialized with the new duration
      timer_init(&ms_tim_1, duration);
      if(MS_BLINK_ON == blink) timer_init(&ms_tim_2, TIMER_LED_BLINK); // LEDs are blinking always with 100ms
      // Switch on led
      led_display_simple(led);
      // reset hall counter
      hall_updateHallCount(HALL_RESET);
   }

   if (MS_BLINK_ON == blink){

      if(timer_isElapsed(&ms_tim_2) == TIMER_ELAPSED){

         if(toggle == 1)led_display_simple(led);
         else led_clear();

         // invert toggle state
         toggle ^= 0x01;

         // reset ms_tim_2
         timer_reset(&ms_tim_2);
         timer_init(&ms_tim_2, TIMER_LED_BLINK);

      }

   }

   return (timer_isElapsed(&ms_tim_1));
}


void ms_init(){
   // this state will only be entered for the very first time
   // after connecting power to the board

   // initialize all peripherals
   MX_GPIO_Init();
   MX_DMA_Init();
   MX_ADC_Init();

   led_clear();

   ms_set_state(MS_START);
   // enter directly stop mode.
   wup_sleep();
}

void ms_start(){

   // setup timer
   timer_init(&ms_tim_1, 100);
   // green LED shall be switched on during this time
   led_display_simple(LED_G);
   // as long as the timer is not elapse, state will not be switched
   if(timer_isElapsed(&ms_tim_1) == TIMER_RUN) return;

   // clear all LEDs
   led_clear();
   // release timer, so that it can be reused.
   timer_reset(&ms_tim_1);

   // switch state to MS_OPERATIONAL
   ms_set_state(MS_OPERATIONAL);
   // set the mode to MS_SHOWER_OFF
   ms_set_mode(MS_SHOWER_OFF);

   // test the shutdown
   ms_set_mode(MS_SHOWER_PHASE1);
   ms_set_mode(MS_SHOWER_PHASE1E);
   ms_set_state(MS_SHUTDOWN);

}

void ms_operational(){
   //static uint8_t index;
   //static TIMER_TIM ms_test_tim;  // for testmode only
   uint8_t rc;



   temp_cyclic();
   hall_cyclic();
   meas_start();

   switch (ms_states.ms_mode){

      case MS_SHOWER_OFF:     // wake up event recognized.
                              // check for 10 second, if there are multiple HALL signals recognized
                              if(TIMER_RUN == ms_ledTimer(TIMER_PERIOD_10S, LED_OFF, MS_BLINK_OFF)) return;
                              // check wether hall count is higher then 20 (5 turns of the propeller
                              if (hall_getHallCount() < HALL_EXPCOUNT_10S){
                                 // seems like only a glitch was detected that woke up the system
                                 // start system shutdown
                                 ms_set_state(MS_SHUTDOWN);
                                 break;
                              }

                              // switch to next mode
                              ms_set_mode(MS_SHOWER_START);
                              break;

      case MS_SHOWER_START:   // shower start phase. Wait until Water is warm
                              rc = ms_ledTimer(TIMER_PERIOD_1MIN, LED_OFF, MS_BLINK_OFF);

                              // check if temperature reached 30°C
                              if(30 <= temp_getTemp()){
                                 // water is already warm, so start phase 1
                                 ms_set_mode(MS_SHOWER_PHASE1);
                                 break;
                              }

                              // return as long as Timer is not elapsed
                              if(rc == TIMER_RUN) return;

                              // or check after 1 min, if there are multiple HALL signals recognized
                              if (hall_getHallCount() < 1000){
                                 // seems like shower tap was closed
                                 ms_set_state(MS_SHUTDOWN);
                                 break;
                              }

                              // ... water is still running
                              ms_set_mode(MS_SHOWER_PHASE1);
                              break;
      case MS_SHOWER_PHASE1:  // First 4:50 min green LED
                              if(TIMER_RUN == ms_ledTimer(TIMER_PERIOD_5MIN - TIMER_PERIOD_10S , LED_G, MS_BLINK_OFF)) return;
                              // check whether water is still flowing

                              if(HALL_POLE_FALSE == hall_getHallStatus_10S()){
                                 // seems like shower tap was closed
                                 ms_set_state(MS_SHUTDOWN);
                                 break;
                              }

                              ms_set_mode(MS_SHOWER_PHASE1E);
                              break;

      case MS_SHOWER_PHASE1E: // 10 s blinking green LED
                              if(TIMER_RUN == ms_ledTimer(TIMER_PERIOD_10S , LED_G, MS_BLINK_ON)) return;
                              // check whether water is still flowing
                              if(HALL_POLE_FALSE == hall_getHallStatus_10S()){
                                 // seems like shower tap was closed
                                 ms_set_state(MS_SHUTDOWN);
                                 break;
                              }

                              ms_set_mode(MS_SHOWER_PHASE2);


      case MS_SHOWER_PHASE2:  // Next 1:50 min blue LED
                              if(TIMER_RUN == ms_ledTimer(TIMER_PERIOD_2MIN - TIMER_PERIOD_10S , LED_B, MS_BLINK_OFF)) return;
                              // check whether water is still flowing
                              if(HALL_POLE_FALSE == hall_getHallStatus_10S()){
                                 // seems like shower tap was closed
                                 ms_set_state(MS_SHUTDOWN);
                                 break;
                              }

                              ms_set_mode(MS_SHOWER_PHASE2E);
                              break;

      case MS_SHOWER_PHASE2E: // 10 s blinking blue LED
                              if(TIMER_RUN == ms_ledTimer(TIMER_PERIOD_10S , LED_B, MS_BLINK_ON)) return;
                              // check whether water is still flowing
                              if(HALL_POLE_FALSE == hall_getHallStatus_10S()){
                                 // seems like shower tap was closed
                                 ms_set_state(MS_SHUTDOWN);
                                 break;
                              }

                              ms_set_mode(MS_SHOWER_PHASE3);

      case MS_SHOWER_PHASE3:  // Next 3 min red LED
                              if(TIMER_RUN == ms_ledTimer(TIMER_PERIOD_3MIN , LED_R, MS_BLINK_OFF)) return;
                              // check whether water is still flowing
                              if(HALL_POLE_FALSE == hall_getHallStatus_10S()){
                                 // seems like shower tap was closed
                                 ms_set_state(MS_SHUTDOWN);
                                 break;
                              }

                              ms_set_mode(MS_SHOWER_PHASE4);
                              break;


      case MS_SHOWER_PHASE4:  // IF shower takes longer then 10 minutes red LED starts blinking
                              if(TIMER_RUN == ms_ledTimer(TIMER_PERIOD_30S, LED_R, MS_BLINK_ON)) return;
                              // check whether water is still flowing
                              if(HALL_POLE_FALSE == hall_getHallStatus_10S()){
                                 // seems like shower tap was closed
                                 ms_set_state(MS_SHUTDOWN);
                                 break;
                              }
                              // keep the same mode but reset timer. Every 30 seconds
                              timer_reset(&ms_tim_1);
                              break;





      case MS_TESTMODE:       // test mode
                              break;

   }
}

void ms_shutdown(){
   static uint8_t count;
   // in case last state is not OPERATIONAL switch directly to sleep
   if(ms_states.ms_last_state == MS_OPERATIONAL){
      // wait for another 3 Minutes if shower is switched on
      // again before entering sleep mode
      // check every 30 seconds if there is again some activity
      if(TIMER_RUN == ms_ledTimer(TIMER_PERIOD_30S, LED_OFF, MS_BLINK_OFF)) return;
      if(HALL_POLE_TRUE == hall_getHallStatus_10S()){
         // seems like shower tap opened again
         ms_set_state(MS_OPERATIONAL);
         if(ms_states.ms_mode < MS_SHOWER_PHASE4){
            // switch again to operational but to the next mode from which
            // we were originally coming
            ms_set_state(ms_states.ms_mode + 1);
         }
         // only if red LED was already blinking (MS_SHOWER_PHASE4)
         // switch to this state again
         ms_set_state(MS_SHOWER_PHASE4);
      }
      // else wait for max 6 rounds
      count ++;
      if(6 < count) ms_set_state(MS_SLEEP);
   }

   // reset counter for the next run
   count = 0;
   // prepare everything for sleep
   ms_set_state(MS_SLEEP);
   // clear all leds
   led_clear();
}

void ms_sleep(){
   ms_set_state(MS_START);
   // enter sleep phase
   wup_sleep();
}



void ms_handle_states(void){

   switch (ms_states.ms_state) {
      case MS_INIT:        ms_init();
                           break;
      case MS_START:       ms_start();
                           break;
      case MS_OPERATIONAL: ms_operational();
                           break;
      case MS_SHUTDOWN:    ms_shutdown();
                           break;
      case MS_SLEEP:       ms_sleep();
                           break;
   }

}

uint8_t ms_get_state(){
   return ms_states.ms_state;
}

void ms_set_state(uint8_t state){

   ms_states.ms_last_state = ms_states.ms_state;
   ms_states.ms_state = state;

}

void ms_set_mode(uint8_t mode){

   ms_states.ms_last_mode = ms_states.ms_mode;
   ms_states.ms_mode = mode;

}

