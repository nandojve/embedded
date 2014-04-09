/**
 * \file
 *
 * \brief Chip-specific system clock manager configuration
 *
 * Copyright (c) 2011-2012 Atmel Corporation. All rights reserved.
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
#ifndef CONF_CLOCK_H_INCLUDED
#define CONF_CLOCK_H_INCLUDED

//#include <platform.h>
//
//#if !defined(ARMTYPE)
//#elif defined(PLATFORM_SAMD20_PRO)
////! Configuration using On-Chip RC oscillator at 48MHz
////! The RC oscillator is calibrated via USB Start Of Frame
////! Clk USB     = 48MHz (used by USB)
////! Clk sys     = 48MHz
////! Clk cpu/per = 24MHz
//#define CONFIG_USBCLK_SOURCE     USBCLK_SRC_RCOSC
//#define CONFIG_OSC_RC32_CAL      48000000UL
//
//#define CONFIG_OSC_AUTOCAL_RC32MHZ_REF_OSC  OSC_ID_USBSOF
//
//#define CONFIG_SYSCLK_SOURCE     SYSCLK_SRC_RC32MHZ
//#define CONFIG_SYSCLK_PSADIV     SYSCLK_PSADIV_2
//#define CONFIG_SYSCLK_PSBCDIV    SYSCLK_PSBCDIV_1_1
//
///*
////! Use external board OSC (8MHz)
////! Clk pll     = 48MHz (used by USB)
////! Clk sys     = 48MHz
////! Clk cpu/per = 12MHz
//
//#define CONFIG_PLL0_SOURCE       PLL_SRC_XOSC
//#define CONFIG_PLL0_MUL          6
//#define CONFIG_PLL0_DIV          1
//
//#define CONFIG_USBCLK_SOURCE     USBCLK_SRC_PLL
//
//#define CONFIG_SYSCLK_SOURCE     SYSCLK_SRC_PLL
//#define CONFIG_SYSCLK_PSADIV     SYSCLK_PSADIV_2
//#define CONFIG_SYSCLK_PSBCDIV    SYSCLK_PSBCDIV_1_2
//*/
//#elif defined(PLATFORM_SAM4S_EK) || defined(PLATFORM_SAM4S_XPLAINED)
//// ===== System Clock (MCK) Source Options
////#define CONFIG_SYSCLK_SOURCE        SYSCLK_SRC_SLCK_RC
////#define CONFIG_SYSCLK_SOURCE        SYSCLK_SRC_SLCK_XTAL
////#define CONFIG_SYSCLK_SOURCE        SYSCLK_SRC_SLCK_BYPASS
////#define CONFIG_SYSCLK_SOURCE        SYSCLK_SRC_MAINCK_4M_RC
////#define CONFIG_SYSCLK_SOURCE        SYSCLK_SRC_MAINCK_8M_RC
////#define CONFIG_SYSCLK_SOURCE        SYSCLK_SRC_MAINCK_12M_RC
////#define CONFIG_SYSCLK_SOURCE        SYSCLK_SRC_MAINCK_XTAL
////#define CONFIG_SYSCLK_SOURCE        SYSCLK_SRC_MAINCK_BYPASS
//#define CONFIG_SYSCLK_SOURCE        SYSCLK_SRC_PLLACK
////#define CONFIG_SYSCLK_SOURCE        SYSCLK_SRC_PLLBCK
//
//// ===== System Clock (MCK) Prescaler Options   (Fmck = Fsys / (SYSCLK_PRES))
////#define CONFIG_SYSCLK_PRES          SYSCLK_PRES_1
////#define CONFIG_SYSCLK_PRES          SYSCLK_PRES_2
////#define CONFIG_SYSCLK_PRES          SYSCLK_PRES_4
////#define CONFIG_SYSCLK_PRES          SYSCLK_PRES_8
////#define CONFIG_SYSCLK_PRES          SYSCLK_PRES_16
////#define CONFIG_SYSCLK_PRES          SYSCLK_PRES_32
////#define CONFIG_SYSCLK_PRES          SYSCLK_PRES_64
//#define CONFIG_SYSCLK_PRES          SYSCLK_PRES_3
//
//// ===== PLL0 (A) Options   (Fpll = (Fclk * PLL_mul) / PLL_div)
//// Use mul and div effective values here.
//#define CONFIG_PLL0_SOURCE          PLL_SRC_MAINCK_XTAL
//#define CONFIG_PLL0_MUL             16
//#define CONFIG_PLL0_DIV             2
//
//// ===== PLL1 (B) Options   (Fpll = (Fclk * PLL_mul) / PLL_div)
//// Use mul and div effective values here.
////#define CONFIG_PLL1_SOURCE          PLL_SRC_MAINCK_XTAL
////#define CONFIG_PLL1_MUL             16
////#define CONFIG_PLL1_DIV             2
//
//// ===== USB Clock Source Options   (Fusb = FpllX / USB_div)
//// Use div effective value here.
//#define CONFIG_USBCLK_SOURCE        USBCLK_SRC_PLL0
////#define CONFIG_USBCLK_SOURCE        USBCLK_SRC_PLL1
//#define CONFIG_USBCLK_DIV           2
//
//// ===== Target frequency (System clock)
//// - XTAL frequency: 12MHz
//// - System clock source: PLLA
//// - System clock prescaler: 3 (divided by 3)
//// - PLLA source: XTAL
//// - PLLA output: XTAL * 16 / 1
//// - System clock: 12 * 16 / 2 / 3 = 32MHz
//// ===== Target frequency (USB Clock)
//// - USB clock source: PLLA
//// - USB clock divider: 2 (divided by 2)
//// - USB clock: 12 * 16 / 2 / 2 = 48MHz
//#else
//
//#if (PLATFORM == mega)
//
/* ===== System Clock Source Options */
#define SYSCLK_SRC_RC16MHZ				0
#define SYSCLK_SRC_RC128KHZ				1
#define SYSCLK_SRC_TRS16MHZ				2
#define SYSCLK_SRC_RC32KHZ				3
#define SYSCLK_SRC_XOC16MHZ				4
#define SYSCLK_SRC_EXTERNAL				5

