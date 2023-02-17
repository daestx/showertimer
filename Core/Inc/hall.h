/*
 * led.h
 *
 *  Created on: 24.01.2023
 *      Author: daest
 */

#ifndef __HALL_H
#define __HALL_H

#include "stm32l0xx_hal.h"

#define HALL_POLE_FALSE    0
#define HALL_POLE_TRUE     1
#define HALL_RESET         0xff


// Min excpected amount of counts
#define HALL_EXPCOUNT_10S  20


void hall_init();
void hall_cyclic();
uint8_t hall_getHallStatus();
uint8_t hall_getHallStatus_10S();

void hall_updateHallCount(uint8_t status);
uint32_t hall_getHallCount();



#endif
