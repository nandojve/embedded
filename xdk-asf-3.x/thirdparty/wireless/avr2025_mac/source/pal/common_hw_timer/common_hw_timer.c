/**
 * @file common_hw_timer.c
 *
 * @brief 
 *
 
 *  Copyright (c) 2009 Atmel Corporation. All rights reserved.
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
 */
 
#include <compiler.h>
#include "sysclk.h"
#include "common_hw_timer.h"
#include "status_codes.h"

static uint8_t timer_multiplier;
static uint16_t compare_value;
static volatile uint8_t timer_mul_var;
static volatile uint32_t overflow_counter;

static tc_callback_t common_tc_ovf_callback;
static tc_callback_t common_tc_cca_callback;

/* === Prototypes =========================================================== */

static void tc_ovf_callback(void);
static void tc_cca_callback(void);


uint16_t countvalue;
uint16_t cv;
uint16_t common_tc_read_count(void)
{
	cv = 0;
	countvalue = tmr_read_count();

	if(timer_mul_var)
	{
		cv = ((UINT16_MAX / timer_multiplier) * timer_mul_var) + (countvalue / timer_multiplier);
	}
	else
	{
		cv = countvalue / timer_multiplier;
	}

	return cv;
}
void common_tc_compare_stop(void)
{
	tmr_disable_cc_interrupt();

	uint8_t flags = cpu_irq_save();
	overflow_counter = 0;
	compare_value = 0;
	cpu_irq_restore(flags);
}
void common_tc_overflow_stop(void)
{
	tmr_disable_ovf_interrupt();
	timer_mul_var = 0;
}
void common_tc_stop(void)
{
	common_tc_compare_stop();
	common_tc_overflow_stop();
	tmr_stop();
}
void common_tc_delay(uint16_t value)
{
	uint32_t temp;
	uint16_t countvalue = tmr_read_count();

	temp = ((uint32_t)value * timer_multiplier);

	overflow_counter = (temp + countvalue);

	overflow_counter = overflow_counter >> 16;

	if(overflow_counter)
	{
		compare_value = (uint16_t)((temp - (UINT16_MAX - countvalue)) - ((overflow_counter - 1) << 16));

		tmr_disable_cc_interrupt();
	}
	else
	{
		compare_value = temp + countvalue;

		tmr_enable_cc_interrupt();
	}
	if(compare_value < 100)
	{
		compare_value += 100;
	}
	tmr_write_cmpreg(compare_value);
}
void common_tc_init(void)
{
	timer_mul_var = 0;
	timer_multiplier = tmr_init();
}
static void tc_ovf_callback(void)
{
	if((overflow_counter > 0) && ((--overflow_counter) == 0))
	{
		tmr_enable_cc_interrupt();
	}
	if((++timer_mul_var) >= timer_multiplier)
	{
		timer_mul_var = 0;

		if (common_tc_ovf_callback) {
			common_tc_ovf_callback();
		}
	}
}
static void tc_cca_callback(void)
{
	tc_set_cca_interrupt_level(TIMER, TC_INT_LVL_OFF);

	if (common_tc_cca_callback) {
		common_tc_cca_callback();
	}
}
void set_common_tc_overflow_callback(tc_callback_t callback)
{
	common_tc_ovf_callback = callback;
}
void set_common_tc_expiry_callback(tc_callback_t callback)
{
	common_tc_cca_callback = callback;
}

/* EOF */
