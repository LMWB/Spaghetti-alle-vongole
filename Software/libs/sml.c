#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>

#include "hardwareGlobal.h"
#include "sml.h"

#define uart_rx_buffer_size 512
static uint8_t uart_IR_rx_buffer[		uart_rx_buffer_size];
static uint8_t uart_terminal_rx_buffer[	uart_rx_buffer_size];
static uint16_t buffer_head = 0;
static bool sml_telegram_read_compleat = false;

uint8_t sml_telegram[] = {27, 27, 27, 27, 1, 1, 1, 1, 118, 5, 18, 172, 2, 55, 98, 0, 98, 0, 114, 99, 1, 1, 118, 1, 7, 255, 255, 255, 255, 255, 255, 5, 6, 57, 86, 19, 11, 10, 1, 69, 70, 82, 32, 2, 207, 149, 224, 114, 98, 1, 101, 6, 57, 85, 212, 1, 99, 172, 177, 0, 118, 5, 18, 172, 2, 56, 98, 0, 98, 0, 114, 99, 7, 1, 119, 7, 255, 255, 255, 255, 255, 255, 11, 10, 1, 69, 70, 82, 32, 2, 207, 149, 224, 7, 1, 0, 98, 10, 255, 255, 114, 98, 1, 101, 6, 57, 85, 212, 241, 6, 119, 7, 1, 0, 96, 50, 1, 1, 1, 1, 1, 1, 4, 69, 70, 82, 1, 119, 7, 1, 0, 96, 1, 0, 255, 1, 1, 1, 1, 11, 10, 1, 69, 70, 82, 32, 2, 207, 149, 224, 1, 119, 7, 1, 0, 1, 8, 0, 255, 100, 28, 0, 4, 114, 98, 1, 101, 6, 57, 85, 212, 98, 30, 82, 255, 101, 3, 63, 88, 100, 1, 119, 7, 1, 0, 2, 8, 0, 255, 1, 114, 98, 1, 101, 6, 57, 85, 212, 98, 30, 82, 255, 100, 80, 62, 28, 1, 119, 7, 1, 0, 16, 7, 0, 255, 1, 1, 98, 27, 82, 0, 83, 1, 1, 1, 119, 7, 1, 0, 32, 7, 0, 255, 1, 1, 98, 35, 82, 255, 99, 8, 250, 1, 119, 7, 1, 0, 52, 7, 0, 255, 1, 1, 98, 35, 82, 255, 99, 8, 234, 1, 119, 7, 1, 0, 72, 7, 0, 255, 1, 1, 98, 35, 82, 255, 99, 8, 254, 1, 119, 7, 1, 0, 31, 7, 0, 255, 1, 1, 98, 33, 82, 254, 98, 62, 1, 119, 7, 1, 0, 51, 7, 0, 255, 1, 1, 98, 33, 82, 254, 98, 27, 1, 119, 7, 1, 0, 71, 7, 0, 255, 1, 1, 98, 33, 82, 254, 98, 65, 1, 119, 7, 1, 0, 81, 7, 1, 255, 1, 1, 98, 8, 82, 0, 82, 118, 1, 119, 7, 1, 0, 81, 7, 2, 255, 1, 1, 98, 8, 82, 0, 83, 0, 238, 1, 119, 7, 1, 0, 81, 7, 4, 255, 1, 1, 98, 8, 82, 0, 83, 1, 75, 1, 119, 7, 1, 0, 81, 7, 15, 255, 1, 1, 98, 8, 82, 0, 83, 1, 86, 1, 119, 7, 1, 0, 81, 7, 26, 255, 1, 1, 98, 8, 82, 0, 83, 1, 63, 1, 119, 7, 1, 0, 14, 7, 0, 255, 1, 1, 98, 44, 82, 255, 99, 1, 243, 1, 119, 7, 1, 0, 0, 2, 0, 0, 1, 1, 1, 1, 6, 48, 51, 46, 48, 48, 1, 119, 7, 1, 0, 96, 90, 2, 1, 1, 1, 1, 1, 3, 66, 189, 1, 119, 7, 1, 0, 97, 97, 0, 0, 1, 1, 1, 1, 3, 0, 0, 1, 119, 7, 1, 0, 96, 50, 1, 4, 1, 1, 1, 1, 8, 80, 49, 46, 50, 46, 49, 50, 1, 119, 7, 1, 0, 96, 50, 4, 4, 1, 1, 1, 1, 3, 4, 34, 1, 1, 1, 99, 187, 17, 0, 118, 5, 18, 172, 2, 57, 98, 0, 98, 0, 114, 99, 2, 1, 113, 1, 99, 137, 3, 0, 0, 27, 27, 27, 27, 26, 1, 182, 162,'\0'};

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart){

	if (huart->Instance == UART_TERMINAL_INSTANCE){
		__NOP();
	}

	if (huart->Instance == UART_IR_INSTANCE){
		sml_read_char_callback();
	}
}

