/**
 * @file pal_sio_hub.c
 *
 * @brief Stream I/O API functions
 *
 * This file implements the Stream I/O API functions.
 *
 * $Id: pal_sio_hub.c 19039 2009-11-03 09:27:02Z sschneid $
 *
 * @author    Atmel Corporation: http://www.atmel.com
 * @author    Support email: avr@atmel.com
 */
/*
 * Copyright (c) 2009, Atmel Corporation All rights reserved.
 *
 * Licensed under Atmel's Limited License Agreement --> EULA.txt
 */
/* === Includes ============================================================ */

#ifdef SIO_HUB

#include <stdint.h>
#include "pal.h"
#include "return_val.h"
#include "pal_uart.h"
#include "pal_usb.h"

/* === Globals =============================================================*/


/* === Prototypes ==========================================================*/


/* === Implementation ======================================================*/

/**
 * @brief Initializes the requested SIO unit
 *
 * This function initializes the requested SIO unit.
 *
 * @param sio_unit Specifies the SIO uint to be initialized
 *
 * @return MAC_SUCCESS  if SIO unit is initialized successfully, FAILURE
 * otherwise
 */
retval_t pal_sio_init(uint8_t sio_unit)
{
    retval_t status = MAC_SUCCESS;

    switch (sio_unit)
    {
#ifdef UART0
        case SIO_0:
    #ifdef BAUD_RATE_UART0
            sio_uart_0_init(BAUD_RATE_UART0);
    #else
            sio_uart_0_init(115200);
    #endif
            break;
#endif
#ifdef UART1
        case SIO_1:
    #ifdef BAUD_RATE_UART1
            sio_uart_1_init(BAUD_RATE_UART1);
    #else
            sio_uart_1_init(115200);
    #endif
            break;
#endif
#ifdef USB0
        case SIO_2:
            sio_usb_init();
            break;
#endif
        default:
            status = FAILURE;
            break;
    }

    return status;
}


/**
 * @brief Transmits data through selected SIO unit
 *
 * This function transmits data through the selected SIO unit.
 *
 * @param sio_unit Specifies the SIO unit
 * @param data Pointer to the data to be transmitted is present
 * @param length Number of bytes to be transmitted
 *
 * @return Actual number of transmitted bytes
 */
uint8_t pal_sio_tx(uint8_t sio_unit, uint8_t *data, uint8_t length)
{
    uint8_t number_of_bytes_transmitted;

    switch (sio_unit)
    {
#ifdef UART0
        case SIO_0: number_of_bytes_transmitted = sio_uart_0_tx(data, length);
                    break;
#endif
#ifdef UART1
        case SIO_1: number_of_bytes_transmitted = sio_uart_1_tx(data, length);
                    break;
#endif
#ifdef USB0
        case SIO_2: number_of_bytes_transmitted = sio_usb_tx(data, length);
                    break;
#endif
        default:    number_of_bytes_transmitted = 0;
                    break;
    }
    return (number_of_bytes_transmitted);
}


/**
 * @brief Receives data from selected SIO unit
 *
 * This function receives data from the selected SIO unit.
 *
 * @param sio_unit Specifies SIO unit
 * @param[out] data Pointer to the buffer to store received data
 * @param[in] max_length Maximum number of bytes to be received
 *
 * @return Actual number of received bytes
 */
uint8_t pal_sio_rx(uint8_t sio_unit, uint8_t *data, uint8_t max_length)
{
    uint8_t number_of_bytes_received;

    switch (sio_unit)
    {
#ifdef UART0
        case SIO_0: number_of_bytes_received = sio_uart_0_rx(data, max_length);
                    break;
#endif
#ifdef UART1
        case SIO_1: number_of_bytes_received = sio_uart_1_rx(data, max_length);
                    break;
#endif
#ifdef USB0
        case SIO_2: number_of_bytes_received = sio_usb_rx(data, max_length);
                    break;
#endif
        default:    number_of_bytes_received = 0;
                    break;
    }

    return (number_of_bytes_received);
}

uint8_t pal_sio_tx_empty(uint8_t sio_unit)
{
	switch(sio_unit)
    {
#ifdef UART0
		case SIO_0:
			return(sio_uart_0_tx_empty());
#endif
#ifdef UART1
		case SIO_1:
			return(sio_uart_1_tx_empty());
#endif
#ifdef USB0
		case SIO_2:
			return(sio_usb_tx_empty());
#endif
		default:
			return(0);
    }
}

void pal_sio_purge(uint8_t sio_unit)
{
	switch(sio_unit)
    {
#ifdef UART0
		case SIO_0:
			sio_uart_0_purge();
			break;
#endif
#ifdef UART1
		case SIO_1:
			sio_uart_1_purge();
			break;
#endif
#ifdef USB0
		case SIO_2:
			sio_usb_purge();
			break;
#endif
		default:
			return;
    }
}

uint8_t pal_sio_rx_length(uint8_t sio_unit)
{
	switch(sio_unit)
    {
#ifdef UART0
		case SIO_0:
			return(sio_uart_0_rx_length());
#endif
#ifdef UART1
		case SIO_1:
			return(sio_uart_1_rx_length());
#endif
#ifdef USB0
		case SIO_2:
			return(sio_usb_rx_length());
#endif
		default:
			return(0);
    }
}

#endif /* SIO_HUB */

/* EOF */
