/**
 * \file
 *
 * \brief UART functions for AVR Xmega
 *
 * Copyright (c) 2011 - 2012 Atmel Corporation. All rights reserved.
 *
 * \asf_license_start
 *
 * \page License
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice,
 *    this list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 *
 * 3. The name of Atmel may not be used to endorse or promote products derived
 *    from this software without specific prior written permission.
 *
 * 4. This software may only be redistributed and used in connection with an
 *    Atmel microcontroller product.
 *
 * THIS SOFTWARE IS PROVIDED BY ATMEL "AS IS" AND ANY EXPRESS OR IMPLIED
 * WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NON-INFRINGEMENT ARE
 * EXPRESSLY AND SPECIFICALLY DISCLAIMED. IN NO EVENT SHALL ATMEL BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
 * STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN
 * ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 *
 * \asf_license_stop
 *
 */

#include <sysclk.h>

#include "conf_board.h"
#include "conf_usb.h"
#include "conf_timeout.h"
#include "conf_usart_serial.h"

#include "uart.h"
#include "delay.h"

static uint8_t usart_ctrl_active = true;
static uint8_t usart_rst_slave_flag = 0;
static volatile bool driver_b_cdc_enable = false;

void uart_config(uint8_t port, usb_cdc_line_coding_t * cfg)
{
	uint8_t reg_ctrlc;
	uint16_t bsel;

	reg_ctrlc = USART_CMODE_ASYNCHRONOUS_gc;

	switch(cfg->bCharFormat)
	{
		case CDC_STOP_BITS_2:
			reg_ctrlc |= USART_SBMODE_bm;
			break;
		case CDC_STOP_BITS_1:
		default:
			// Default stop bit = 1 stop bit
			break;
	}

	switch(cfg->bParityType)
	{
		case CDC_PAR_EVEN:
			reg_ctrlc |= USART_PMODE_EVEN_gc;
			break;
		case CDC_PAR_ODD:
			reg_ctrlc |= USART_PMODE_ODD_gc;
			break;
		default:
		case CDC_PAR_NONE:
			reg_ctrlc |= USART_PMODE_DISABLED_gc;
			break;
	}

	switch(cfg->bDataBits)
	{
		case 5:
			reg_ctrlc |= USART_CHSIZE_5BIT_gc;
			break;
		case 6:
			reg_ctrlc |= USART_CHSIZE_6BIT_gc;
			break;
		case 7:
			reg_ctrlc |= USART_CHSIZE_7BIT_gc;
			break;
		default:
		case 8:
			reg_ctrlc |= USART_CHSIZE_8BIT_gc;
			break;
		case 9:
			reg_ctrlc |= USART_CHSIZE_9BIT_gc;
			break;
	}

	sysclk_enable_module(USART_PORT_SYSCLK, USART_SYSCLK);
	// Set configuration
	USART.CTRLC = reg_ctrlc;
	// Update baudrate
	//usart_set_baudrate(&USART, cfg->dwDTERate, sysclk_get_cpu_hz());
	usart_set_baudrate(&USART, 115200, sysclk_get_cpu_hz());
}

void uart_open(uint8_t port)
{
	sysclk_enable_module(USART_PORT_SYSCLK, USART_SYSCLK);
	// Open UART communication
	USART_PORT.DIRSET = USART_PORT_PIN_TX; // TX as output.
	USART_PORT.DIRCLR = USART_PORT_PIN_RX; // RX as input.

	// Enable both RX and TX
	USART.CTRLB = USART_RXEN_bm | USART_TXEN_bm | USART_CLK2X_bm;
	// Enable interrupt with priority higher than USB
	USART.CTRLA = (register8_t) USART_RXCINTLVL_HI_gc | (register8_t) USART_DREINTLVL_OFF_gc;
}

void uart_close(uint8_t port)
{
	sysclk_disable_module(USART_PORT_SYSCLK, USART_SYSCLK);
	// Disable interrupts
	USART.CTRLA = 0;
	// Close RS232 communication
	USART.CTRLB = 0;
}

void uart_rx_notify(uint8_t port)
{
	if(!usart_ctrl_active)
		return;

	// If UART is open
	if(USART.CTRLA != 0)
	{
		// Enable UART TX interrupt to send values
		USART.CTRLA = (register8_t) USART_RXCINTLVL_HI_gc | (register8_t)
		USART_DREINTLVL_HI_gc;
	}
}

