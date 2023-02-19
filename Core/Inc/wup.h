/*
 * led.h
 *
 *  Created on: 24.01.2023
 *      Author: daest
 */

#ifndef __WUP_H
#define __WUP_H

#include "stm32l0xx_hal.h"

void wup_clearIsrFlags();
void wup_sleep();

#endif
