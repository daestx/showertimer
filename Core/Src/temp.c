/*
 * led.c
 *
 *  Created on: 24.01.2023
 *      Author: daest
 */

#include "temp.h"
#include "meas.h"

#define TEMP_SAMPLES 3
#define TEMP_SCALEFACTOR 1000

void temp_init(){

}


static int32_t  temp_real;


void temp_calcMean(){
   MEAS_VAL val;
   static uint32_t count;
   static uint32_t temp_array[TEMP_SAMPLES];
   uint32_t temp_mean;
   static uint8_t index;



   // get current ADC value
   if(MEAS_OK == meas_get_values(&val)){
      // new values received?
      if(val.count != count){
         // channel one is temperature
         temp_array[index++] = val.chan_1;
         count = val.count;
         index = index % TEMP_SAMPLES;
      }
   }

   // calculate mean value of defined samples (low pass)
   temp_mean = 0;
   for (uint8_t i = 0; i < TEMP_SAMPLES; i++){
      temp_mean += temp_array[i];
   }
   temp_mean /= TEMP_SAMPLES;

   // convert mean value in V
   // 3V = 2^12 = 4096
   // V = (3/4096) * temp_mean *1000 = mV

   temp_mean *= TEMP_SCALEFACTOR;
   temp_mean *= 3;
   temp_mean /= 4096;

   // convert V value to °C
   // according data sheet, temp curve is linear
   // V-V1 = ((V2-v1)/(T2-T1))*(T-T1)
   // In a range from 0 - 40°C: T = -(1039/5,45)* V[mV]
   temp_real = temp_mean;
   temp_real -= 1039;
   temp_real *= -100;
   temp_real /= 545;

}

int16_t temp_getTemp(){

   return temp_real;

}

void temp_cyclic(){
   // calculate fresh temperature values
   temp_calcMean();
}

