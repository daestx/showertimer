/*
 * led.c
 *
 *  Created on: 24.01.2023
 *      Author: daest
 */

#include "wup.h"
#include "led.h"


// clear external interrupt flag
void wup_clearIsrFlags(){

   __HAL_GPIO_EXTI_CLEAR_FLAG(GPIO_PIN_14);
   NVIC_ClearPendingIRQ(EXTI4_15_IRQn);

}


void wup_sleep(){
   // clear pending interrupt flags before switching to sleep mode.
   // Otherwise system will wakeup immediately
   wup_clearIsrFlags();
   // Put controller into stop mode and configure it to wait for interrupt.
   // Any external interrupt will wakeup the microcontroller. For this application
   // interrupt source connected to Hall sensor. No ISR required
   HAL_PWR_EnterSTOPMode(PWR_LOWPOWERREGULATOR_ON, PWR_STOPENTRY_WFE);



}
