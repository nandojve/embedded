/**
 * @file pal_uart.c
 *
 * @brief PAL UART related functions
 *
 * This file implements SAM4S UART related transmission functions
 * and reception functions.
 *
 * $Id: pal_uart.c 28470 2011-09-08 07:16:41Z mahendran.p $
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

#if ((defined UART0) || (defined UART1))
#include <stdint.h>
#include "pal.h"
#include "pal_config.h"
#include "pal_uart.h"
#include <sysclk.h>
#include <uart.h>

/* === Macros =============================================================== */
#define DISABLE_ALL_INTERRUPTS       (0xFFFFFFFF)

/* === Globals ============================================================== */
#if (defined UART0)
uart_communication_buffer_t uart_0_buffer;
#endif

#if (defined UART1)
uart_communication_buffer_t uart_1_buffer;
#endif

/* === Prototypes =========================================================== */

/* === Implementation ======================================================= */

#ifdef UART0
/**
 * @brief Initializes UART 0
 *
 * This function initializes the UART channel 0.
 *
 * @param baud_rate Baud rate Register value corresponding baud rate
 */
void sio_uart_0_init(uint32_t baud_rate)
{
	const sam_uart_opt_t		uart_console_settings		=
	{
		sysclk_get_peripheral_hz(),
		baud_rate,
		UART_MR_PAR_NO,
		UART_MR_CHMODE_NORMAL
	};

	/* Enable the peripheral clock in the PMC. */
	sysclk_enable_peripheral_clock(ID_UART0);

	/* Configure USART in serial mode. */
	uart_init(UART0, &uart_console_settings);

	/* Disable all the interrupts. */
	uart_disable_interrupt(UART0, DISABLE_ALL_INTERRUPTS);

	/* Configure and enable interrupt of USART. */
	NVIC_DisableIRQ(UART0_IRQn);
	NVIC_ClearPendingIRQ(UART0_IRQn);
	NVIC_EnableIRQ(UART0_IRQn);
}
#endif  /* UART0 */



#ifdef UART1
/**
 * @brief Initializes UART 1
 *
 * This function initializes the UART channel 1.
 *
 * @param baud_rate Baud rate Register value corresponding baud rate
 */

void sio_uart_1_init(uint32_t baud_rate)
{
	const sam_uart_opt_t		uart_console_settings		=
	{
		sysclk_get_peripheral_hz(),
		baud_rate,
		UART_MR_PAR_NO,
		UART_MR_CHMODE_NORMAL
	};

	/* Enable the peripheral clock in the PMC. */
	sysclk_enable_peripheral_clock(ID_UART1);

	/* Configure USART in serial mode. */
	uart_init(UART1, &uart_console_settings);

	/* Disable all the interrupts. */
	uart_disable_interrupt(UART1, DISABLE_ALL_INTERRUPTS);

	/* Configure and enable interrupt of USART. */
	NVIC_DisableIRQ(UART1_IRQn);
	NVIC_ClearPendingIRQ(UART1_IRQn);
	NVIC_EnableIRQ(UART1_IRQn);
}
#endif  /* UART1 */



#ifdef UART0
/**
 * @brief Transmit data via UART 0
 *
 * This function transmits data via UART channel 0.
 *
 * @param data Pointer to the buffer where the data to be transmitted is present
 * @param length Number of bytes to be transmitted
 *
 * @return Number of bytes actually transmitted
 */
uint8_t sio_uart_0_tx(uint8_t *data, uint8_t length)
{
    uint8_t bytes_to_be_written;
    uint8_t head;
    uint8_t tail;
    uint8_t size;
    uint8_t back;

    /* The transmit interrupt is disabled. */
    uart_disable_interrupt(UART0, UART_IDR_TXRDY);

    /*
     * Calculate available buffer space
     */
    head = uart_0_buffer.tx_buf_head;
    tail = uart_0_buffer.tx_buf_tail;

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
        uart_0_buffer.tx_buf[uart_0_buffer.tx_buf_tail] = *data;

        if ((UART_MAX_TX_BUF_LENGTH - 1) == uart_0_buffer.tx_buf_tail)
        {
            /* Reached the end of buffer, revert back to beginning of buffer. */
            uart_0_buffer.tx_buf_tail = 0;
        }
        else
        {
            /*
             * Increment the index to point the next character to be
             * inserted.
             */
            uart_0_buffer.tx_buf_tail++;
        }

        bytes_to_be_written--;
        data++;
    }

    /*
     * Check whether there is a transmission ongoing. Otherwise write
     * data into the uart data register. Transmission of subsequent
     * bytes / data will be taken care in the ISR.
     */
    if (uart_0_buffer.tx_count == 0)
    {
		while((UART0->UART_SR & UART_SR_TXEMPTY) == 0);
		UART0->UART_THR = uart_0_buffer.tx_buf[head];

        uart_0_buffer.tx_count = 1;
    }

    /* The transmit interrupt is enabled. */
    uart_enable_interrupt(UART0, UART_IER_TXRDY);

    return back;
}
#endif  /* UART0 */