//#define  SYSCLK_SOURCE				SYSCLK_SRC_RC16MHZ
//#define SYSCLK_SOURCE					SYSCLK_SRC_RC128KHZ
#define SYSCLK_SOURCE					SYSCLK_SRC_TRS16MHZ
//#define SYSCLK_SOURCE					SYSCLK_SRC_XOC16MHZ

/* ===== System Clock Bus Division Options */

#define CONFIG_SYSCLK_PSDIV				SYSCLK_PSDIV_1

//#elif (PLATFORM == xmega)
//
//
///* #define CONFIG_SYSCLK_SOURCE			SYSCLK_SRC_RC2MHZ */
//#define CONFIG_SYSCLK_SOURCE			SYSCLK_SRC_RC32MHZ
///* #define CONFIG_SYSCLK_SOURCE			SYSCLK_SRC_RC32KHZ */
///* #define CONFIG_SYSCLK_SOURCE			SYSCLK_SRC_XOSC */
///* #define CONFIG_SYSCLK_SOURCE			SYSCLK_SRC_PLL */
//
///* Fbus = Fsys / (2 ^ BUS_div) */
//#define CONFIG_SYSCLK_PSADIV			SYSCLK_PSADIV_1
//#define CONFIG_SYSCLK_PSBCDIV			SYSCLK_PSBCDIV_1_1
//
///* #define CONFIG_PLL0_SOURCE			PLL_SRC_XOSC */
///* #define CONFIG_PLL0_SOURCE			PLL_SRC_RC2MHZ */
//#define CONFIG_PLL0_SOURCE				PLL_SRC_RC32MHZ
//
///* Fpll = (Fclk * PLL_mul) / PLL_div */
//#define CONFIG_PLL0_MUL					6					// 48MHz
//#define CONFIG_PLL0_DIV					1
//
///* External oscillator frequency range */
///* 0.4 to 2 MHz frequency range */
///* define CONFIG_XOSC_RANGE XOSC_RANGE_04TO2 */
///* 2 to 9 MHz frequency range */
///* define CONFIG_XOSC_RANGE XOSC_RANGE_2TO9 */
///* 9 to 12 MHz frequency range */
///* define CONFIG_XOSC_RANGE XOSC_RANGE_9TO12 */
///* 12 to 16 MHz frequency range */
///* define CONFIG_XOSC_RANGE XOSC_RANGE_12TO16 */
//
///* DFLL autocalibration */
////#define CONFIG_OSC_AUTOCAL_RC32MHZ_REF_OSC	OSC_ID_RC32KHZ		// Internal 32kHz
//#define CONFIG_OSC_AUTOCAL_RC32MHZ_REF_OSC		OSC_ID_XOSC			// External 32.768kHz
//
///* ***************************************************************
 //* **                  CONFIGURATION WITH USB                   **
 //* ***************************************************************
 //* The following clock configuraiton can be used for USB operation
 //* It allows to operate USB using On-Chip RC oscillator at 48MHz
 //* The RC oscillator is calibrated via USB Start Of Frame
 //* Clk USB     = 48MHz (used by USB)
 //* Clk sys     = 48MHz
 //* Clk cpu/per = 24MHz */
//
//#define CONFIG_USBCLK_SOURCE					USBCLK_SRC_PLL
///* #define CONFIG_OSC_RC32_CAL					48000000UL */
///* #define CONFIG_OSC_AUTOCAL_RC32MHZ_REF_OSC	OSC_ID_USBSOF */
///* #define CONFIG_SYSCLK_SOURCE					SYSCLK_SRC_RC32MHZ */
///* #define CONFIG_SYSCLK_PSADIV					SYSCLK_PSADIV_2 */
///* #define CONFIG_SYSCLK_PSBCDIV				SYSCLK_PSBCDIV_1_1 */
//
//#else
	//#error "Platform not defined or invalid!"
//#endif
//
//#endif
//
#endif /* CONF_CLOCK_H_INCLUDED */
