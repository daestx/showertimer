/*
 * led.h
 *
 *  Created on: 24.01.2023
 *      Author: daest
 */

#ifndef __MEAS_H
#define __MEAS_H

#include "stm32l0xx_hal.h"

#define MEAS_OK   0
#define MEAS_ERR  1

typedef struct{
   uint16_t chan_1;
   uint16_t chan_2;
   uint32_t count;
}MEAS_VAL;

uint8_t meas_start();
uint8_t meas_get_values(MEAS_VAL * val);


#endif
