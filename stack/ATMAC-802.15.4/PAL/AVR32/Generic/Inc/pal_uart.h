/**
 * @file pal_uart.h
 *
 * @brief PAL uart internal functions prototypes
 *
 * This header contains the function prototypes for transmit,
 * receive functions and macros used in UART module for AVR32 MCUs.
 *
 * $Id: pal_uart.h 28574 2011-09-15 07:43:50Z pawan.jadia $
 *
 * @author    Atmel Corporation: http://www.atmel.com
 * @author    Support email: avr@atmel.com
 */
/*
 * Copyright (c) 2010, Atmel Corporation All rights reserved.
 *
 * Licensed under Atmel's Limited License Agreement --> EULA.txt
 */

/* Prevent double inclusion */
#ifndef PAL_UART_H
#define PAL_UART_H

/* === Includes ============================================================= */

#include "app_config.h"

/* === Types ================================================================ */

#if( (defined UART0) ||(defined UART1) ||(defined UART2) || (defined UART3))

/*
 * Structure containing the transmit and receive buffer
 * and also the buffer head, tail and count
 */
typedef struct uart_communication_buffer_tag
{
    /* Transmit buffer */
    uint8_t tx_buf[UART_MAX_TX_BUF_LENGTH];

    /* Receive buffer */
    uint8_t rx_buf[UART_MAX_RX_BUF_LENGTH];

    /* Transmit buffer head */
    uint8_t tx_buf_head;

    /* Transmit buffer tail */
    uint8_t tx_buf_tail;

    /* Receive buffer head */
    uint8_t rx_buf_head;

    /* Receive buffer tail */
    uint8_t rx_buf_tail;

    /* Number of bytes in transmit buffer */
    uint8_t tx_count;

    /* Number of bytes in receive buffer */
    uint8_t rx_count;

} uart_communication_buffer_t;


/* UART different Baudrate definitions */
enum uart_br
{
    BR_1200      = SYS_F_UART / (16ul * (2ul-1ul) * 1200ul), // 1200 baud rate
    BR_2400      = SYS_F_UART / (16ul * (2ul-1ul) * 2400ul), // 2400 baud rate
    BR_4800      = SYS_F_UART / (16ul * (2ul-1ul) * 4800ul), // 4800 baud rate
    BR_9600      = SYS_F_UART / (16ul * (2ul-1ul) * 9600ul), // 9600 baud rate
    BR_19200     = SYS_F_UART / (16ul * (2ul-1ul) * 19200ul), // 19200 baud rate
    BR_38400     = SYS_F_UART / (16ul * (2ul-1ul) * 38400ul), // 38400 baud rate
    BR_57600     = SYS_F_UART / (16ul * (2ul-1ul) * 57600ul), // 57600 baud rate
    BR_115200    = SYS_F_UART / (16ul * (2ul-1ul) * 115200ul) // 115200 baud rate
};
/* === Externals ============================================================ */


/* === Macros =============================================================== */

/*
 * These are the values to be placed in the UART control registers
 * corresponding to a system clok of 16MHz and the required baud rate.
 */
#define UART_BAUD_1MHz_4k8     (25)   /* UPD:  0.2% */
#define UART_BAUD_1MHz_9k6     (12)   /* UPD:  0.2% */
#define UART_BAUD_4MHz_9k6     (51)   /* UPD:  0.2% */
#define UART_BAUD_8MHz_9k6     (103)  /* UPD:  0.2% */
#define UART_BAUD_8MHz_19k2    (51)   /* UPD:  0.2% */
#define UART_BAUD_16MHz_9k6    (207)  /* UPD:  0.2% */
#define UART_BAUD_16MHz_19k2   (103)  /* UPD:  0.2% */

/* Receive interrupt Mask */
#define RX_INT_MASK (0x80)

/* Transmit interrupt Mask */
#define TX_INT_MASK (0x40)

/* UART0 */

/* Enables the RX interrupt of UART0 */
#define ENABLE_UART_RX_INT()   (USART.ier |= AVR32_USART_IER_RXRDY_MASK)

/* Enables the TX interrupt of UART0 */
#define ENABLE_UART_TX_INT()   (USART.ier |= AVR32_USART_IER_TXRDY_MASK)

/* Disables the RX interrupt of UART0 */
#define DISABLE_UART_RX_INT()  (USART.idr |= AVR32_USART_IER_RXRDY_MASK)

/* Disables the TX interrupt of UART0 */
#define DISABLE_UART_TX_INT()  (USART.idr |= AVR32_USART_IER_TXRDY_MASK)


/* Writes data in the UART data register */
#define USART_WRITE(tx_data)   do {                                 \
    /*                                                              \
     * Data is written to tranmsit register only when               \
     * no ongoing data transmission.                                \
     */                                                             \
    while ((USART.csr & AVR32_USART_IMR_TXRDY_MASK) == 0);          \
    USART.thr = tx_data;                                            \
} while (0);


/* Writes data in the UART data register */
#define USART_READ(rx_data)   do {                                  \
    /*                                                              \
     * Reading from receive register till there                     \
     * is some data available.                                      \
     */                                                             \
    while ((USART.csr & AVR32_USART_IMR_RXRDY_MASK) == 0);          \
    rx_data = USART.rhr;                                            \
} while (0);


/* === Prototypes =========================================================== */
#ifdef __cplusplus
extern "C" {
#endif

/* To initialize the UART */
void sio_usart_init(uint8_t baud_rate);

/* To receive data from UART */
uint8_t sio_usart_rx(uint8_t *data, uint8_t max_length);

/* To transmit data from UART */
uint8_t sio_usart_tx(uint8_t *data, uint8_t length);

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /* #if ((defined UART0) || (defined UART1)) */

#endif /* PAL_UART_H */
/* EOF */
