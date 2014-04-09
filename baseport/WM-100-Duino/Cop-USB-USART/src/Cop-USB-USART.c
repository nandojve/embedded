/*
 * Cop_USB_USART.c
 *
 * Created: 04/04/2014 18:24:40
 *  Author: nando
 */ 

#include <board.h>
#include <sysclk.h>
#include <stdio_usb.h>
#include <delay.h>
#include <string.h>
#include <sleepmgr.h>

#include "Terminal.h"
#include "system_config.h"

void myterminal_process_command(TerminalState* state, char* command);

/**
 * \brief main function
 */
int main (void)
{
	/* Initialize basic board support features.
	 * - Initialize system clock sources according to device-specific
	 *   configuration parameters supplied in a conf_clock.h file.
	 * - Set up GPIO and board-specific features using additional configuration
	 *   parameters, if any, specified in a conf_board.h file.
	 */
	sysclk_init();
	
	// Initialize System
	sleepmgr_init();
	timeout_init();
	
	// Initialize Board
	board_init();
	system_config_init();

	// Initialize interrupt vector table support.
	irq_initialize_vectors();

	// Enable interrupts
	cpu_irq_enable();

	/* Call a local utility routine to initialize C-Library Standard I/O over
	 * a USB CDC protocol. Tunable parameters in a conf_usb.h file must be
	 * supplied to configure the USB device correctly.
	 */
	stdio_usb_init();
	vcom_init();

	TerminalState* state = terminal_init(myterminal_process_command);
	
//	while(*state != TS_BOOT)
//		terminal_task();

	for(;;)
	{
		vcom_task();
	}
}

void myterminal_process_command(TerminalState* state, char* command)
{
	if(!strncmp(command, "msg", 3))
	{
		printf("NAK\n");
		
		*state = TS_INIT;
	}
	else if(!strncmp(command, "size", 4))
	{
		printf("NAK\n");
		
		*state = TS_INIT;
	}
	else if(!strncmp(command, "show", 4))
	{
		printf("NAK\n");
		
		*state = TS_INIT;
	}
	else if(!strncmp(command, "boot", 4))
	{
		*state = TS_BOOT;
	}
}