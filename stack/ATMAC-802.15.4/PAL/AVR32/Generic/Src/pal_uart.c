/**
 * @file pal_uart.c
 *
 * @brief PAL UART related functions
 *
 * This file implements AVR32 MCUs UART related transmission
 * and reception functions.
 *
 * $Id: pal_uart.c 28574 2011-09-15 07:43:50Z pawan.jadia $
 *
 * @author    Atmel Corporation: http://www.atmel.com
 * @author    Support email: avr@atmel.com
 */
/*
 * Copyright (c) 2010, Atmel Corporation All rights reserved.
 *
 * Licensed under Atmel's Limited License Agreement --> EULA.txt
 */

/* === Includes ============================================================= */

#include <stdint.h>
#include "pal_types.h"
#include "pal_config.h"
#include "pal_uart.h"
#include "pal_internal.h"
/* === Macros =============================================================== */

/* === Globals ============================================================== */

#if((defined UART0) ||(defined UART1) ||(defined UART2) || (defined UART3))
uart_communication_buffer_t usart_buffer;
#endif

/* === Prototypes =========================================================== */

/* === Implementation ======================================================= */

#if( (defined UART0) ||(defined UART1) ||(defined UART2) || (defined UART3))
/**
 * @brief Initializes UART
 *
 * This function initializes the UART.
 *
 * @param baud_rate Baud rate Register value corresponding baud rate
 */
void sio_usart_init(uint8_t baud_rate)
{
    USART.cr = AVR32_USART_RSTRX_MASK | AVR32_USART_RSTTX_MASK;
    avr32_usart_mr_t mr = {
        USART_CH_MODE  = AVR32_USART_CHMODE_NORMAL,
        USART_STOP_BIT = AVR32_USART_MR_NBSTOP_1,
        USART_PARITY   = AVR32_USART_MR_PAR_NONE,
        USART_CHR1     = AVR32_USART_CHRL_8,
        USART_MODE     = AVR32_USART_MODE_NORMAL,
        };

    USART.MR = mr;
    /* Set baud rate. */
    USART.brgr = BR_57600;
    USART.cr   =  AVR32_USART_CR_RXEN_MASK;
    USART.cr  |= AVR32_USART_CR_TXEN_MASK;
    /* Enable the receiver and transmitter and associated interrupts. */
    ENABLE_UART_RX_INT();
    /* To make Compiler Happy*/
    baud_rate = baud_rate;
}
#endif  /* UART0 | UART1 | UART2 | UART3 */


#if( (defined UART0) ||(defined UART1) ||(defined UART2) || (defined UART3))
/**
 * @brief Transmit data via UART
 *
 * This function transmits data via UART channel.
 *
 * @param data Pointer to the buffer where the data to be transmitted is present
 * @param length Number of bytes to be transmitted
 *
 * @return Number of bytes actually transmitted
 */
uint8_t sio_usart_tx(uint8_t *data, uint8_t length)
{
    uint8_t bytes_to_be_written;
    uint8_t head;
    uint8_t tail;
    uint8_t size;
    uint8_t back;

    /* The transmit interrupt is disabled. */
    DISABLE_UART_TX_INT();

    /*
     * Calculate available buffer space
     */
    head = usart_buffer.tx_buf_head;
    tail = usart_buffer.tx_buf_tail;

    if (tail >= head)
    {
        size = (UART_MAX_TX_BUF_LENGTH - 1) - (tail - head);
    }
    else
    {
        size = (head - 1) - tail;
    }

    if (size < length)
    {
        /* Not enough buffer space available. Use the remaining size. */
        bytes_to_be_written = size;
    }
    else
    {
        bytes_to_be_written = length;
    }

    /* Remember the number of bytes transmitted. */
    back = bytes_to_be_written;

    /* The data is copied to the transmit buffer. */
    while (bytes_to_be_written > 0)
    {
        usart_buffer.tx_buf[usart_buffer.tx_buf_tail] = *data;

        if ((UART_MAX_TX_BUF_LENGTH - 1) == usart_buffer.tx_buf_tail)
        {
            /* Reached the end of buffer, revert back to beginning of buffer. */
            usart_buffer.tx_buf_tail = 0;
        }
        else
        {
            /*
             * Increment the index to point the next character to be
             * inserted.
             */
            usart_buffer.tx_buf_tail++;
        }

        bytes_to_be_written--;
        data++;
    }

    /*
     * Check whether there is a transmission ongoing. Otherwise write
     * data into the UART data register. Transmission of subsequent
     * bytes / data will be taken care in the ISR.
     */
    if (usart_buffer.tx_count == 0)
    {
        USART.thr = usart_buffer.tx_buf[head];
        usart_buffer.tx_count = 1;
    }
    /* The transmit interrupt is enabled. */
    ENABLE_UART_TX_INT();

    return back;
}
#endif  /* UART0 | UART1 | UART2 | UART3 */