void HAL_UARTEx_RxEventCallback(UART_HandleTypeDef *huart, uint16_t Size){

	if (huart->Instance == UART_TERMINAL_INSTANCE){
		printf("---- NEW LINE ---- \n");
		UART_TERMINAL_READ_LINE_IRQ(uart_terminal_rx_buffer, uart_rx_buffer_size);
	}
}

void sml_read_char_callback(void){
	// regular printf() does not work, it swallows chars!
//	char b[8];
//	uint8_t s = 0;
//	s = sprintf(b, "%02X ", uart_IR_rx_buffer[buffer_head]);
//	UART_SEND_TERMINAL((uint8_t*)b, s);

	if(sml_check_for_start_sequence(uart_IR_rx_buffer)){
		// start sequence detected, start a new line on terminal
//		printf("\t HIT \n");
		sml_telegram_read_compleat = true;
	}

	// increment pointer on char buffer
	buffer_head++;
	// prevent pointer from increase more than buffer length
	buffer_head = buffer_head & 0x1FF;
	// activate interrupt based, byte wise uart read
	UART_IR_READ_BYTE_IRQ( &uart_IR_rx_buffer[buffer_head] );
}

// Function to search for a byte pattern in an array
bool searchBytePattern(const unsigned char *array, uint16_t arraySize,
                       const unsigned char *pattern, uint16_t patternSize,
                       uint16_t *foundIndex) {
    // Handle edge cases
    if (!array || !pattern || arraySize < patternSize || patternSize == 0) {
        return false;
    }

    // Loop through the array to search for the pattern
    for (uint16_t i = 0; i <= arraySize - patternSize; i++) {
        bool match = true;

        // Check if the pattern matches at this position
        for (uint16_t j = 0; j < patternSize; j++) {
            if (array[i + j] != pattern[j]) {
                match = false;
                break;
            }
        }

        // If a match is found, return the index
        if (match) {
            *foundIndex = i;
            return true;
        }
    }

    // If no match is found, return false
    return false;
}

void test_searchBytePattern(void){
    unsigned char array[] = {0x12, 0x34, 0x56, 0x78, 0x9A, 0xBC, 0xDE};
    unsigned char pattern[] = {0x56, 0x78, 0x9A};
    uint16_t foundIndex;

    if (searchBytePattern(array, sizeof(array), pattern, sizeof(pattern), &foundIndex)) {
        printf("Pattern found at index: %d\n", foundIndex);
    } else {
        printf("Pattern not found.\n");
    }
}

