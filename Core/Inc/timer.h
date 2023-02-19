/*
 * led.h
 *
 *  Created on: 24.01.2023
 *      Author: daest
 */

#ifndef __TIMER_H
#define __TIMER_H

#include "stm32l0xx_hal.h"


#define TIMER_RUN       0
#define TIMER_ELAPSED   1

#define TIMER_TIM_OK    0x00
#define TIMER_TIM_INIT  0xFF


// define some time periods
#define TIMER_LED_BLINK       100
#define TIMER_PERIOD_1S       1000
#define TIMER_PERIOD_10S      10000
#define TIMER_PERIOD_30S      30000
#define TIMER_PERIOD_1MIN     60000
#define TIMER_PERIOD_2MIN     120000
#define TIMER_PERIOD_3MIN     180000
#define TIMER_PERIOD_4MIN     240000
#define TIMER_PERIOD_5MIN     300000
#define TIMER_PERIOD_6MIN     360000
#define TIMER_INFINITY        0XFFFFFFFF




typedef struct{
   uint32_t startTime;
   uint32_t endTime;
   uint8_t  owflw;
   uint8_t  init;
}TIMER_TIM;



uint8_t  timer_init(TIMER_TIM *tim, uint32_t duration);

uint8_t timer_isElapsed(TIMER_TIM *tim);

uint8_t timer_reset(TIMER_TIM *tim);

#endif
