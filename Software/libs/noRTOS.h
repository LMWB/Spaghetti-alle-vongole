#ifndef NORTOS_H_
#define NORTOS_H_

#include <stdint.h>
#include <stdbool.h>
#include "main.h"
#include "utils.h"


#define NORTOS_VERSION_MINOR 1
#define NORTOS_VERSION_MAYOR 0

#define NORTOS_SCHEDULAR_GET_TICK()	GET_TICK() // returns tick in milliseconds
#define NORTOS_NO_OF_MAX_TASK		128

typedef void(*VoidFunctionPointer)(void);

typedef enum {
	eNORTOS_PERIODE_1ms 	= 1,
	eNORTOS_PERIODE_2ms 	= 2,
	eNORTOS_PERIODE_3ms 	= 3,
	eNORTOS_PERIODE_4ms 	= 4,
	eNORTOS_PERIODE_5ms 	= 5,
	eNORTOS_PERIODE_6ms 	= 6,
	eNORTOS_PERIODE_7ms 	= 7,
	eNORTOS_PERIODE_8ms 	= 8,
	eNORTOS_PERIODE_9ms 	= 9,
	eNORTOS_PERIODE_10ms 	= 10,
	eNORTOS_PERIODE_100ms 	= 100,
	eNORTOS_PERIODE_500ms 	= 500,
	eNORTOS_PERIODE_1s	= 1 * 1000,
	eNORTOS_PERIODE_5s 	= 5 * 1000,
	eNORTOS_PERIODE_10s	= 10 * 1000,
	eNORTOS_PERIODE_30s = 30 * 1000,
	eNORTOS_PERIODE_1min	= 60 * 1000,
	eNORTOS_PERIODE_10min	= 10 * 60 * 1000,
	eNORTOS_PERIODE_1h		= 60 * 60 * 1000,
} task_delay_e;

typedef struct {
	uint32_t tick;
	task_delay_e delay;
	VoidFunctionPointer task_callback;
} noRTOS_task_t;

extern noRTOS_task_t* global_list_of_tasks[NORTOS_NO_OF_MAX_TASK];

/* adding callbacks to schedular */
bool noRTOS_add_task_to_scheduler(noRTOS_task_t *task);

/* runs once and once only just before scheduler */
void noRTOS_setup(void);

/* runs forever call calbacks on a regular basis which have been added using the noRTOS_add_task_to_scheduler() API */
void noRTOS_run_scheduler(void);

/* */
void noRTOS_print_version(void);

/* */
void noRTOS_print_active_task_running(void);


/**
 * *************** temporary UART 2 Handler ***************
 */

#define UART_BUFFER_SIZE 128
extern uint8_t 	uart2_buffer[UART_BUFFER_SIZE];
bool noRTOS_is_UART2_read_line_complete(void);
void noRTOS_UART2_read_byte_with_interrupt(void);
void noRTOS_UART2_clear_rx_buffer(void);
void noRTOS_UART2_echo_whats_been_received(void);
void noRTOS_UART2_receive_byte_callback(void);
#endif /* NORTOS_H_ */
