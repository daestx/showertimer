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
#define  MS_SHOWER_PHASE1E 0x11
#define  MS_SHOWER_PHASE2  0x12
#define  MS_SHOWER_PHASE2E 0x13
#define  MS_SHOWER_PHASE3  0x14
#define  MS_SHOWER_PHASE3E 0x15
#define  MS_SHOWER_PHASE4  0x16
#define  MS_TESTMODE       0xF0

// modes in shutdown state
#define  MS_SHTDWN_DIRECT  0xFA


// states
#define  MS_INIT           0x00
#define  MS_START          0x01
#define  MS_OPERATIONAL    0x02
#define  MS_SHUTDOWN       0x03
#define  MS_SLEEP          0x04

void ms_handle_states(void);
void ms_init();
uint8_t ms_get_state();
void ms_set_state(uint8_t state);
void ms_set_mode(uint8_t mode);


#endif