#ifdef UART1
/**
 * @brief Transmit data via UART 1
 *
 * This function transmits data via UART channel 1.
 *
 * @param data Pointer to the buffer where the data to be transmitted is present
 * @param length Number of bytes to be transmitted
 *
 * @return Number of bytes actually transmitted
 */
uint8_t sio_uart_1_tx(uint8_t *data, uint8_t length)
{
    uint8_t bytes_to_be_written;
    uint8_t head;
    uint8_t tail;
    uint8_t size;
    uint8_t back;

    /* The transmit interrupt is disabled. */
    uart_disable_interrupt(UART1, UART_IDR_TXRDY);

    /*
     * Calculate available buffer space
     */
    head = uart_1_buffer.tx_buf_head;
    tail = uart_1_buffer.tx_buf_tail;

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
        uart_1_buffer.tx_buf[uart_1_buffer.tx_buf_tail] = *data;

        if ((UART_MAX_TX_BUF_LENGTH - 1) == uart_1_buffer.tx_buf_tail)
        {
            /* Reached the end of buffer, revert back to beginning of buffer. */
            uart_1_buffer.tx_buf_tail = 0;
        }
        else
        {
            /*
             * Increment the index to point the next character to be
             * inserted.
             */
            uart_1_buffer.tx_buf_tail++;
        }

        bytes_to_be_written--;
        data++;
    }

    /*
     * Check whether there is a transmission ongoing. Otherwise write
     * data into the UART data register. Transmission of subsequent
     * bytes / data will be taken care in the ISR.
     */
    if (uart_1_buffer.tx_count == 0)
    {
		while((UART1->UART_SR & UART_SR_TXEMPTY) == 0);
		UART1->UART_THR = uart_1_buffer.tx_buf[head];

        uart_1_buffer.tx_count = 1;
    }

    /* The transmit interrupt is enabled. */
    uart_enable_interrupt(UART1, UART_IER_TXRDY);

    return back;
}
#endif  /* UART1 */



#ifdef UART0
/**
 * @brief Receives data from UART 0
 *
 * This function receives data from UART channel 0.
 *
 * @param data pointer to the buffer where the received data is to be stored
 * @param max_length maximum length of data to be received
 *
 * @return actual number of bytes received
 */
uint8_t sio_uart_0_rx(uint8_t *data, uint8_t max_length)
{
    uint8_t data_received = 0;

    if (uart_0_buffer.rx_count == 0)
    {
        /* uart receive buffer is empty. */
        return 0;
    }

    /* The receive interrupt is disabled. */
    uart_disable_interrupt(UART0, UART_IDR_RXRDY);

    if (UART_MAX_RX_BUF_LENGTH <= uart_0_buffer.rx_count)
    {
        /*
         * Bytes between head and tail are overwritten by new data.
         * The oldest data in buffer is the one to which the tail is
         * pointing. So reading operation should start from the tail.
         */
        uart_0_buffer.rx_buf_head = uart_0_buffer.rx_buf_tail;

        /*
         * This is a buffer overflow case. But still only bytes equivalent to
         * full buffer size are useful.
         */
        uart_0_buffer.rx_count = UART_MAX_RX_BUF_LENGTH;

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
        if (max_length > uart_0_buffer.rx_count)
        {
            /*
             * Requested receive length (max_length) is more than the data
             * present in receive buffer. Hence only the number of bytes
             * present in receive buffer are read.
             */
            max_length = uart_0_buffer.rx_count;
        }
    }

    data_received = max_length;

    while (max_length > 0)
    {
        /* Start to copy from head. */
        *data = uart_0_buffer.rx_buf[uart_0_buffer.rx_buf_head];
        uart_0_buffer.rx_buf_head++;
        uart_0_buffer.rx_count--;
        data++;
        max_length--;
        if ((UART_MAX_RX_BUF_LENGTH) == uart_0_buffer.rx_buf_head)
        {
            uart_0_buffer.rx_buf_head = 0;
        }
    }

    /* The receive interrupt is enabled back. */
    uart_enable_interrupt(UART0, UART_IER_RXRDY);

    return data_received;
}
#endif  /* UART0 */



#ifdef UART1
/**
 * @brief Receives data from UART 1
 *
 * This function receives data from UART channel 1.
 *
 * @param data pointer to the buffer where the received data is to be stored
 * @param max_length maximum length of data to be received
 *
 * @return actual number of bytes received
 */