// Function to search for a byte pattern in an array
bool sml_search_byte_pattern(const char *array, uint16_t arraySize,
                       const char *pattern, uint16_t patternSize,
                       uint16_t *foundIndex) {
    // Handle edge cases
    if (!array || !pattern || arraySize < patternSize || patternSize == 0) {
        return false;
    }

    // Loop through the array to search for the pattern
    for (uint16_t i = 0; i <= arraySize - patternSize; i++) {
        bool match = true;

        // Check if the pattern matches at this position
        for (uint16_t j = 0; j < patternSize; j++) {
            if (array[i + j] != pattern[j]) {
                match = false;
                break;
            }
        }

        // If a match is found, return the index
        if (match) {
            *foundIndex = i;
            return true;
        }
    }

    // If no match is found, return false
    return false;
}


// key_instantaneous_power 	= '77 07 01 00 10 07 00 FF'
// key_energie_consumed 	= '77 07 01 00 01 08 00 FF'
// key_energie_generated 	= '77 07 01 00 02 08 00 FF'

static uint8_t obis_code_power[] = 	{0x77, 0x07, 0x01, 0x00, 0x10, 0x07, 0x00, 0xFF,};
static uint8_t obis_code_energieP[] = 	{0x77, 0x07, 0x01, 0x00, 0x01, 0x08, 0x00, 0xFF,};
static uint8_t obis_code_energieN[] = 	{0x77, 0x07, 0x01, 0x00, 0x02, 0x08, 0x00, 0xFF,};

void sml_telegram_decoder(void){
	uint16_t sml_telegram_length = sizeof(sml_telegram)/sizeof(uint8_t);
	printf("sml telegram given has length of %d\n", sml_telegram_length);

	uint16_t hit = 0;
	sml_search_byte_pattern((char*)sml_telegram, sizeof(sml_telegram), (char*)obis_code_power, sizeof(obis_code_power), &hit);
	printf("found monumental power value in sml telegram %d", hit);
}

bool sml_check_for_start_sequence(uint8_t *buffer) {
	if (	buffer[buffer_head] == 0x01 && buffer[buffer_head - 1] == 0x01 &&
			buffer[buffer_head - 2] == 0x1B && buffer[buffer_head - 3] == 0x1B) {
		// start sequence detected
		return true;
	} else {
		return false;
	}
}

void sml_uart_terminal_bridge_setup(void){
	UART_IR_READ_BYTE_IRQ( &uart_IR_rx_buffer[buffer_head] );
	printf("activate UART1 in read byte mode\n");
	UART_TERMINAL_READ_LINE_IRQ( uart_terminal_rx_buffer, uart_rx_buffer_size);
	printf("activate UART2 in DMA read line mode\n");
}

void sml_main(void){

	if(sml_telegram_read_compleat){
		sml_telegram_read_compleat = false;
		uint16_t hit = 0;

		sml_search_byte_pattern((char*)uart_IR_rx_buffer, sizeof(uart_IR_rx_buffer), (char*)obis_code_power, sizeof(obis_code_power), &hit);
		int32_t power = (uart_IR_rx_buffer[hit+15] << 8 )| uart_IR_rx_buffer[hit+16];

		sml_search_byte_pattern((char*)uart_IR_rx_buffer, sizeof(uart_IR_rx_buffer), (char*)obis_code_energieP, sizeof(obis_code_energieP), &hit);
		int32_t energieP = (uart_IR_rx_buffer[hit+25] << 24 ) | (uart_IR_rx_buffer[hit+26] << 16) | (uart_IR_rx_buffer[hit+27] << 8) | uart_IR_rx_buffer[hit+28];

		sml_search_byte_pattern((char*)uart_IR_rx_buffer, sizeof(uart_IR_rx_buffer), (char*)obis_code_energieN, sizeof(obis_code_energieN), &hit);
		int32_t energieN = (uart_IR_rx_buffer[hit+22] << 16) | (uart_IR_rx_buffer[hit+23] << 8) | uart_IR_rx_buffer[hit+24];

		printf("\n\n1.8.0: %ldWh\t2.8.0: %ldWh\t 16.7.0: %ldW\n", energieP/10, energieN/10, power);
	}

}
