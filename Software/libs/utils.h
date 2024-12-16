#ifndef COMMON_UTILS_H
#define COMMON_UTILS_H


#include <stdlib.h>
#include <string.h>
#include <stdarg.h> //for va_list var arg function
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include "hardwareGlobal.h"


uint32_t raw_buffer_to_hex_string(const uint8_t *buffer, size_t buffer_size, char *hex_string);

#endif
