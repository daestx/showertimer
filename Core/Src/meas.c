/*
 * led.c
 *
 *  Created on: 24.01.2023
 *      Author: daest
 */

#include "meas.h"
#include "adc.h"


#define MEAS_BUF_SIZE   2

static MEAS_VAL meas_Val;

uint16_t meas_DMA_buf[MEAS_BUF_SIZE];


// Called when buffer is completely filled
void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef* hadc) {
  meas_Val.chan_1 = meas_DMA_buf[0];
  meas_Val.chan_2 = meas_DMA_buf[1];
  meas_Val.count++;
}


void meas_init(){

}

uint8_t meas_start(){
   if(HAL_OK == HAL_ADC_Start_DMA(&hadc, (uint32_t*) &meas_DMA_buf, MEAS_BUF_SIZE)){
      return MEAS_OK;
   }

   return MEAS_ERR;
}


uint8_t meas_get_values(MEAS_VAL * val){

   *val = meas_Val;

   return MEAS_OK;

}
