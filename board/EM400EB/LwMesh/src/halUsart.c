/**
 * \file halUart.c
 *
 * \brief ATSAMD20 UART implementation
 *
 * Copyright (C) 2012-2013, Atmel Corporation. All rights reserved.
 *
 * \asf_license_start
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
 * $Id: halUart.c 8367 2013-07-25 17:18:50Z ataradov $
 *
 */

/*- Includes ---------------------------------------------------------------*/
#include <stdbool.h>
#include "hal.h"
#include "halUart.h"
#include "halGpio.h"
#include "config.h"
#include "conf_board.h"
#include <sysclk.h>
#include <serial.h>

/*- Definitions ------------------------------------------------------------*/
#ifndef						HAL_UART_TX_FIFO_SIZE
	#define					HAL_UART_TX_FIFO_SIZE			10
#endif

#ifndef						HAL_UART_RX_FIFO_SIZE
	#define					HAL_UART_RX_FIFO_SIZE			10
#endif

#define						ALL_INTERRUPT_MASK				0xffffffff


/*- Types ------------------------------------------------------------------*/
typedef struct
{
	uint16_t					head;
	uint16_t					tail;
	uint16_t					size;
	uint16_t					bytes;
	uint8_t*					data;
} FifoBuffer_t;

/*- Variables --------------------------------------------------------------*/
static FifoBuffer_t				txFifo;
static uint8_t					txData[HAL_UART_TX_FIFO_SIZE+1];

static volatile FifoBuffer_t	rxFifo;
static uint8_t					rxData[HAL_UART_RX_FIFO_SIZE+1];

static volatile bool			udrEmpty;
static volatile bool			newData;

/*- Implementations --------------------------------------------------------*/

/*************************************************************************//**
*****************************************************************************/
void HAL_UartInit(uint32_t baudrate)
{
	const sam_usart_opt_t		usart_console_settings		=
	{
		baudrate,
		US_MR_CHRL_8_BIT,
		US_MR_PAR_NO,
		US_MR_NBSTOP_1_BIT,
		US_MR_CHMODE_NORMAL,
		0													// This field is only used in IrDA mode.
	};

	/* Enable the peripheral clock in the PMC. */
	sysclk_enable_peripheral_clock(USART_MODULE_ID);

	/* Configure USART in serial mode. */
	usart_init_rs232(USART_MODULE, &usart_console_settings,	sysclk_get_cpu_hz());

	/* Disable all the interrupts. */
	usart_disable_interrupt(USART_MODULE, ALL_INTERRUPT_MASK);

	/* Enable the receiver and transmitter. */
	usart_enable_tx(USART_MODULE);
	usart_enable_rx(USART_MODULE);

	/* Configure and enable interrupt of USART. */
	NVIC_DisableIRQ(USART_MODULE_IRQ);
	NVIC_ClearPendingIRQ(USART_MODULE_IRQ);
	NVIC_EnableIRQ(USART_MODULE_IRQ);

	txFifo.data							= txData;
	txFifo.size							= HAL_UART_TX_FIFO_SIZE;
	txFifo.bytes						= 0;
	txFifo.head							= 0;
	txFifo.tail							= 0;

	rxFifo.data							= rxData;
	rxFifo.size							= HAL_UART_RX_FIFO_SIZE;
	rxFifo.bytes						= 0;
	rxFifo.head							= 0;
	rxFifo.tail							= 0;

	udrEmpty							= true;
	newData								= false;
}

/*************************************************************************//**
*****************************************************************************/
void HAL_UartWriteByte(uint8_t byte)
{
	if(txFifo.bytes == txFifo.size)
		return;

	txFifo.data[txFifo.tail++]			= byte;
	if(txFifo.tail == txFifo.size)
		txFifo.tail						= 0;
	txFifo.bytes++;
}

/*************************************************************************//**
*****************************************************************************/
uint8_t HAL_UartReadByte(void)
{
	uint8_t byte;

	ATOMIC_SECTION_ENTER
		byte							= rxFifo.data[rxFifo.head++];
		if(rxFifo.head == rxFifo.size)
			rxFifo.head					= 0;
		rxFifo.bytes--;
	ATOMIC_SECTION_LEAVE

	return byte;
}

/*************************************************************************//**
*****************************************************************************/
ISR(USART_RX_Vect)
{
	uint32_t		flags				= usart_get_status(USART_MODULE);

	if(flags & US_CSR_TXRDY)
	{
		udrEmpty						= true;
		usart_disable_interrupt(USART_MODULE, US_IDR_TXRDY);
	}

	if(flags & US_CSR_RXRDY)
	{
		uint8_t		byte				= (uint8_t) (USART_MODULE->US_RHR & US_RHR_RXCHR_Msk);

		if(0 == (flags & (US_CSR_OVRE | US_CSR_FRAME | US_CSR_PARE)))
		{
			if(rxFifo.bytes == rxFifo.size)
				return;

			rxFifo.data[rxFifo.tail++]	= byte;
			
			if(rxFifo.tail == rxFifo.size)
				rxFifo.tail				= 0;
			
			rxFifo.bytes++;

			newData						= true;
		}
	}
}

/*************************************************************************//**
*****************************************************************************/
void HAL_UartTaskHandler(void)
{
	if(txFifo.bytes && udrEmpty)
	{
		uint8_t		byte				= txFifo.data[txFifo.head++];
		
		if(txFifo.head == txFifo.size)
			txFifo.head					= 0;
		txFifo.bytes--;

		ATOMIC_SECTION_ENTER
		usart_putchar(USART_MODULE, byte);
		usart_enable_interrupt(USART_MODULE, US_IER_TXRDY);
		udrEmpty					= false;
		ATOMIC_SECTION_LEAVE
	}

	{
		uint16_t	bytes;
		bool		new;

		ATOMIC_SECTION_ENTER
		new							= newData;
		newData						= false;
		bytes						= rxFifo.bytes;
		ATOMIC_SECTION_LEAVE

		if(new)
			HAL_UartBytesReceived(bytes);
	}
}
