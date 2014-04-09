/**
 * @file pal_uart.h
 *
 * @brief PAL uart internal functions prototypes
 *
 * This header contains the function prototypes for transmit,
 * receive functions and macros used in UART module.
 *
 * $Id: pal_uart.h 21966 2010-04-30 08:44:44Z uwalter $
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
#ifndef PAL_UART_H
#define PAL_UART_H

/* === Includes ============================================================= */

#include "app_config.h"
#include "pal_config.h"

/* === Types ================================================================ */

#if ((defined UART0) || (defined UART1) || (defined UART2))
#define USART_Baudrate_Set(_usart, _bselValue, _bScaleFactor)                  \
	(_usart)->BAUDCTRLA =(uint8_t)_bselValue;                                           \
	(_usart)->BAUDCTRLB =(_bScaleFactor << USART_BSCALE0_bp)|(_bselValue >> 8)

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

/* === Externals ============================================================ */


/* === Macros =============================================================== */

/*
 * These are the values to be placed in the UART control registers
 * corresponding to a system clok of 16MHz and the required baud rate.
 */
#define UART_BAUD(rate) \
    (((F_CPU) + 4UL * (rate)) / (8UL * (rate)) - 1UL)

/* Receive interrupt Mask */
#define RX_INT_MASK (0x80)

/* Transmit interrupt Mask */
#define TX_INT_MASK (0x40)

/* UART0 */
#ifdef UART0
/* Enables the RX interrupt of UART0 */
#define ENABLE_UART_0_RX_INT()   (UART0_REG.CTRLA |= USART_RXCINTLVL_gm)

/* Enables the TX interrupt of UART0 */
#define ENABLE_UART_0_TX_INT()   (UART0_REG.CTRLA |= USART_TXCINTLVL_gm)

/* Disables the RX interrupt of UART0 */
#define DISABLE_UART_0_RX_INT()  (UART0_REG.CTRLA &= ~USART_RXCINTLVL_gm)

/* Disables the TX interrupt of UART0 */
#define DISABLE_UART_0_TX_INT()  (UART0_REG.CTRLA &= ~USART_TXCINTLVL_gm)
#endif  /* UART0 */

/* UART1 */
#ifdef UART1
/* Enables the RX interrupt of UART1 */
#define ENABLE_UART_1_RX_INT()   (UART1_REG.CTRLA |= USART_RXCINTLVL_gm)

/* Enables the TX interrupt of UART1 */
#define ENABLE_UART_1_TX_INT()   (UART1_REG.CTRLA |= USART_TXCINTLVL_gm)

/* Disables the RX interrupt of UART1 */
#define DISABLE_UART_1_RX_INT()  (UART1_REG.CTRLA &= ~USART_RXCINTLVL_gm)

/* Disables the TX interrupt of UART1 */
#define DISABLE_UART_1_TX_INT()  (UART1_REG.CTRLA &= ~USART_TXCINTLVL_gm)
#endif  /* UART1 */

/* UART2 */
#ifdef UART2
/* Enables the RX interrupt of UART2 */
#define ENABLE_UART_2_RX_INT()   (UART2_REG.CTRLA |= USART_RXCINTLVL_gm)

/* Enables the TX interrupt of UART2 */
#define ENABLE_UART_2_TX_INT()   (UART2_REG.CTRLA |= USART_TXCINTLVL_gm)

/* Disables the RX interrupt of UART2 */
#define DISABLE_UART_2_RX_INT()  (UART2_REG.CTRLA &= ~USART_RXCINTLVL_gm)

/* Disables the TX interrupt of UART2 */
#define DISABLE_UART_2_TX_INT()  (UART2_REG.CTRLA &= ~USART_TXCINTLVL_gm)
#endif  /* UART2 */

/* === Prototypes =========================================================== */
#ifdef __cplusplus
extern "C" {
#endif

void sio_uart_0_init(uint32_t baud_rate);
void sio_uart_1_init(uint32_t baud_rate);
void sio_uart_2_init(uint32_t baud_rate);
uint8_t sio_uart_0_rx(uint8_t *data, uint8_t max_length);
uint8_t sio_uart_1_rx(uint8_t *data, uint8_t max_length);
uint8_t sio_uart_2_rx(uint8_t *data, uint8_t max_length);
uint8_t sio_uart_0_tx(uint8_t *data, uint8_t length);
uint8_t sio_uart_1_tx(uint8_t *data, uint8_t length);
uint8_t sio_uart_2_tx(uint8_t *data, uint8_t length);

uint8_t sio_uart_0_tx_empty(void);
uint8_t sio_uart_1_tx_empty(void);
uint8_t sio_uart_2_tx_empty(void);
void sio_uart_0_purge(void);
void sio_uart_1_purge(void);
void sio_uart_2_purge(void);

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /* #if ((defined UART0) || (defined UART1) || (defined UART2)) */

#endif /* PAL_UART_H */
/* EOF */