void uart_set_dtr(uint8_t port, bool b_enable)
{
	if(b_enable)
	{
		irqflags_t flags = cpu_irq_save(); // Protect udi_cdc_state
		usart_rst_slave_flag = 1;
		cpu_irq_restore(flags);
	}
}

void uart_task(void)
{
#if (UART_RST_SVALE_ENABLED == 1)
	if(usart_rst_slave_flag != 0)
	{
		LED_Off(LED_VBUS);

		irqflags_t flags = cpu_irq_save();
		usart_rst_slave_flag = 0;
		cpu_irq_restore(flags);
		timeout_start_singleshot(TIMEOUT_RST_SLAVE, UART_RST_SLAVE_TIMEOUT);
		gpio_configure_pin(GPIO_PUSH_BUTTON_0, IOPORT_DIR_OUTPUT | IOPORT_INIT_LOW);
		gpio_configure_pin(GPIO_PUSH_BUTTON_1, IOPORT_DIR_OUTPUT | IOPORT_INIT_LOW);
	}
	
	if(timeout_test_and_clear_expired(TIMEOUT_RST_SLAVE))
	{
		gpio_configure_pin(GPIO_PUSH_BUTTON_1, IOPORT_DIR_INPUT | IOPORT_PULL_UP);
		delay_ms(10);
		gpio_configure_pin(GPIO_PUSH_BUTTON_0, IOPORT_DIR_INPUT | IOPORT_PULL_UP);
		LED_On(LED_VBUS);
	}
#endif
	
	if(timeout_test_expired(TIMEOUT_RX))
	{
		LED_Off(LED_RX);
	}
	if(timeout_test_expired(TIMEOUT_TX))
	{
		LED_Off(LED_TX);
	}
}
void uart_vbus_action(bool b_high)
{
	if(b_high)
	{
		// Attach USB Device
		udc_attach();
	}
	else
	{
		// VBUS not present
		udc_detach();	
	}
}
void uart_usb_cdc_init(void)
{
	LED_On(LED_VBUS);
	
	uart_open(0);
	
	// Start USB stack to authorize VBus monitoring
	stdio_usb_init();

	if(!udc_include_vbus_monitoring())
	{
		// VBUS monitoring is not available on this product
#if (USB_SELF_POWERED == 1)
		// SO, VBUS is monitoring by I/O PD 2 IRQ
		PORTD.INTCTRL				|= (PORT_INT0LVL0_bm | PORT_INT0LVL1_bm);	// High Level Int
		PORTD.INT0MASK				= (1 << USB_VBUS_GPIO);						// VBUS pin
		PORTD.PIN5CTRL				= PORT_ISC_BOTHEDGES_gc;					// Enable Both Edge Int
		PMIC.CTRL					|= PMIC_HILVLEN_bm;							// Enable HIGH Level Int
#else
		// activate always!
		driver_vbus_action(true);
#endif
	}
}

ISR(USART_RX_Vect)
{
	uint8_t value;

	// Data received
	if(0 != (USART.STATUS & (USART_FERR_bm | USART_BUFOVF_bm)))
	{
		udi_cdc_signal_framing_error();
//		ui_com_error();
	}

	// Transfer UART RX fifo to CDC TX
	value = USART.DATA;
	if(!udi_cdc_is_tx_ready())
	{
		// Fifo full
		udi_cdc_signal_overrun();
//		ui_com_overflow();
	}
	else
	{
		timeout_start_singleshot(TIMEOUT_RX, TIMEOUT_TX_RX);
		LED_On(LED_RX);

		udi_cdc_putc(value);
	}
}

ISR(USART_DRE_Vect)
{
	// Data send
	if(udi_cdc_is_rx_ready())
	{
		// Transmit next data
		timeout_start_singleshot(TIMEOUT_TX, TIMEOUT_TX_RX);
		LED_On(LED_TX);

		USART.DATA = udi_cdc_getc();
	}
	else
	{
		// Fifo empty then Stop UART transmission
		USART.CTRLA = (register8_t) USART_RXCINTLVL_HI_gc | (register8_t) USART_DREINTLVL_OFF_gc;
	}
}
#if (USB_SELF_POWERED == 1)
ISR(PORTD_INT0_vect)
{
	uint8_t						input						= PORTD_IN;
	input													&= (1 << USB_VBUS_GPIO);
	bool						vbus						= (input == (1 << USB_VBUS_GPIO));
	uart_vbus_action(vbus);
}
#endif