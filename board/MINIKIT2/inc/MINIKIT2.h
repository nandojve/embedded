/**
 * \file
 *
 * \brief XMEGA_RF212B_ZIGBIT board header file.
 *
 * This file contains definitions and services related to the features of the
 * XMEGA_RF212B_ZIGBIT board.
 *
 * To use this board define BOARD=XMEGA_RF212B_ZIGBIT
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
#ifndef MINIKIT2_H
#define MINIKIT2_H

#include <compiler.h>

#define MCU_SOC_NAME					"ATxmega192A3"
#define BOARD_NAME						"MINIKIT2"

#define COMMON_SENSOR_PLATFORM

/**
 * \name LEDs
 *
 * LED0 and LED1 are single yellow LEDs that are active low..
 * LED2 and LED3 are inside one package (Led red and green close
 * to USB connector) but can be controlled individually.
 * LED2 has a red color and is active low. This LED can be
 * used for general purposes.
 * LED3 has a green color and is active high. By default this
 * LED is on since it shall indicate that power is applied to the
 * board. By pulling the gate of a N-FET low it is possible to
 * turn off the LED if needed.
 */
#define LED0_GPIO						IOPORT_CREATE_PIN(PORTE, 1)
#define LED0							LED0_GPIO
#define LED_COUNT                       1

#define GPIO_PUSH_BUTTON_0              IOPORT_CREATE_PIN(PORTA, 2)

// name Connections of the AT86RFX transceiver
#define AT86RFX_SPI						&SPID
#define AT86RFX_RST_PIN					IOPORT_CREATE_PIN(PORTD, 0)
#define AT86RFX_MISC_PIN				IOPORT_CREATE_PIN(PORTD, 3)
#define AT86RFX_IRQ_PIN					IOPORT_CREATE_PIN(PORTD, 2)
#define AT86RFX_SLP_PIN					IOPORT_CREATE_PIN(PORTD, 1)
#define AT86RFX_SPI_CS					IOPORT_CREATE_PIN(PORTD, 4)
#define AT86RFX_SPI_MOSI				IOPORT_CREATE_PIN(PORTD, 5)
#define AT86RFX_SPI_MISO				IOPORT_CREATE_PIN(PORTD, 6)
#define AT86RFX_SPI_SCK					IOPORT_CREATE_PIN(PORTD, 7)

#define AT86RFX_INTC_INIT()				ioport_configure_pin(AT86RFX_IRQ_PIN, IOPORT_DIR_INPUT); \
										PORTD.PIN2CTRL = PORT_ISC0_bm; \
										PORTD.INT0MASK = PIN2_bm; \
										PORTD.INTFLAGS = PORT_INT0IF_bm;

#define AT86RFX_ISR()					ISR(PORTD_INT0_vect)

/** Enables the transceiver main interrupt. */
#define ENABLE_TRX_IRQ()				(PORTD.INTCTRL |= PORT_INT0LVL_gm)

/** Disables the transceiver main interrupt. */
#define DISABLE_TRX_IRQ()				(PORTD.INTCTRL &= ~PORT_INT0LVL_gm)

/** Clears the transceiver main interrupt. */
#define CLEAR_TRX_IRQ()					(PORTD.INTFLAGS = PORT_INT0IF_bm)

/*
 * This macro saves the trx interrupt status and disables the trx interrupt.
 */
#define ENTER_TRX_REGION()				{ uint8_t irq_mask = PORTD.INTCTRL; PORTD.INTCTRL &= ~PORT_INT0LVL_gm

/*
 *  This macro restores the transceiver interrupt status
 */
#define LEAVE_TRX_REGION()				PORTD.INTCTRL = irq_mask; }

#endif /* MINIKIT2_H */
