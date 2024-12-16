#include "utils.h"


/* override _write, is used by puts and printf */
int _write(int file, char *ptr, int len)
{
    UART_SEND_TERMINAL((uint8_t*) ptr, (uint16_t)len);
	return len;
}


uint32_t raw_buffer_to_hex_string(const uint8_t *buffer, size_t buffer_size, char *hex_string) {
	uint32_t bytes_written = 0;
	for (uint_fast32_t i = 0; i < buffer_size; i++) {
		bytes_written += sprintf(hex_string + 2 * i, "%02X", buffer[i]);
	}
	hex_string[2 * buffer_size] = '\0';

	return bytes_written + 1;
}
