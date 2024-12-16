/*
 * multifunc.h
 *
 *  Created on: Feb 1, 2023
 *      Author: lukasbartels
 */

#ifndef MUTIFUNCSHIELD_MULTIFUNC_H_
#define MUTIFUNCSHIELD_MULTIFUNC_H_

#include <stdint.h>

/* has been moved to hardwareGlobal.h */
///* LEDs are low active */
//#define LED1_OFF() 		HAL_GPIO_WritePin(LD1_GPIO_Port, LD1_Pin, GPIO_PIN_SET)
//#define LED1_ON() 		HAL_GPIO_WritePin(LD1_GPIO_Port, LD1_Pin, GPIO_PIN_RESET)
//#define LED1_TOGGLE()	HAL_GPIO_TogglePin(LD1_GPIO_Port, LD1_Pin)
//
//#define LED2_OFF() 		HAL_GPIO_WritePin(LD2_GPIO_Port, LD2_Pin, GPIO_PIN_SET)
//#define LED2_ON() 		HAL_GPIO_WritePin(LD2_GPIO_Port, LD2_Pin, GPIO_PIN_RESET)
//#define LED2_TOGGLE()	HAL_GPIO_TogglePin(LD2_GPIO_Port, LD2_Pin)
//
//#define LED3_OFF() 		HAL_GPIO_WritePin(LD3_GPIO_Port, LD3_Pin, GPIO_PIN_SET)
//#define LED3_ON() 		HAL_GPIO_WritePin(LD3_GPIO_Port, LD3_Pin, GPIO_PIN_RESET)
//#define LED3_TOGGLE()	HAL_GPIO_TogglePin(LD3_GPIO_Port, LD3_Pin)
//
//#define LED4_OFF() 		HAL_GPIO_WritePin(LD4_GPIO_Port, LD4_Pin, GPIO_PIN_SET)
//#define LED4_ON() 		HAL_GPIO_WritePin(LD4_GPIO_Port, LD4_Pin, GPIO_PIN_RESET)
//#define LED4_TOGGLE()	HAL_GPIO_TogglePin(LD4_GPIO_Port, LD4_Pin)
//
///* Buzzer Stuff */
//#define BEEP_OFF()		HAL_GPIO_WritePin(Buzzer_GPIO_Port, Buzzer_Pin, GPIO_PIN_SET)
//#define BEEP_ON()		HAL_GPIO_WritePin(Buzzer_GPIO_Port, Buzzer_Pin, GPIO_PIN_RESET)
//
///* Shift Register Stuff */
//#define SHCP_PIN_LOW()	HAL_GPIO_WritePin(SHCP_GPIO_Port, SHCP_Pin, 0)
//#define SHCP_PIN_HIGH()	HAL_GPIO_WritePin(SHCP_GPIO_Port, SHCP_Pin, 1)
//
//#define STCP_PIN_LOW()	HAL_GPIO_WritePin(STCP_GPIO_Port, STCP_Pin, 0)
//#define STCP_PIN_HIGH()	HAL_GPIO_WritePin(STCP_GPIO_Port, STCP_Pin, 1)

/**
 * most part of this library handles the 7-segment display
 * the display consist of four individual 7-segment units embed in one housing
 * in order to use all four digits simultaneously one has to
 * provide some code to refresh all segments in a fast way to not have it recognized by
 * the human eye
 */

// set all LED in all four segments to turn off
void mf_reset_display(void);

// write a char type to one of the four digits e.g. 'A', '1'
void mf_write_char_to_display_segment(const uint8_t segment, char data);

// write a uint8_t type to one of the four digits (will be cut off to 1 digit) e.g. 1, 3, 8
void mf_write_uint_to_display_segment(const uint8_t segment, uint8_t data);

// same as mf_write_uint_to_display_segment
void mf_write_decimal_to_display_segment(const uint8_t segment, uint8_t data);

// write a raw byte type to one of the four digits e.g. 0x1f, 0b01010011, 5
void mf_write_byte_to_display_segment(const uint8_t segment, uint8_t data);

// write a percentage value to be mapped to a full circel representation
void mf_write_gauge_to_display_segment(const uint8_t segment, uint8_t persantage);

// need to bee called very frequently (min. 6 milli seconds period) in order
// to show all four digits simultaneously
void mf_refresh_display(void);

// loops through some hard coded data to show on all four digits
void mf_demo_segment(void);


// tot make static
void mf_write_to_display_memory(const uint8_t segment, char data);
char mf_uint_to_char(uint8_t uint);




#endif /* MUTIFUNCSHIELD_MULTIFUNC_H_ */
