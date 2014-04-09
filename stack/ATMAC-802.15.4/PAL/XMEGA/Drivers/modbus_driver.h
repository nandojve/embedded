/*
 * FreeModbus Libary: AVR Port
 * Copyright (C) 2011 Gerson Fernando Budke
 *   - Initial version + atxmega line support
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
 *
 */
#ifndef MODBUS_DRIVER_H
#define MODBUS_DRIVER_H

#include <avr/io.h>
#include <avr/interrupt.h>

#include "avr_compiler.h"
#include "modbus_driver_port.h"

/* ----------------------- Defines ------------------------------------------*/
#define	INLINE                      inline
#define PR_BEGIN_EXTERN_C           extern "C" {
#define	PR_END_EXTERN_C             }

#define ENTER_CRITICAL_SECTION()   AVR_ENTER_CRITICAL_REGION()
#define EXIT_CRITICAL_SECTION()    AVR_LEAVE_CRITICAL_REGION()

#define assert(x)

typedef char						BOOL;
typedef unsigned char				UCHAR;
typedef char						CHAR;
typedef unsigned short				USHORT;
typedef short						SHORT;
typedef unsigned long				ULONG;
typedef long						LONG;

#ifndef TRUE
	#define TRUE					1
#endif

#ifndef FALSE
	#define FALSE					0
#endif

/* ----------------------- RS485 specifics ----------------------------------*/
#ifdef RTS_ENABLE

#define RTS_HIGH					(MODBUS_RTS_PORT.OUTSET = _BV(MODBUS_RTS_PIN))
#define RTS_LOW						(MODBUS_RTS_PORT.OUTCLR = _BV(MODBUS_RTS_PIN))

#define RTS_INIT											\
	do {													\
		MODBUS_RTS_PORT.DDR			|= _BV(MODBUS_RTS_PIN);	\
        RTS_LOW;											\
    }while(0);

#endif // RTS_ENABLE

/* ----------------------- Defines ------------------------------------------*/

#define MB_TIMER_TICKS			(F_CPU / MODBUS_TIMER_PSC_VALUE)
#define MB_50US_TICKS			(20000UL)


#endif // MODBUS_DRIVER_H