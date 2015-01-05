/*
 * vcom.c
 *
 * Created: 09/03/2013 19:27:55
 *  Author: nando
 */

#include <compiler.h>
#include <usb_protocol_cdc.h>
#include <ioport.h>
#include <timeout.h>
#include <board.h>

#include "conf_vcom.h"
#include "conf_board.h"
#include "conf_timeout.h"

#include "vcom.h"

#include "uart.h"
#include "terminal.h"
#include "system_config.h"

#if (VCOM_ENABLE == 1)

// VirtualCOM/USART Select
static AVRISP_MKII_VCOMSEL_STATE_t		vcomsel_state		= CMD_STATE_NOT_PRESS;
static volatile	uint8_t					vcomsel_pin_state	= false;
static uint8_t							vcomsel_active		= false;

void vcom_init(void)
{
	ioport_set_pin_dir(VCOM_PIN, IOPORT_DIR_INPUT);
	ioport_set_pin_mode(VCOM_PIN, IOPORT_MODE_PULLUP);
	
	vcomsel_active											= true;
	vcomsel_state											= CMD_STATE_CHANGE_STATE;
}
void vcom_task(void)
{
	vcomsel_pin_state										= ioport_get_pin_level(VCOM_PIN);

	switch(vcomsel_state)
	{
		case CMD_STATE_NOT_PRESS:
		{
			if(!vcomsel_pin_state)
			{
				vcomsel_state								= CMD_STATE_PRESS;
				timeout_start_singleshot(TIMEOUT_1, VCOM_SELECT_TIMEOUT);
			}
			break;
		}
		case CMD_STATE_PRESS:
		{
			if(!vcomsel_pin_state)
			{
				if(timeout_test_expired(TIMEOUT_1))
				{
					vcomsel_state							= CMD_STATE_CHANGE_STATE;
				}
			}
			else
			{
				vcomsel_state								= CMD_STATE_NOT_PRESS;
				timeout_stop(TIMEOUT_1);
			}			
			break;
		}
		case CMD_STATE_CHANGE_STATE:
		{
			vcomsel_state									= CMD_STATE_WAIT_UNPRESS;
			vcomsel_active									= !vcomsel_active;
				
			if(vcomsel_active)
			{
				LED_Off(LED_VBUS);
				uart_close(0);
				terminal_reset();
			}			
			else
			{
				terminal_close();
				uart_open(0);
				LED_On(LED_VBUS);
				LED_Off(LED_RX);
				LED_Off(LED_TX);
			}				

			break;
		}
		case CMD_STATE_WAIT_UNPRESS:
		{
			if(vcomsel_pin_state)
				vcomsel_state								= CMD_STATE_NOT_PRESS;

			break;
		}			
	}
	
	// process virtual com terminal
	if(vcomsel_active)
	{
		terminal_task();
	}
}
#endif