/*
  pins_arduino.h - Pin definition functions for Arduino
  Part of Arduino - http://www.arduino.cc/

  Copyright (c) 2007 David A. Mellis

  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation; either
  version 2.1 of the License, or (at your option) any later version.

  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  Lesser General Public License for more details.

  You should have received a copy of the GNU Lesser General
  Public License along with this library; if not, write to the
  Free Software Foundation, Inc., 59 Temple Place, Suite 330,
  Boston, MA  02111-1307  USA

  $Id: wiring.h 249 2007-02-03 16:52:51Z mellis $
*/

/*
	This version of pins_arduino.h is for the WM-100-Duino
	Gerson Fernando Budke 2014 Abr 14
*/

#ifndef Pins_Arduino_h
#define Pins_Arduino_h

#include <avr/pgmspace.h>

// PORTLIST	Functions
// -------------------------------------------	
// PORT	   ID	1° Function				Digital	PWM		PCINT
// PD 2 ** 0 ** USART1_RX				D0		
// PD 3 ** 1 ** USART1_TX				D1
// PE 7 ** 2 **							D2
// PE 6 ** 3 **							D3
// PG 2 ** 4 **	AMR						D4
// PD 4 ** 5 **							D5
// PD 7 ** 6 **							D6
// PB 2 ** 7 **							D7				PCINT

// PB 6 ** 8 **							D8		PWM		PCINT
// PB 5 ** 9 **							D9		PWM		PCINT
// PB 0 ** 10 ** SPI_SS					D10				PCINT
// PB 2 ** 11 ** SPI_MOSI				D11				PCINT
// PB 3 ** 12 ** SPI_MISO				D12				PCINT
// PB 1 ** 13 ** SPI_SCK				D13				PCINT

// PF 0 ** 14 ** A0						D14
// PF 1 ** 15 ** A1						D15
// PF 5 ** 16 ** A5						D16
// PF 7 ** 17 ** A7						D17
// PF 4 ** 18 ** A4						D18
// PF 6 ** 19 ** A6						D19

// PD 0 ** 20 ** I2C_SCL				D20
// PD 1 ** 21 ** I2C_SDA				D21

// PE 3 ** 22 ** LED_0					D22		PWM
// PE 4 ** 23 ** LED_1					D23		PWM
// PE 5 ** 24 ** LED_2					D24		PWM

#define NUM_DIGITAL_PINS				25
#define NUM_ANALOG_INPUTS				6
#define analogInputToDigitalPin(p)		(((p) < 6) ? (p) + 14 : -1)
#define digitalPinHasPWM(p)				((p) == 8 || (p) == 9 || (p) == 23 || (p) == 24 || (p) == 25)

const static uint8_t SS					= 10;
const static uint8_t MOSI				= 11;
const static uint8_t MISO				= 12;
const static uint8_t SCK				= 13;

const static uint8_t A0					= 14;
const static uint8_t A1					= 15;
const static uint8_t A2					= 16;
const static uint8_t A3					= 17;
const static uint8_t A4					= 18;
const static uint8_t A5					= 19;

const static uint8_t SDA				= 20;
const static uint8_t SCL				= 21;
const static uint8_t LED0				= 22;
const static uint8_t LED1				= 23;
const static uint8_t LED2				= 24;

// A majority of the pins are NOT PCINTs, SO BE WARNED (i.e. you cannot use them as receive pins)
// Only pins available for RECEIVE (TRANSMIT can be on any pin):

#define digitalPinToPCICR(p)			( (((p) >= 7) && ((p) <= 13)) ? (&PCICR) : ((uint8_t *) 0) )
#define digitalPinToPCICRbit(p)			( 0 )
#define digitalPinToPCMSK(p)			( (((p) >= 7) && ((p) <= 13)) ? (&PCMSK0) : ((uint8_t *) 0) )
#define digitalPinToPCMSKbit(p)			( ((p) == 7) ? 2 : \
										( ((p) == 8) ? 6 : \
										( ((p) == 9) ? 5 : \
										( ((p) == 10) ? 0 : \
										( ((p) == 11) ? 2 : \
										( ((p) == 12) ? 3 : \
										( ((p) == 13) ? 1 : \
										0 ) ) ) ) ) ) ) 

#ifdef ARDUINO_MAIN

const uint16_t PROGMEM port_to_mode_PGM[] =
{
	NOT_A_PORT,
	NOT_A_PORT,
	(uint16_t)&DDRB,
	NOT_A_PORT,
	(uint16_t)&DDRD,
	(uint16_t)&DDRE,
	(uint16_t)&DDRF,
	(uint16_t)&DDRG,
	NOT_A_PORT,
	NOT_A_PORT,
	NOT_A_PORT,
	NOT_A_PORT,
	NOT_A_PORT,
};

const uint16_t PROGMEM port_to_output_PGM[] =
{
	NOT_A_PORT,
	NOT_A_PORT,
	(uint16_t)&PORTB,
	NOT_A_PORT,
	(uint16_t)&PORTD,
	(uint16_t)&PORTE,
	(uint16_t)&PORTF,
	(uint16_t)&PORTG,
	NOT_A_PORT,
	NOT_A_PORT,
	NOT_A_PORT,
	NOT_A_PORT,
	NOT_A_PORT,
};

const uint16_t PROGMEM port_to_input_PGM[] =
{
	NOT_A_PIN,
	NOT_A_PIN,
	(uint16_t)&PINB,
	NOT_A_PIN,
	(uint16_t)&PIND,
	(uint16_t)&PINE,
	(uint16_t)&PINF,
	(uint16_t)&PING,
	NOT_A_PIN,
	NOT_A_PIN,
	NOT_A_PIN,
	NOT_A_PIN,
	NOT_A_PIN,
};