uint8_t sio_uart_1_rx(uint8_t *data, uint8_t max_length)
{
    uint8_t data_received = 0;

    if (uart_1_buffer.rx_count == 0)
    {
        /* uart receive buffer is empty. */
        return 0;
    }

    /* The receive interrupt is disabled. */
    uart_disable_interrupt(UART1, UART_IDR_RXRDY);

    if (UART_MAX_RX_BUF_LENGTH <= uart_1_buffer.rx_count)
    {
        /*
         * Bytes between head and tail are overwritten by new data.
         * The oldest data in buffer is the one to which the tail is
         * pointing. So reading operation should start from the tail.
         */
        uart_1_buffer.rx_buf_head = uart_1_buffer.rx_buf_tail;

        /*
         * This is a buffer overflow case. But still only bytes equivalent to
         * full buffer size are useful.
         */
        uart_1_buffer.rx_count = UART_MAX_RX_BUF_LENGTH;

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
        if (max_length > uart_1_buffer.rx_count)
        {
            /*
             * Requested receive length (max_length) is more than the data
             * present in receive buffer. Hence only the number of bytes
             * present in receive buffer are read.
             */
            max_length = uart_1_buffer.rx_count;
        }
    }

    data_received = max_length;

    while (max_length > 0)
    {
        /* Start to copy from head. */
        *data = uart_1_buffer.rx_buf[uart_1_buffer.rx_buf_head];
        uart_1_buffer.rx_buf_head++;
        uart_1_buffer.rx_count--;
        data++;
        max_length--;
        if ((UART_MAX_RX_BUF_LENGTH) == uart_1_buffer.rx_buf_head)
        {
            uart_1_buffer.rx_buf_head = 0;
        }
    }

    /* The receive interrupt is enabled back. */
    uart_enable_interrupt(UART1, UART_IER_RXRDY);

    return data_received;
}
#endif  /* UART1 */

#ifdef UART0
uint8_t sio_uart_0_rx_length(void)
{
	return(uart_0_buffer.rx_count);
}
#endif  /* UART0 */

#ifdef UART1
uint8_t sio_uart_1_rx_length(void)
{
	return(uart_1_buffer.rx_count);
}
#endif  /* UART1 */

#ifdef UART0
uint8_t sio_uart_0_tx_empty(void)
{
	if(uart_0_buffer.tx_buf_head != uart_0_buffer.tx_buf_tail)
	return(0);

	return(1);
}
#endif  /* UART0 */

#ifdef UART1
uint8_t sio_uart_1_tx_empty(void)
{
	if(uart_1_buffer.tx_buf_head != uart_1_buffer.tx_buf_tail)
	return(0);

	return(1);
}
#endif  /* UART1 */

#ifdef UART2
uint8_t sio_uart_2_tx_empty(void)
{
	if(uart_2_buffer.tx_buf_head != uart_2_buffer.tx_buf_tail)
	return(0);

	return(1);
}
#endif  /* UART2 */

#ifdef UART0
void sio_uart_0_purge(void)
{
	uart_0_buffer.rx_buf_head	= 0;
	uart_0_buffer.rx_buf_tail	= 0;
	uart_0_buffer.rx_count		= 0;

	uart_0_buffer.tx_buf_head	= 0;
	uart_0_buffer.tx_buf_tail	= 0;
	uart_0_buffer.tx_count		= 0;
}
#endif  /* UART0 */

#ifdef UART1
void sio_uart_1_purge(void)
{
	uart_1_buffer.rx_buf_head	= 0;
	uart_1_buffer.rx_buf_tail	= 0;
	uart_1_buffer.rx_count		= 0;

	uart_1_buffer.tx_buf_head	= 0;
	uart_1_buffer.tx_buf_tail	= 0;
	uart_1_buffer.tx_count		= 0;
}
#endif  /* UART1 */

#ifdef UART2
void sio_uart_2_purge(void)
{
	uart_2_buffer.rx_buf_head	= 0;
	uart_2_buffer.rx_buf_tail	= 0;
	uart_2_buffer.rx_count		= 0;

	uart_2_buffer.tx_buf_head	= 0;
	uart_2_buffer.tx_buf_tail	= 0;
	uart_2_buffer.tx_count		= 0;
}
#endif  /* UART2 */

#ifdef UART0
/**
 * @brief ISR for UART0 transmission and reception interrupts
 *
 * This service routine is executed when a byte is successfully transmitted or
 * received on UART0.
 */

