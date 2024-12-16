/*
 * multifunc.c
 *
 *  Created on: Feb 1, 2023
 *      Author: lukasbartels
 */

#include "multifunc.h"
#include "hardwareGlobal.h"

// buffer that hold the bit pattern in order to drive the LEDs of 7-segment display
// do not worry about its content, it could be a representation of type:
// char, uint8_t, decimal or hex
volatile uint8_t display_Memory[4]	= {' ', ' ', ' ', ' '};

// constant bit mask to select one out of four 7-segment LED array
// it is been used for the bit shift register
const uint8_t SEGEMNT_SELECT[] 		= {0x10, 0x20, 0x40, 0x80};
//const uint8_t SEGEMNT_SELECT[] 		= {0xf0, 0xf0, 0xf0, 0xf0};

// kind of dictionary to decode uint8_t, char or hex to a LED-array representation
const char SEGMENT_MAP_HEX[]		= { '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'A', 'B', 'C', 'D', 'E', 'F'};
const uint8_t SEGMENT_MAP_DIGIT[]	= { 0xFC, 0x60, 0xDA, 0xF2, 0x66, 0xB6, 0x3E, 0xE0, 0xFE, 0xE6}; // 0..9
const uint8_t SEGMENT_MAP_GAUGE[]	= { 0x00, 0x40, 0x60, 0x70, 0x78, 0x7c, 0xFC}; // 00 x 60 x x fc
const uint8_t SEGMENT_MAP_ALPH[] 	= {
		0xEE, 0xFE, 0x9C, 0xFC, 0x9E, 0x8E, // A, B, C, D, E, F
		0xBE, 0x6E, 0x0C, 0x78, 0x00, 0x1C, // G, H, I, J, K, L
		0xEC, 0x2E, 0x3A, 0xCE, 0x3B, 0xEE,	// M, N, O, P, Q, R
		0xB6, 0x0E, 0x7C, 0x38, 0x38, 0x0,	// S, T, U, V, W, X
		0x66, 0xDA,};						// Y, Z


static void mf_delay(void)
{
	const uint8_t counts = 3;
	/* create a very stupid delay (blocking) */
	uint32_t i = 0;
	while(i<counts)
	{
		i++;
	}
}

static uint8_t mf_decimal_to_segment_value(uint8_t decimal){
	uint8_t segmentValue = 182;
	uint8_t d = decimal;
	if(d > 9){
		d = 9;
	}
	segmentValue = ~SEGMENT_MAP_DIGIT[d];
	return segmentValue;
}

// convert a char type (ASCII sign) to a LED on/off pattern
// that represents desired format to make visible
static uint8_t mf_ascii_to_segment_value(char ascii) {
	uint8_t segmentValue = 182;

	if (ascii >= '0' && ascii <= '9') {
		segmentValue = ~SEGMENT_MAP_DIGIT[ascii - '0'];

	} else if (ascii >= 'a' && ascii <= 'z') {
		segmentValue = ~SEGMENT_MAP_ALPH[ascii - 'a'];

	} else if (ascii >= 'A' && ascii <= 'Z') {
		segmentValue = ~SEGMENT_MAP_ALPH[ascii - 'A'];

	} else {
		switch (ascii) {
		case '-':
			segmentValue = ~(0x02);
			break;

		case '.':
			segmentValue = ~(0x01);
			break;

		case '_':
			segmentValue = ~(0x10);
			break;

		case ' ':
			segmentValue = 0xFF; // all off
			break;
		default:
			break;
		}
	}
	return segmentValue;
}

/* 0 ... 100% mapped to 6 segments */
static uint8_t mf_map_percentage_to_gauge_index(uint8_t percent){
	// 7 Stepp: 0, 14, 28, 42, 58,
	// 											 _
	// 	  	|	  |		  |		  |		| |		| |
	// 	   		  |		 _|		|_|		|_|		|_|
	//  0	1	2		3		4		5		6

	if(percent >= 100){
		return 6;
	}else if(percent >= 85){
		return 5;
	}else if(percent >= 68){
		return 4;
	}else if (percent >= 51) {
		return 3;
	}else if (percent >= 35){
		return 2;
	}else if (percent >= 17){
		return 1;
	}else{
		return 0;
	}
}

/* Shift Register */
static void mf_rising_latch_serial_data_pin(void)
{
	MF_SHCP_PIN_LOW();
	MF_SHCP_PIN_HIGH();
	mf_delay();
	MF_SHCP_PIN_LOW();
}

/* Output register */
static void mf_rising_latch_serial_store_pin(void)
{
	MF_STCP_PIN_LOW();
	MF_STCP_PIN_HIGH();
	mf_delay();
	MF_STCP_PIN_LOW();
}


static void mf_shift_data(const uint16_t data) {
	uint16_t dummy_word = data;

	for (uint_fast16_t i = 16; i > 0; i--) {
		// put one bit and one bit only to DS pin
		//HAL_GPIO_WritePin(DS_GPIO_Port, DS_Pin, dummy_word & 0x1);
		MF_WIRTIE_BIT_TO_DATA_PIN(dummy_word & 0x1);
		// clock data into shift register
		mf_rising_latch_serial_data_pin();
		// right shift
		dummy_word = dummy_word >> 1;
	}
}

//static void mf_write_char(const uint8_t segment, char data)
//{
//	// segment 0..3
//
//	uint8_t msb = SEGEMNT_SELECT[segment&0x03];
//	uint8_t lsb = mf_ascii_to_segment_value(data);
//	mf_shift_data( (msb<<8) | lsb);
//	// put shift register content to output register
//	mf_rising_latch_serial_store_pin();
//}

static void mf_write_raw(const uint8_t segment, uint8_t raw_byte)
{
	// segment 0..3

	uint8_t msb = SEGEMNT_SELECT[segment&0x03];
	uint8_t lsb = raw_byte;
	mf_shift_data( (msb<<8) | lsb);
	// put shift register content to output register
	mf_rising_latch_serial_store_pin();
}

static void mf_refresh_segment(uint8_t segment){
	uint8_t s = segment&0x03; // only 0..3 allowed
	char data = display_Memory[s];
	mf_write_raw(s, data);
}

/* Multiplexing through all four parts of the 7-segment display
 * this function should be called with high period
 */
void mf_refresh_display(void)
{
	static uint8_t current_segment = 0;
	mf_refresh_segment(current_segment);

	current_segment+=1;
	if(current_segment>=4)
	{
		current_segment = 0;
	}
}

void mf_write_to_display_memory(const uint8_t segment, char data)
{
	display_Memory[segment] = data;
}


void mf_write_char_to_display_segment(const uint8_t segment, char data)
{
	display_Memory[segment] = mf_ascii_to_segment_value(data);
	mf_refresh_segment(segment);
}

void mf_write_uint_to_display_segment(const uint8_t segment, uint8_t data)
{
	display_Memory[segment] = mf_decimal_to_segment_value(data);
	mf_refresh_segment(segment);
}

void mf_write_decimal_to_display_segment(const uint8_t segment, uint8_t data)
{
	display_Memory[segment] = mf_decimal_to_segment_value(data);
	mf_refresh_segment(segment);
}

void mf_write_byte_to_display_segment(const uint8_t segment, uint8_t data)
{
	display_Memory[segment] = data;
	mf_refresh_segment(segment);
}

uint8_t mf_percentage_to_segment_value(uint8_t persantage)
{
	uint8_t segmentValue = 0;
	uint8_t i = mf_map_percentage_to_gauge_index(persantage);
	segmentValue = ~SEGMENT_MAP_GAUGE[i];
	return segmentValue;
}

void mf_write_gauge_to_display_segment(const uint8_t segment, uint8_t persantage)
{
	display_Memory[segment] = mf_percentage_to_segment_value(persantage);
	mf_refresh_segment(segment);
}

char mf_uint_to_char(uint8_t uint)
{
	return SEGMENT_MAP_HEX[uint];
}

void mf_reset_display(void)
{
	/* delete all date in shift register and put in on output */
	mf_shift_data(0x0000);
	// put shift register content to output register
	mf_rising_latch_serial_store_pin();
}


void mf_demo_segment(void) {

	/* 0..9 */
	for (uint_fast8_t j = 0; j < 4; j++) {
		for (uint_fast8_t i = 0; i < 10; i++) {
			//mf_write_char(j, i + '0');
			HAL_Delay(250);
		}
	}

	/* A..Z */
	for (uint_fast8_t j = 0; j < 4; j++) {
		for (uint_fast8_t i = 0; i < 26; i++) {
			//mf_write_char(j, i + 'a');
			HAL_Delay(250);
		}
	}

	/* Baseline */
	for (uint_fast8_t j = 0; j < 4; j++) {
		//mf_write_char(j, '-');
		HAL_Delay(100);

	}
}







