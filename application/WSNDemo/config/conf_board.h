/**
 * \file
 *
 * \brief Board configuration
 *
 * Copyright (c) 2011 Atmel Corporation. All rights reserved.
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
#ifndef CONF_BOARD_H_INCLUDED
#define CONF_BOARD_H_INCLUDED

#include <platform.h>

#if !defined(ARMTYPE)

// Definition of SYS Timer
#define		SYSTIMER										TCC1
#define		SYSTIMER_PORT_SYSCLK							SYSCLK_PORT_C
#define		SYSTIMER_SYSCLOCK								SYSCLK_TC1
#define		SYSTIMER_IRQ_OVF								TCC1_OVF_vect
#define		SYSTIMER_IRQ_CCB								TCC1_CCB_vect

// Definition of RF SPI
#define		RF_SPI_PORT_SYSCLK								SYSCLK_PORT_C
#define		RF_SPI_SYSCLCK									SYSCLK_SPI

// Definition of USART connection for this example
#define		USART_MODULE									USARTE0
#define		USART_RX_Vect									USARTE0_RXC_vect
#define		USART_DRE_Vect									USARTE0_DRE_vect
#define		USART_PORT										PORTE
#define		USART_PORT_PIN_TX								(1 << PIN3)  // PC3 (TXC0)
#define		USART_PORT_PIN_RX								(1 << PIN2)  // PC2 (RXC0)
#define		USART_PORT_SYSCLK								SYSCLK_PORT_E
#define		USART_SYSCLK									SYSCLK_USART0

// Definition of USB CDC Device
#define		USB_CLASS_CDC_DEVICE

#elif defined(PLATFORM_SAMD20_PRO)
// Definition of SYS Timer
#define		SYSTIMER										TC4->COUNT16
#define		SYSTIMER_PORT_SYSCLK							
#define		SYSTIMER_SYSCLOCK								
#define		SYSTIMER_OVF_Vect								TC4_Handler
#define		SYSTIMER_CCB_Vect

// Definition of RF SPI
#define		RF_SPI_PORT_SYSCLK								
#define		RF_SPI_SYSCLCK									
#define		RF_SPI_PORT										
#define		RF_SPI_MODULE									SERCOM1
#define		RF_PHY_Vect										EIC_Handler

// Definition of USART connection for this example
#define		USART_MODULE									SERCOM3
#define		USART_RX_Vect									SERCOM3_Handler
#define		USART_DRE_Vect									
#define		USART_PORT										
#define		USART_PORT_PIN_TX								
#define		USART_PORT_PIN_RX								
#define		USART_PORT_SYSCLK								
#define		USART_SYSCLK									

#elif defined(PLATFORM_SAM4S_EK) || defined(PLATFORM_SAM4S_XPLAINED)

// Definition of SYS Timer
#define		SYSTIMER										TC0
#define		SYSTIMER_TICK_CHANNEL							0
#define		SYSTIMER_TICK_CHANNEL_ID						ID_TC0
#define		SYSTIMER_TICK_CHANNEL_IRQn						TC0_IRQn
#define		SYSTIMER_DELAY_CHANNEL							1
#define		SYSTIMER_DELAY_CHANNEL_ID						ID_TC1
#define		SYSTIMER_DELAY_CHANNEL_IRQn						TC1_IRQn
#define		SYSTIMER_OVF_Vect								TC0_Handler
#define		SYSTIMER_CCB_Vect								TC1_Handler

// Definition of RF SPI
#define		CONF_BOARD_AT86RFX
#define		RF_SPI_PORT_SYSCLK
#define		RF_SPI_SYSCLCK
#define		RF_SPI_PORT
#define		RF_SPI_MODULE									SPI
#define		RF_SPI_MODULE_ID								ID_SPI

#if defined(PLATFORM_SAM4S_EK)
#define		RF_PHY_Vect										PIOA_Handler
#define		RF_PHY_Vect_IRQn								PIOA_IRQn
#define		RF_PHY_Vect_PIO									PIOA

#define		RF_SPI_CHIP_SELECT								AT86RFX_SPI_CS_NUM
//#define		CONF_BOARD_SPI_NPCS2
//#define		CONF_BOARD_SPI_NPCS2_GPIO						SPI_NPCS2_PB2_GPIO
//#define		CONF_BOARD_SPI_NPCS2_FLAGS						SPI_NPCS2_PB2_FLAGS
#else // defined(PLATFORM_SAM4S_XPLAINED)
#define		RF_PHY_Vect										PIOB_Handler
#define		RF_PHY_Vect_IRQn								PIOB_IRQn
#define		RF_PHY_Vect_PIO									PIOB

#define		RF_SPI_CHIP_SELECT								AT86RFX_SPI_CS_NUM
//#define		CONF_BOARD_SPI_NPCS1
//#define		CONF_BOARD_SPI_NPCS1_GPIO						SPI_NPCS1_PA31_GPIO
//#define		CONF_BOARD_SPI_NPCS1_FLAGS						SPI_NPCS1_PA31_FLAGS
#endif

// Definition of USART connection for this example
#if defined(PLATFORM_SAM4S_EK)
#define		USART_MODULE_ID									ID_USART1
#define		USART_MODULE									USART1
#define		USART_MODULE_IRQ								USART1_IRQn
#define		USART_RX_Vect									USART1_Handler
#define		USART_DRE_Vect
#define		USART_PORT
#define		USART_PORT_PIN_TX
#define		USART_PORT_PIN_RX
#define		USART_PORT_SYSCLK
#define		USART_SYSCLK

#define		CONF_BOARD_USART_RXD
#define		CONF_BOARD_USART_TXD
#define		CONF_BOARD_USB_PORT
#else // defined(PLATFORM_SAM4S_XPLAINED)
#define		USART_MODULE_ID									ID_UART0
#define		USART_MODULE									UART0
#define		USART_MODULE_IRQ								UART0_IRQn
#define		USART_RX_Vect									UART0_Handler

#define		CONF_BOARD_UART_CONSOLE
#define		CONSOLE_UART_ID									USART_MODULE_ID
#define		CONF_BOARD_UART_RXD
#define		CONF_BOARD_UART_TXD
#endif

// Definition of USB CDC Device
#define		USB_CLASS_CDC_DEVICE

#else
#error "Platform " PLATFORM " is invalid."
#endif

#endif /* CONF_BOARD_H_INCLUDED */
