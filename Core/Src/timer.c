/*
 * timer.c
 *
 *    timer handling based on internal tick counting since 1 tick equals 1 ms
 *
 *  Created on: 24.01.2023
 *      Author: daest
 */

#include "timer.h"
#include "meas.h"

// setup timer structure
// duration in [ms] when timer will be marked as elapsed
uint8_t timer_init(TIMER_TIM *tim, uint32_t duration){

   if(tim->init == TIMER_TIM_INIT) return TIMER_TIM_INIT;

   tim->owflw = 0;
   tim->startTime = HAL_GetTick();
   tim->endTime = tim->startTime + duration;
   // overflow of timer ticks expected after ~49 days.
   if (tim->endTime < tim->startTime)tim->owflw = 1;
   tim->init = TIMER_TIM_INIT;

   return TIMER_TIM_OK;
}


uint8_t timer_isElapsed(TIMER_TIM *tim){

   uint32_t ticks = HAL_GetTick();

   // no owerflow
   if(tim->owflw == 0){

      if (ticks > tim->endTime) return TIMER_ELAPSED;
      return TIMER_RUN;

   }
   else{
      // overflow happened, hence flag is cleared
      if(ticks < tim->startTime) tim->owflw = 0;
      return TIMER_RUN;
   }
}


uint8_t timer_reset(TIMER_TIM *tim){

   // remove init flag. Timer structure could be reused again.
   tim->init = TIMER_TIM_OK;

   return TIMER_TIM_OK;
}