ISR(UART0_Handler)
{
    uint32_t uart_irq_cause;

    /*
     * The interrupt is acknowledged by reading the status register. This
     * clears the interrupt at the UART level.
     */
    uart_irq_cause = uart_get_status(UART0);

    if (UART_SR_RXRDY & uart_irq_cause)
    {
        uint8_t tail = uart_0_buffer.rx_buf_tail;

        /* Count of bytes received through UART0 channel is incremented. */
        uart_0_buffer.rx_count++;

		uart_0_buffer.rx_buf[tail] = (uint8_t) (UART0->UART_RHR & UART_RHR_RXCHR_Msk);

        if ((UART_MAX_RX_BUF_LENGTH - 1) == uart_0_buffer.rx_buf_tail)
        {
            /*
             * Revert back to beginning of buffer after reaching end of buffer.
             */
            uart_0_buffer.rx_buf_tail = 0;
        }
        else
        {
            uart_0_buffer.rx_buf_tail++;
        }
    }
    else if (UART_SR_TXRDY & uart_irq_cause)
    {
        /*
         * UART0 transmit interrupt is disabled.
         * If not disabled, an immediate interrupt after the update of UDR0
         * prevents the execution of remaining instructions.
         */
        uart_disable_interrupt(UART0, UART_IDR_TXRDY);

        if ((UART_MAX_TX_BUF_LENGTH - 1) == uart_0_buffer.tx_buf_head)
        {
            /* Reached the end of buffer, revert back to beginning of buffer. */
            uart_0_buffer.tx_buf_head = 0;
        }
        else
        {
            /*
             * Increment the index to point the next character to be
             * transmitted.
             */
            uart_0_buffer.tx_buf_head++;
        }

        if (uart_0_buffer.tx_buf_head != uart_0_buffer.tx_buf_tail)
        {
			while((UART0->UART_SR & UART_SR_TXEMPTY) == 0);
			UART0->UART_THR = uart_0_buffer.tx_buf[uart_0_buffer.tx_buf_head];

            /*
             * Some more bytes are to be transmitted, hence the transmit
             * interrupt is enabled back.
             */
            uart_enable_interrupt(UART0, UART_IER_TXRDY);
        }
        else
        {
            /* No more data for transmission */
            uart_0_buffer.tx_count = 0;
        }
    }
}
#endif  /* UART0 */


#ifdef UART1
/**
 * @brief ISR for UART1 transmission and reception interrupts
 *
 * This service routine is executed when a byte is successfully transmitted or
 * received on UART1.
 */
ISR(UART1_Handler)
{
    uint32_t uart_irq_cause;

    /*
     * The interrupt is acknowledged by reading the status register. This
     * clears the interrupt at the UART level.
     */
    uart_irq_cause = uart_get_status(UART1);

    if (UART_SR_RXRDY & uart_irq_cause)
    {
        uint8_t tail = uart_1_buffer.rx_buf_tail;

        /* Count of bytes received through UART1 channel is incremented. */
        uart_1_buffer.rx_count++;

        uart_1_buffer.rx_buf[tail] = (uint8_t) (UART1->UART_RHR & UART_RHR_RXCHR_Msk);

        if ((UART_MAX_RX_BUF_LENGTH - 1) == uart_1_buffer.rx_buf_tail)
        {
            /*
             * Revert back to beginning of buffer after reaching end of buffer.
             */
            uart_1_buffer.rx_buf_tail = 0;
        }
        else
        {
            uart_1_buffer.rx_buf_tail++;
        }
    }
    else if (UART_SR_TXRDY & uart_irq_cause)
    {
        /*
         * UART1 transmit interrupt is disabled.
         * If not disabled, an immidiate interrupt after the update of UDR0
         * prevents the execution of remaining instructions.
         */
        uart_disable_interrupt(UART1, UART_IDR_TXRDY);

        if ((UART_MAX_TX_BUF_LENGTH - 1) == uart_1_buffer.tx_buf_head)
        {
            /* Reached the end of buffer, revert back to beginning of buffer. */
            uart_1_buffer.tx_buf_head = 0;
        }
        else
        {
            /*
             * Increment the index to point the next character to be
             * transmitted.
             */
            uart_1_buffer.tx_buf_head++;
        }

        if (uart_1_buffer.tx_buf_head != uart_1_buffer.tx_buf_tail)
        {
			while((UART1->UART_SR & UART_SR_TXEMPTY) == 0);
			UART1->UART_THR = uart_1_buffer.tx_buf[uart_1_buffer.tx_buf_head];

            /*
             * Some more bytes are to be transmitted, hence the transmit
             * interrupt is enabled back.
             */
            uart_enable_interrupt(UART1, UART_IER_TXRDY);
        }
        else
        {
            /* No more data for transmission */
            uart_1_buffer.tx_count = 0;
        }
    }
}
#endif  /* UART1 */

#endif  /* ((defined UART0) || (defined UART1)) */

/* EOF */
