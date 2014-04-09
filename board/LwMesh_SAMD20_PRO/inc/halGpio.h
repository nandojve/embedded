/**
 * \file halGpio.h
 *
 * \brief ATSAMD20 GPIO interface
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
 * $Id: halGpio.h 8410 2013-08-08 16:59:55Z ataradov $
 *
 */

#ifndef _HAL_GPIO_H_
#define _HAL_GPIO_H_

/*- Includes ---------------------------------------------------------------*/
#include <stdint.h>
#include "sysTypes.h"

/*- Definitions ------------------------------------------------------------*/
#define MMIO_REG(mem_addr, type) (*(volatile type *)(mem_addr))

#define PORT_INSTANCE_ADDR		REG_PORT_DIR0
#define PORT_INSTANCE_OFFSET	0x80
#define PORT_INSTANCE_REG(port, offset) MMIO_REG(PORT_INSTANCE_ADDR + (PORT_INSTANCE_OFFSET << RES(PORT##port##_INSTANCE)) + offset, uint8_t)
#define PORTA_INSTANCE 0
#define PORTB_INSTANCE 1

#define PERIPHERAL_FUNCTION_A	0x00
#define PERIPHERAL_FUNCTION_B	0x01
#define PERIPHERAL_FUNCTION_C	0x02
#define PERIPHERAL_FUNCTION_D	0x03
#define PERIPHERAL_FUNCTION_E	0x04
#define PERIPHERAL_FUNCTION_F	0x05
#define PERIPHERAL_FUNCTION_G	0x06
#define PERIPHERAL_FUNCTION_H	0x07
#define PORT_PMUX_FUNCTION_OFFSET(bit)	((bit % 2) == 1 ? PORT_PMUX_PMUXO_Pos : PORT_PMUX_PMUXE_Pos)
#define PORT_PMUX_FUNCTION_MASK(bit)	((bit % 2) == 1 ? PORT_PMUX_PMUXO_Msk : PORT_PMUX_PMUXE_Msk)
#define PORT_PMUX_FUNCTION_NOTMASK(bit)	((bit % 2) == 1 ? PORT_PMUX_PMUXE_Msk : PORT_PMUX_PMUXO_Msk)

#define  CONCAT(a, b) XCAT(a, b)
#define  XCAT(a, b)   a ## b
#define  RES(R)       R

#define HAL_GPIO_PIN(name, port, bit) \
	INLINE void    HAL_GPIO_##name##_set(void)      { CONCAT(REG_PORT_OUTSET, RES(PORT##port##_INSTANCE))	= (1 << bit); }				\
	INLINE void    HAL_GPIO_##name##_clr(void)      { CONCAT(REG_PORT_OUTCLR, RES(PORT##port##_INSTANCE))	= (1 << bit); }				\
	INLINE void    HAL_GPIO_##name##_toggle(void)   { CONCAT(REG_PORT_OUTTGL, RES(PORT##port##_INSTANCE))	= (1 << bit); }				\
	INLINE void    HAL_GPIO_##name##_in(void)       { CONCAT(REG_PORT_DIRCLR, RES(PORT##port##_INSTANCE))	= (1 << bit);				\
	PORT_INSTANCE_REG(port, PORT_PINCFG_OFFSET + bit)														|= PORT_PINCFG_INEN;		\
	PORT_INSTANCE_REG(port, PORT_PINCFG_OFFSET + bit)														&= ~PORT_PINCFG_PULLEN; }	\
	INLINE void    HAL_GPIO_##name##_out(void)      { CONCAT(REG_PORT_DIRSET, RES(PORT##port##_INSTANCE))	= (1 << bit);				\
	PORT_INSTANCE_REG(port, PORT_PINCFG_OFFSET + bit)														|= PORT_PINCFG_INEN; }		\
	INLINE void    HAL_GPIO_##name##_pullup(void)   { CONCAT(REG_PORT_OUTSET, RES(PORT##port##_INSTANCE))	= (1 << bit);				\
	PORT_INSTANCE_REG(port, PORT_PINCFG_OFFSET + bit)														|= PORT_PINCFG_PULLEN; }	\
	INLINE uint8_t HAL_GPIO_##name##_read(void)     { return (CONCAT(REG_PORT_IN, RES(PORT##port##_INSTANCE)) & (1 << bit)) != 0; }		\
	INLINE uint8_t HAL_GPIO_##name##_state(void)    { return (CONCAT(REG_PORT_DIR, RES(PORT##port##_INSTANCE)) & (1 << bit)) != 0; }	\
	INLINE void    HAL_GPIO_##name##_pmuxen(void)   {																					\
	PORT_INSTANCE_REG(port, PORT_PINCFG_OFFSET + bit)														|= PORT_PINCFG_PMUXEN; }	\
	INLINE void    HAL_GPIO_##name##_pmuxdis(void)  {																					\
	PORT_INSTANCE_REG(port, PORT_PINCFG_OFFSET + bit)														&= ~PORT_PINCFG_PMUXEN; }

#define HAL_GPIO_PMUX_FUNCTION(port, bit, function)	{																					\
	uint8_t			value												= PORT_INSTANCE_REG(port, PORT_PMUX_OFFSET + (bit / 2));		\
	value																&= PORT_PMUX_FUNCTION_NOTMASK(bit);								\
	PORT_INSTANCE_REG(port, PORT_PMUX_OFFSET + (bit / 2))				= (value | (function & 0x03)); }

#endif // _HAL_GPIO_H_
