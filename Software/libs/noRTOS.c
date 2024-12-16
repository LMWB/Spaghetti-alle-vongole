#include "noRTOS.h"
#include "hardwareGlobal.h"
#include <string.h>
/**
 * *************** Task Schedular ***************
 */

noRTOS_task_t *global_list_of_tasks[NORTOS_NO_OF_MAX_TASK];
static uint32_t number_of_active_task = 0;


void noRTOS_print_version(void){
	printf("booting from noRTOS Version: %d.%d\n", NORTOS_VERSION_MAYOR, NORTOS_VERSION_MINOR);
}

void noRTOS_print_active_task_running(void){
	printf("starting noRTOS scheduler with %ld task in queue\n", number_of_active_task);
}

bool noRTOS_add_task_to_scheduler(noRTOS_task_t *task){
	if(number_of_active_task < NORTOS_NO_OF_MAX_TASK){
		global_list_of_tasks[number_of_active_task] = task;
		number_of_active_task++;
		return true;
	}
	return false;
}

/* override this with your implementation */
__weak void noRTOS_setup(void){
	;
}

void noRTOS_run_scheduler(void) {

	noRTOS_print_version();
	noRTOS_print_active_task_running();
	noRTOS_setup();

	while(1){
		uint32_t now = NORTOS_SCHEDULAR_GET_TICK();

		for(uint_fast32_t i = 0; i < number_of_active_task; i++){
			if( (now - global_list_of_tasks[i]->tick) >= global_list_of_tasks[i]->delay){
				global_list_of_tasks[i]->tick = now;
				global_list_of_tasks[i]->task_callback();
			}
		}
	}
}

/**
 * *************** temporary UART 2 Handler ***************
 */

uint8_t uart2_buffer[UART_BUFFER_SIZE] =  {0};
static uint16_t rx_size = 0;
static bool uart2_read_line_complete = false;

bool noRTOS_is_UART2_read_line_complete(void){
	return uart2_read_line_complete;
}

void noRTOS_UART2_read_byte_with_interrupt(void){
	HAL_UART_Receive_IT(&huart2, &uart2_buffer[rx_size], 1);
}

void noRTOS_UART2_clear_rx_buffer(void){
	rx_size = 0;
	uart2_read_line_complete = false;
	memset(uart2_buffer, 0, UART_BUFFER_SIZE);
	noRTOS_UART2_read_byte_with_interrupt();
}

void noRTOS_UART2_echo_whats_been_received(void)
{
	UART_SEND_TERMINAL( uart2_buffer, rx_size);
}

void noRTOS_UART2_receive_byte_callback(void){
	/* check if read line complete */
	if(rx_size >= 3 && uart2_buffer[rx_size-1] == '\r' && uart2_buffer[rx_size] == '\n'){
		rx_size++;
		uart2_read_line_complete = true;
	}else{
		rx_size++;
		noRTOS_UART2_read_byte_with_interrupt();
	}
}
