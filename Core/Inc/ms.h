/*
 *  ms.h
 *
 *  Created on: 24.01.2023
 *      Author: daest
 */


#ifndef __MS_H
#define __MS_H

#include "stm32l0xx_hal.h"

// modes in operational state
#define  MS_SHOWER_OFF     0x00
#define  MS_SHOWER_START   0x01
#define  MS_SHOWER_PHASE1  0x10
#define  MS_SHOWER_PHASE2  0x11
#define  MS_SHOWER_PHASE3  0x12
#define  MS_SHOWER_PHASE4  0x13
#define  MS_SHOWER_END     0xA0
#define  MS_TESTMODE       0xF0

// states
#define  MS_INIT           0x00
#define  MS_START          0x01
#define  MS_OPERATIONAL    0x02
#define  MS_SHUTDOWN       0x03
#define  MS_SLEEP          0x04

void ms_handle_states(void);
void ms_init();
uint8_t ms_get_state();
uint8_t ms_set_state(uint8_t state);
uint8_t ms_set_mode(uint8_t state);


#endif
