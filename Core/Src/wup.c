/*
 * led.c
 *
 *  Created on: 24.01.2023
 *      Author: daest
 */

#include "wup.h"
#include "led.h"

// activate external interrupt source
void wup_activate(){

}


// suspend external interrupt source
void wup_suspend(){

}


void wup_sleep(){
   // Put controller into stop mode and configure it to wait for interrupt.
   // Any external interrupt will wakeup the microcontroller. For this application
   // interrupt source connected to Hall sensor. No ISR required
   HAL_PWR_EnterSTOPMode(PWR_LOWPOWERREGULATOR_ON, PWR_STOPENTRY_WFE);


}
