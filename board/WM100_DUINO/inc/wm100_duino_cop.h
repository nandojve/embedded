/**
 * \file
 *
 * \brief WM100_DUINO-COP board header file.
 *
 * This file contains definitions and services related to the features of the
 * STK600 board.
 *
 * To use this board, define BOARD = USER.
 *
 * Copyright (c) 2013 Atmel Corporation. All rights reserved.
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
#ifndef WM100_DUINO_COP_H
#define WM100_DUINO_COP_H

#include "compiler.h"
#include "led.h"

#define MCU_SOC_NAME					"ATxmega32A4U"
#define BOARD_NAME						"WM-100-DUINO"

#define LED0_GPIO						IOPORT_CREATE_PIN(PORTC, 5)		// CO_BUS
#define LED1_GPIO						IOPORT_CREATE_PIN(PORTC, 4)		// CO_TX
#define LED2_GPIO						IOPORT_CREATE_PIN(PORTC, 3)		// CO_RX

#define LED0							LED0_GPIO
#define LED1							LED1_GPIO
#define LED2							LED2_GPIO

//! Number of LEDs.
#define LED_COUNT						3

#define GPIO_PUSH_BUTTON_0				IOPORT_CREATE_PIN(PORTC, 2)		// PB6
#define GPIO_PUSH_BUTTON_1				IOPORT_CREATE_PIN(PORTA, 2)		// #RST

#define BUTTON_COUNT					2

//! \name Communication interfaces
//@{
#define TRANSCEIVER_EN					IOPORT_CREATE_PIN(PORTA, 4)		// CO_COM_EN

#define TWIC_SCL						IOPORT_CREATE_PIN(PORTC, 0)		// PD0
#define TWIC_SDA						IOPORT_CREATE_PIN(PORTC, 1)		// PD1
#define USARTC1_RXD						IOPORT_CREATE_PIN(PORTC, 6)		// PE
#define USARTC1_TXD						IOPORT_CREATE_PIN(PORTC, 7)		// PE
//@}

//! \name USB Port Interface
//@{
#define USB_SELF_POWERED				1
#define USB_VBUS_GPIO					PIN2_bp							// USB_DETECT
//@}

//! \name Communication interfaces
//@{
#define SENSORS_MPL115A2_EN				IOPORT_CREATE_PIN(PORTA, 6)		// #CO_ALT_SHTN
#define SENSORS_CC2D33S_READY			IOPORT_CREATE_PIN(PORTA, 7)		// #CO_HUM_RDY
#define SENSORS_VCNL40XX_INT			IOPORT_CREATE_PIN(PORTB, 4)		// #CO_LUM_INT
//@}

// Device List
#define VCNL40XX_DEVICE					(0x13)
#define CC2D33S_DEVICE					(0x28)
#define AT24MAC602_MEM_DEVICE			(0x57)
#define AT24MAC602_EUI_DEVICE			(0x5F)
#define MPL115A2_DEVICE					(0x60)

#endif  /* WM100_DUINO_COP_H */
