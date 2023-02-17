/*
 * led.h
 *
 *  Created on: 24.01.2023
 *      Author: daest
 */

#ifndef __TEMP_H
#define __TEMP_H

#include "stm32l0xx_hal.h"



void temp_init();
void temp_cyclic();
int16_t temp_getTemp();




#endif
