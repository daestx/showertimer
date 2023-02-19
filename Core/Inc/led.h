/*
 * led.h
 *
 *  Created on: 24.01.2023
 *      Author: daest
 */

#ifndef __LED_H
#define __LED_H

#include "stm32l0xx_hal.h"

#define LED_OFF   0x00
#define LED_R     0x01
#define LED_G     0x02
#define LED_B     0x03


void led_init();
void led_clear();
void led_display_simple(uint8_t led);




#endif