const uint8_t PROGMEM digital_pin_to_port_PGM[] =
{
	// PORTLIST		
	// -------------------------------------------		
	PD	, // PD 2 ** 0 ** USART1_RX	
	PD	, // PD 3 ** 1 ** USART1_TX	
	PE	, // PE 7 ** 2 ** D2
	PE	, // PE 6 ** 3 ** D3
	PG	, // PG 2 ** 4 ** AMR
	PD	, // PD 4 ** 5 ** D5
	PD	, // PD 7 ** 6 ** D6
	PB	, // PB 2 ** 7 ** D7
	
	PB	, // PB 6 ** 8 ** D8
	PB	, // PB 5 ** 9 ** D9
	PB	, // PB 0 ** 10 ** SPI_SS
	PB	, // PB 2 ** 11 ** SPI_MOSI
	PB	, // PB 3 ** 12 ** SPI_MISO
	PB	, // PB 1 ** 13 ** SPI_SCK

	PF	, // PF 0 ** 14 ** A0
	PF	, // PF 1 ** 15 ** A1
	PF	, // PF 5 ** 16 ** A5
	PF	, // PF 7 ** 17 ** A7
	PF	, // PF 4 ** 18 ** A4
	PF	, // PF 6 ** 19 ** A6

	PD	, // PD 0 ** 20 ** I2C_SCL
	PD	, // PD 1 ** 21 ** I2C_SDA

	PE  , // PE 3 ** 22 ** LED_0
	PE  , // PE 4 ** 23 ** LED_1
	PE	, // PE 5 ** 24 ** LED_2
};

const uint8_t PROGMEM digital_pin_to_bit_mask_PGM[] =
{
	// PIN IN PORT		
	// -------------------------------------------		
	_BV( 2 )	, // PD 0 ** 0 ** USART1_RX	
	_BV( 3 )	, // PD 1 ** 1 ** USART1_TX	
	_BV( 7 )	, // PE 7 ** 2 ** 
	_BV( 6 )	, // PE 6 ** 3 ** 
	_BV( 2 )	, // PG 2 ** 4 ** AMR
	_BV( 4 )	, // PD 4 ** 5 ** 
	_BV( 7 )	, // PD 7 ** 6 ** 
	_BV( 2 )	, // PB 2 ** 7 ** 

	_BV( 6 )	, // PB 6 ** 8 ** 
	_BV( 5 )	, // PB 5 ** 9 ** 
	_BV( 0 )	, // PB 0 ** 10 ** SPI_SS
	_BV( 2 )	, // PB 2 ** 11 ** SPI_MOSI
	_BV( 3 )	, // PB 3 ** 12 ** SPI_MISO
	_BV( 1 )	, // PB 1 ** 13 ** SPI_SCK

	_BV( 0 )	, // PF 0 ** 14 ** A0
	_BV( 1 )	, // PF 1 ** 15 ** A1
	_BV( 5 )	, // PF 5 ** 16 ** A5
	_BV( 7 )	, // PF 7 ** 17 ** A7
	_BV( 4 )	, // PF 4 ** 18 ** A4
	_BV( 6 )	, // PF 6 ** 19 ** A6

	_BV( 0 )	, // PD 1 ** 20 ** I2C_SCL
	_BV( 1 )	, // PD 1 ** 21 ** I2C_SDA

	_BV( 3 )	, // PE 3 ** 22 ** LED_0
	_BV( 4 )  	, // PE 4 ** 23 ** LED_1
	_BV( 5 )  	, // PE 5 ** 24 ** LED_2
};

const uint8_t PROGMEM digital_pin_to_timer_PGM[] =
{
	// TIMERS		
	// -------------------------------------------	
	NOT_ON_TIMER	, // PD 2 ** 0 ** USART0_RX	
	NOT_ON_TIMER	, // PD 3 ** 1 ** USART0_TX	
	NOT_ON_TIMER	, // PE 7 ** 2 ** D2
	NOT_ON_TIMER	, // PE 6 ** 3 ** D3
	NOT_ON_TIMER	, // PG 2 ** 4 ** AMR
	NOT_ON_TIMER	, // PD 4 ** 5 ** D5
	NOT_ON_TIMER	, // PD 7 ** 6 ** D6
	NOT_ON_TIMER	, // PB 2 ** 7 ** D7

	TIMER1B			, // PB 6 ** 8 ** PWM8
	TIMER1A			, // PB 5 ** 9 ** PWM9
	NOT_ON_TIMER	, // PB 0 ** 10 ** SPI_SS
	NOT_ON_TIMER	, // PB 2 ** 11 ** SPI_MOSI
	NOT_ON_TIMER	, // PB 3 ** 12 ** SPI_MISO
	NOT_ON_TIMER	, // PB 1 ** 13 ** SPI_SCK

	NOT_ON_TIMER	, // PF 0 ** 14 ** A0
	NOT_ON_TIMER	, // PF 1 ** 15 ** A1
	NOT_ON_TIMER	, // PF 2 ** 16 ** A2
	NOT_ON_TIMER	, // PF 3 ** 17 ** A3
	NOT_ON_TIMER	, // PF 4 ** 18 ** A4
	NOT_ON_TIMER	, // PF 5 ** 19 ** A5

	NOT_ON_TIMER	, // PD 0 ** 20 ** I2C_SCL
	NOT_ON_TIMER	, // PD 1 ** 21 ** I2C_SDA	

	TIMER3A		  	, // PE 3 ** 22 ** LED_0 PWM
	TIMER3B		 	, // PE 4 ** 23 ** LED_1 PWM
	TIMER3C			, // PE 5 ** 24 ** LED_2 PWM
};	

#endif

#endif