/*
 * led.c
 *
 *  Created on: 24.01.2023
 *      Author: daest
 */

#include "led.h"
#include "main.h"


#define LED_B_Pin GPIO_PIN_7
#define LED_G_Pin GPIO_PIN_9
#define LED_R_Pin GPIO_PIN_10


struct{
	uint16_t GPIO_Pin;
	GPIO_PinState PinState;
}LED_config;

void led_init(){

}

void led_clear(){
	// clear all leds
	HAL_GPIO_WritePin(GPIOA, LED_R_Pin, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(GPIOA, LED_G_Pin, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(GPIOA, LED_B_Pin, GPIO_PIN_RESET);
}

void led_display_simple(uint8_t led){

	led_clear();

	// set selected led
	if(led == LED_R)HAL_GPIO_WritePin(GPIOA, LED_R_Pin, GPIO_PIN_SET);
	else if(led == LED_G)HAL_GPIO_WritePin(GPIOA, LED_G_Pin, GPIO_PIN_SET);
	else if(led == LED_B)HAL_GPIO_WritePin(GPIOA, LED_B_Pin, GPIO_PIN_SET);
	else; // do nothing -> Equals LED_OFF

}

void led_display_rgb(){

}
