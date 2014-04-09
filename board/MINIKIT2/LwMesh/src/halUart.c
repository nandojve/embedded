/**
 * \file halUart.c
 *
 * \brief ATxmega128a1 UART implementation
 *
 * Copyright (C) 2012 Atmel Corporation. All rights reserved.
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
 * $Id: halUart.c 5223 2012-09-10 16:47:17Z ataradov $
 *
 */

#include <stdbool.h>
#include "hal.h"
#include "halUart.h"
#include "halGpio.h"
#include "config.h"

#ifdef HAL_ENABLE_UART

/*****************************************************************************
*****************************************************************************/
#ifndef HAL_UART_TX_FIFO_SIZE
#define HAL_UART_TX_FIFO_SIZE  0
#endif

#ifndef HAL_UART_RX_FIFO_SIZE
#define HAL_UART_RX_FIFO_SIZE  0
#endif

#ifndef HAL_UART_CHANNEL
#define HAL_UART_CHANNEL       1 // E0
#endif

/*****************************************************************************
*****************************************************************************/
#if HAL_UART_CHANNEL == 1
  #define USARTx            USARTC1
  #define USARTx_DRE_vect   USARTC1_DRE_vect
  #define USARTx_RXC_vect   USARTC1_RXC_vect

  PRAGMA(diag_suppress=Pe177)
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wstrict-prototypes"

  HAL_GPIO_PIN(UART_RX, C, 6);
  HAL_GPIO_PIN(UART_TX, C, 7);

#pragma GCC diagnostic pop
  PRAGMA(diag_default=Pe177)

#else
  #error Unsupported UART channel
#endif

/*****************************************************************************
*****************************************************************************/
typedef struct
{
  uint16_t  head;
  uint16_t  tail;
  uint16_t  size;
  uint16_t  bytes;
  uint8_t   *data;
} FifoBuffer_t;

/*****************************************************************************
*****************************************************************************/
static FifoBuffer_t txFifo;
static uint8_t txData[HAL_UART_TX_FIFO_SIZE+1];

static volatile FifoBuffer_t rxFifo;
static uint8_t rxData[HAL_UART_RX_FIFO_SIZE+1];

static volatile bool udrEmpty;
static volatile bool newData;

/*****************************************************************************
*****************************************************************************/
bool HAL_UartSetBaudrate(USART_t *usart, uint32_t baud, uint32_t cpu_hz);

/*****************************************************************************
*****************************************************************************/
void HAL_UartInit(uint32_t baudrate)
{
  HAL_GPIO_UART_TX_out();
  HAL_GPIO_UART_TX_set();
  HAL_GPIO_UART_RX_in();

// 32MHz at 115200
  USARTx.BAUDCTRLA = 0x2E;
  USARTx.BAUDCTRLB = 0x98;

  USARTx.CTRLB = USART_CLK2X_bm | USART_RXEN_bm | USART_TXEN_bm;
  USARTx.CTRLC = (uint8_t)USART_CHSIZE_8BIT_gc | USART_PMODE_DISABLED_gc;
  USARTx.CTRLA = USART_RXCINTLVL_LO_gc;
  
//  HAL_UartSetBaudrate(&USARTx, baudrate, F_CPU);

  txFifo.data = txData;
  txFifo.size = HAL_UART_TX_FIFO_SIZE;
  txFifo.bytes = 0;
  txFifo.head = 0;
  txFifo.tail = 0;

  rxFifo.data = rxData;
  rxFifo.size = HAL_UART_RX_FIFO_SIZE;
  rxFifo.bytes = 0;
  rxFifo.head = 0;
  rxFifo.tail = 0;

  udrEmpty = true;
  newData = false;
}