#if( (defined UART0) ||(defined UART1) ||(defined UART2) || (defined UART3))
/**
 * @brief Receives data from UART
 *
 * This function receives data from UART channel.
 *
 * @param data pointer to the buffer where the received data is to be stored
 * @param max_length maximum length of data to be received
 *
 * @return actual number of bytes received
 */
uint8_t sio_usart_rx(uint8_t *data, uint8_t max_length)
{
    uint8_t data_received = 0;

    if (usart_buffer.rx_count == 0)
    {
        /* UART receive buffer is empty. */
        return 0;
    }

    /* The receive interrupt is disabled. */
    DISABLE_UART_RX_INT();

    if (UART_MAX_RX_BUF_LENGTH <= usart_buffer.rx_count)
    {
        /*
         * Bytes between head and tail are overwritten by new data.
         * The oldest data in buffer is the one to which the tail is
         * pointing. So reading operation should start from the tail.
         */
        usart_buffer.rx_buf_head = usart_buffer.rx_buf_tail;

        /*
         * This is a buffer overflow case. But still only bytes equivalent to
         * full buffer size are useful.
         */
        usart_buffer.rx_count = UART_MAX_RX_BUF_LENGTH;

        /* Bytes received is more than or equal to buffer. */
        if (UART_MAX_RX_BUF_LENGTH <= max_length)
        {
            /*
             * Requested receive length (max_length) is more than the
             * max size of receive buffer, but at max the full
             * buffer can be read.
             */
            max_length = UART_MAX_RX_BUF_LENGTH;
        }
    }
    else
    {
        /* Bytes received is less than receive buffer maximum length. */
        if (max_length > usart_buffer.rx_count)
        {
            /*
             * Requested receive length (max_length) is more than the data
             * present in receive buffer. Hence only the number of bytes
             * present in receive buffer are read.
             */
            max_length = usart_buffer.rx_count;
        }
    }

    data_received = max_length;

    while (max_length > 0)
    {
        /* Start to copy from head. */
        *data = usart_buffer.rx_buf[usart_buffer.rx_buf_head];
        usart_buffer.rx_buf_head++;
        usart_buffer.rx_count--;
        data++;
        max_length--;
        if ((UART_MAX_TX_BUF_LENGTH) == usart_buffer.rx_buf_head)
        {
            usart_buffer.rx_buf_head = 0;
        }
    }

    /* The receive interrupt is enabled back. */
    ENABLE_UART_RX_INT();

    return data_received;
}
#endif  /* UART0 | UART1 | UART2 | UART3 */

#if( (defined UART0) ||(defined UART1) ||(defined UART2) || (defined UART3))
/**
 * @brief ISR for UART receive interrupt
 *
 * This service routine is executed when a byte is received successfully on
 * UART channel.
 */
#if (defined __GNUC__)
  __attribute__((__interrupt__))
#elif (defined __ICCAVR32__)
  __interrupt
#endif
void usart0_isr(void)
{
    uint32_t status = USART.csr;
    status &= USART.imr;
    if ((status & AVR32_USART_IMR_RXRDY_MASK) == AVR32_USART_IMR_RXRDY_MASK)
    {
        /* Count of bytes received through UART channel is incremented. */
        usart_buffer.rx_count++;

        //usart_buffer.rx_buf[usart_buffer.rx_buf_tail] = USART.rhr;
        USART_READ(usart_buffer.rx_buf[usart_buffer.rx_buf_tail]);

        if ((UART_MAX_RX_BUF_LENGTH - 1) == usart_buffer.rx_buf_tail)
        {
            /* Revert back to beginning of buffer after reaching end of buffer. */
            usart_buffer.rx_buf_tail = 0;
        }
        else
        {
            usart_buffer.rx_buf_tail++;
        }

    }
    if ((status & AVR32_USART_IMR_TXRDY_MASK) == AVR32_USART_IMR_TXRDY_MASK)
    {
        /*Uart Tx Interrupt is Disabled for Safety*/
        DISABLE_UART_TX_INT();

        if ((UART_MAX_TX_BUF_LENGTH - 1) == usart_buffer.tx_buf_head)
        {
            /* Reached the end of buffer, revert back to beginning of buffer. */
            usart_buffer.tx_buf_head = 0;
        }
        else
        {
            /* Increment the index to point the next character to be transmitted.*/
            usart_buffer.tx_buf_head++;
        }

        if (usart_buffer.tx_buf_head != usart_buffer.tx_buf_tail)
        {
            USART_WRITE(usart_buffer.tx_buf[usart_buffer.tx_buf_head]);
            /*Uart Tx Interrupt is Enabled for Further Transmission if any */
            ENABLE_UART_TX_INT();
        }
        else
        {
            /* No more data for transmission */
            usart_buffer.tx_count = 0;
        }
    }
}
#endif  /* UART0 | UART1 | UART2 | UART3 */
/* EOF */
