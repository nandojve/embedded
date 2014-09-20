/**
 * \file
 *
 * \brief Main functions
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

#include <asf.h>
#include "conf_usb.h"

//// token pasting
//#define token_paste2_int(x, y) x ## y
//#define token_paste2(x, y) token_paste2_int(x, y)
//#define token_paste3_int(x, y, z) x ## y ## z
//#define token_paste3(x, y, z) token_paste3_int(x, y, z)
//
//// ENTER_PIN
//#define ENTER_PORT              PORTC
//#define ENTER_PIN               2
//#define ENTER_PIN_CTRL          token_paste3(ENTER_PORT.PIN, ENTER_PIN, CTRL)
//#define ENTER_PIN_STATE         0
//#define ENTER_PIN_PUEN          1

/*! \brief Main function. Execution starts here.
 */
int main(void)
{
	//void (*reset_vect)(void)	= 0x000000;
	//
	//ENTER_PORT.DIRCLR			= (1 << ENTER_PIN);
	//ENTER_PIN_CTRL				= 0x18;
	//volatile uint8_t	A		= ENTER_PORT.IN;// & (1 << ENTER_PIN);
	//volatile uint8_t	B		= (ENTER_PIN_STATE ? (1 << ENTER_PIN) : 0);
//
	//if(A == B)
	//{	
		// Map interrupt vectors table in bootloader section
		ccp_write_io((uint8_t*)&PMIC.CTRL, PMIC_IVSEL_bm | PMIC_LOLVLEN_bm | PMIC_MEDLVLEN_bm | PMIC_HILVLEN_bm);

		sysclk_init();
		cpu_irq_enable();

		// Start USB stack to authorize VBus monitoring
		udc_start();

		udc_attach();
		for(;;);
	}
//
	//// Jump into main code
	//EIND					= 0x00;
	//reset_vect();
}


/**
 * \mainpage ASF USB Device DFU
 *
 * \section intro Introduction
 * This application is the official Atmel USB DFU bootloader for XMEGA.
 *
 * A USB DFU bootloader guide is proposed for all AVR XMEGA parts with USB interface:
 *
 * \section files Application Files
 * The specific files of application are:
 * - main.c: the main file to start clock and USB DFU Device.
 * - specific linker script
 * - specific optimized codes to fit with boot section size
 */
