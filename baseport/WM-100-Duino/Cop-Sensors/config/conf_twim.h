/**
 * \file
 *
 * \brief TWIM Configuration File for AVR XMEGA.
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
#ifndef _CONF_TWIM_H_
#define _CONF_TWIM_H_

#include <platform.h>

/** TWI Standard Mode */
#define TWI_STD_MODE_SPEED				( 100000 /* kbit/s */)
/** TWI Fast Mode */
#define TWI_FAST_MODE_SPEED				( 400000 /* kbit/s */)
/** TWI Fast Mode Plus */
#define TWI_FAST_MODE_PLUS_SPEED		(1000000 /* kbit/s */)
/** TWI High Speed Mode */
#define TWI_HIGH_SPEED_MODE_SPEED		(3400000 /* kbit/s */)

#define CONF_TWIM_PORT					&TWIC
#define CONF_TWIM_INTLVL				TWI_MASTER_INTLVL_MED_gc
#define CONF_PMIC_INTLVL				PMIC_MEDLVLEN_bm
#define CONF_TWIM_MASTER_SPEED			TWI_STD_MODE_SPEED

#endif // _CONF_TWIM_H_
