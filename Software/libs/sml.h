/*
 * sml.h
 *
 *  Created on: Dec 28, 2024
 *      Author: lukas
 */

#ifndef SML_H_
#define SML_H_
#include <stdbool.h>

void sml_main(void);
void sml_uart_terminal_bridge_setup(void);
void sml_telegram_decoder(void);


// ------------------- helper functions --------------
bool sml_check_for_start_sequence(uint8_t *buffer);
void sml_read_char_callback(void);

#endif /* SML_H_ */
