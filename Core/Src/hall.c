/*
 * led.c
 *
 *  Created on: 24.01.2023
 *      Author: daest
 */

#include "hall.h"
#include "meas.h"

#define HALL_BOUNDRAY 150

static uint32_t   hall_real;
static uint8_t    hall_status;
static uint32_t   hall_count;


void hall_init(){


}


void hall_checkBoundary(){
   MEAS_VAL val;
   static uint32_t count;

   // get current ADC value
   if(MEAS_OK == meas_get_values(&val)){
      // new values received?
      if(val.count != count){
         // channel_2 is HAL value
         hall_real = val.chan_2;
         count = val.count;
      }
   }

   // if The magnetic pole (north or sout) is applied facing the branded side of the package
   // output signal will switch to low. Zero magnetic pole equals to high

   if (hall_real < HALL_BOUNDRAY) hall_status = HALL_POLE_TRUE;
   else hall_status = HALL_POLE_FALSE;

}


void hall_cyclic(){
   hall_checkBoundary();
}

uint8_t hall_getHallStatus(){

   return hall_status;

}

void hall_updateHallCount(uint8_t status){

   static uint8_t last_state;

   if(status == HALL_RESET) hall_count = 0;

   // increment hall counter with every rising or falling edge
   // one full turn of the propeller will increase Hall counter by 4
   // if all signal edges were recognized
   if (status != last_state)  hall_count++;

}
uint32_t hall_getHallCount(){
   return hall_count;
}

