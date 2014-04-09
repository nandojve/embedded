/**
 * @file sio_handler.h
 *
 * @brief This file contains the prototypes for UART related functions.
 *
 * $Id: sio_handler.h 22720 2010-08-02 12:48:19Z awachtle $
 *
 * @author    Atmel Corporation: http://www.atmel.com
 * @author    Support email: avr@atmel.com
 */
/*
 * Copyright (c) 2009, Atmel Corporation All rights reserved.
 *
 * Licensed under Atmel's Limited License Agreement --> EULA.txt
 */

/* Prevent double inclusion */
#ifndef SIO_HANDLER_H
#define SIO_HANDLER_H

/* === Includes ============================================================= */

#include <stdio.h>

/* === Macros =============================================================== */

#ifndef SIO_CHANNEL
	#error "The user must define SIO_CHANNEL=SIO_0, SIO_1, SIO_2, SIO_3, SIO_4, SIO_5. Note that SIO_2 is USB0."
#endif

#ifdef UART0
	#ifndef BAUD_RATE_UART0
		#error "The user must define BAUD_RATE_UART0=115200, for example."
	#endif
#endif

#ifdef UART1
	#ifndef BAUD_RATE_UART1
		#error "The user must define BAUD_RATE_UART1=115200, for example."
	#endif
#endif

#ifdef UART2
	#ifndef BAUD_RATE_UART2
		#error "The user must define BAUD_RATE_UART2=115200, for example."
	#endif
#endif

#ifdef UART3
	#ifndef BAUD_RATE_UART3
		#error "The user must define BAUD_RATE_UART3=115200, for example."
	#endif
#endif

#ifdef UART4
	#ifndef BAUD_RATE_UART4
		#error "The user must define BAUD_RATE_UART4=115200, for example."
	#endif
#endif

/* Function aliases allowing IAR and GCC functions use the same way */
#if ((defined __ICCAVR__) || (defined __ICCARM__))
#define sio_putchar(data)       _sio_putchar(data)
#define sio_getchar()           _sio_getchar()
#define sio_getchar_nowait()    _sio_getchar_nowait()
#define fdevopen(a,b)           /*tkr: iar does not use the fdevopen - the __write() (or __read()) must exist instread (here in file write.c)*/
#else
#define sio_putchar(data)       _sio_putchar(data, NULL)
#define sio_getchar()           _sio_getchar(NULL)
#define sio_getchar_nowait()    _sio_getchar_nowait(NULL)
#endif

/* === Types ================================================================ */


/* === Externals ============================================================ */


/* === Prototypes =========================================================== */

#ifdef __cplusplus
extern "C" {
#endif


#if ((defined __ICCAVR__) || (defined __ICCARM__))
int _sio_putchar(char data);
int _sio_getchar(void);
int _sio_getchar_nowait(void);
#else
int _sio_putchar(char data, FILE *dummy);
int _sio_getchar(FILE *dummy);
int _sio_getchar_nowait(FILE *dummy);
#endif


#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /* SIO_HANDLER_H */
/* EOF */