/*****************************************************************************
*****************************************************************************/
bool HAL_UartSetBaudrate(USART_t *usart, uint32_t baud, uint32_t cpu_hz)
{
	int8_t exp;
	uint32_t div;
	uint32_t limit;
	uint32_t ratio;
	uint32_t min_rate;
	uint32_t max_rate;

	/*
	 * Check if the hardware supports the given baud rate
	 */
	/* 8 = (2^0) * 8 * (2^0) = (2^BSCALE_MIN) * 8 * (BSEL_MIN) */
	max_rate = cpu_hz / 8;
	/* 4194304 = (2^7) * 8 * (2^12) = (2^BSCALE_MAX) * 8 * (BSEL_MAX+1) */
	min_rate = cpu_hz / 4194304;

	if (!((usart)->CTRLB & USART_CLK2X_bm)) {
		max_rate /= 2;
		min_rate /= 2;
	}

	if ((baud > max_rate) || (baud < min_rate)) {
		return false;
	}

	/* Check if double speed is enabled. */
	if (!((usart)->CTRLB & USART_CLK2X_bm)) {
		baud *= 2;
	}

	/* Find the lowest possible exponent. */
	limit = 0xfffU >> 4;
	ratio = cpu_hz / baud;

	for (exp = -7; exp < 7; exp++) {
		if (ratio < limit) {
			break;
		}

		limit <<= 1;

		if (exp < -3) {
			limit |= 1;
		}
	}

	/*
	 * Depending on the value of exp, scale either the input frequency or
	 * the target baud rate. By always scaling upwards, we never introduce
	 * any additional inaccuracy.
	 *
	 * We are including the final divide-by-8 (aka. right-shift-by-3) in
	 * this operation as it ensures that we never exceeed 2**32 at any
	 * point.
	 *
	 * The formula for calculating BSEL is slightly different when exp is
	 * negative than it is when exp is positive.
	 */
	if (exp < 0) {
		/* We are supposed to subtract 1, then apply BSCALE. We want to
		 * apply BSCALE first, so we need to turn everything inside the
		 * parenthesis into a single fractional expression.
		 */
		cpu_hz -= 8 * baud;

		/* If we end up with a left-shift after taking the final
		 * divide-by-8 into account, do the shift before the divide.
		 * Otherwise, left-shift the denominator instead (effectively
		 * resulting in an overall right shift.)
		 */
		if (exp <= -3) {
			div = ((cpu_hz << (-exp - 3)) + baud / 2) / baud;
		} else {
			baud <<= exp + 3;
			div = (cpu_hz + baud / 2) / baud;
		}
	} else {
		/* We will always do a right shift in this case, but we need to
		 * shift three extra positions because of the divide-by-8.
		 */
		baud <<= exp + 3;
		div = (cpu_hz + baud / 2) / baud - 1;
	}

	(usart)->BAUDCTRLB = (uint8_t)(((div >> 8) & 0X0F) | (exp << 4));
	(usart)->BAUDCTRLA = (uint8_t)div;

	return true;
}

/*****************************************************************************
*****************************************************************************/
void HAL_UartWriteByte(uint8_t byte)
{
  if (txFifo.bytes == txFifo.size)
    return;

  txFifo.data[txFifo.tail++] = byte;
  if (txFifo.tail == txFifo.size)
    txFifo.tail = 0;
  txFifo.bytes++;
}

/*****************************************************************************
*****************************************************************************/
uint8_t HAL_UartReadByte(void)
{
  uint8_t byte;

  PRAGMA(diag_suppress=Pa082)
  ATOMIC_SECTION_ENTER
    byte = rxFifo.data[rxFifo.head++];
    if (rxFifo.head == rxFifo.size)
      rxFifo.head = 0;
    rxFifo.bytes--;
  ATOMIC_SECTION_LEAVE
  PRAGMA(diag_default=Pa082)

  return byte;
}

/*****************************************************************************
*****************************************************************************/
ISR(USARTx_DRE_vect)
{
  udrEmpty = true;
  USARTx.CTRLA &= ~USART_DREINTLVL_gm;
}

/*****************************************************************************
*****************************************************************************/
ISR(USARTx_RXC_vect)
{
  PRAGMA(diag_suppress=Pa082);

  uint8_t status = USARTx.STATUS;
  uint8_t byte = USARTx.DATA;

  if (0 == (status & (USART_FERR_bm | USART_BUFOVF_bm | USART_PERR_bm)))
  {
    if (rxFifo.bytes == rxFifo.size)
      return;

    rxFifo.data[rxFifo.tail++] = byte;
    if (rxFifo.tail == rxFifo.size)
      rxFifo.tail = 0;
    rxFifo.bytes++;

    newData = true;
  }

  PRAGMA(diag_default=Pa082);
}

/*****************************************************************************
*****************************************************************************/
void HAL_UartTaskHandler(void)
{
  if (txFifo.bytes && udrEmpty)
  {
    uint8_t byte;

    byte = txFifo.data[txFifo.head++];
    if (txFifo.head == txFifo.size)
      txFifo.head = 0;
    txFifo.bytes--;

    ATOMIC_SECTION_ENTER
      USARTx.DATA = byte;
      USARTx.CTRLA |= USART_DREINTLVL_gm;
      udrEmpty = false;
    ATOMIC_SECTION_LEAVE
  }

  {
    uint16_t bytes;
    bool new;

    ATOMIC_SECTION_ENTER
      new = newData;
      newData = false;
      bytes = rxFifo.bytes;
    ATOMIC_SECTION_LEAVE

    if (new)
      HAL_UartBytesReceived(bytes);
  }
}

#endif // HAL_ENABLE_UART
