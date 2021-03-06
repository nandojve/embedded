/**
 * \file
 *
 * \brief ST7565R display controller driver example.
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

/**
 * \mainpage
 *
 * \section intro Introduction
 * This simple example shows how to use the \ref st7565r_lcd_controller_group
 * for interfacing the ST7565R controller
 *
 * \section files Main files:
 * - st7565r_example.c: the ST7565R low level driver example
 * - conf_st7565r.h: configuration of the ST7565R controller and interface
 * - conf_board.h: configuration for the board
 * - conf_clock.h: configuration for the clocks used in the device
 * - interface configuration files.
 *
 * \section apiinfo ST7565R low level driver API
 * The ST7565R low level driver API can be found
 * \ref st7565r_lcd_controller_group "here".
 *
 * \section deviceinfo Device Info
 * All devices with a SPI (USART SPI) can be used.
 * \note Parallel interface is also possible, but not currently supported by the
 * driver.
 *
 * \section exampledescription Description of the example
 * The display is initialized and a pattern with alternating lines are drawn to
 * the entire screen. After this the screen is scrolled line by line using the
 * hardware scrolling function available in the LCD controller.
 *
 * \section compinfo Compilation Info
 * This software was written for the GNU GCC and IAR for AVR.
 * Other compilers may or may not work.
 *
 * \section contactinfo Contact Information
 * For further information, visit
 * <A href="http://www.atmel.com/">Atmel</A>.\n
 */
#include <board.h>
#include <sysclk.h>
#include <st7565r.h>

int main(void)
{
	//! the page address to write to
	uint8_t page_address;
	//! the column address, or the X pixel.
	uint8_t column_address;
	//! store the LCD controller start draw line
	uint8_t start_line_address = 0;

	board_init();
	sysclk_init();

	// initialize the interface (SPI), ST7565R LCD controller and LCD
	st7565r_init();

	// set addresses at beginning of display
	st7565r_set_page_address(0);
	st7565r_set_column_address(0);

	// fill display with lines
	for (page_address = 0; page_address <= 4; page_address++) {
		st7565r_set_page_address(page_address);
		for (column_address = 0; column_address < 128; column_address++) {
			st7565r_set_column_address(column_address);
			/* fill every other pixel in the display. This will produce
			horizontal lines on the display. */
			st7565r_write_data(0xAA);
		}
	}

	// scroll the display using hardware support in the LCD controller
	while (true) {
		st7565r_set_display_start_line_address(start_line_address++);
		delay_ms(250);
	}
}
