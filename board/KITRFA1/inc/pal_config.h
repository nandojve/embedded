/**
 * @file pal_config.h
 *
 * @brief PAL configuration for ATmega128RFA1
 *
 * This header file contains configuration parameters for ATmega128RFA1.
 *
 * $Id: pal_config.h 22250 2010-06-16 13:51:39Z sschneid $
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
#ifndef PAL_CONFIG_H
#define PAL_CONFIG_H

/* === Includes =============================================================*/

#include "pal_boardtypes.h"

#if (BOARD_TYPE == KITRFA1)

/*
 * This header file is required since a function with
 * return type retval_t is declared
 */
#include "return_val.h"

/* === Types ================================================================*/

/* Enumerations used to identify LEDs */
typedef enum led_id_tag
{
    LED_0,
    LED_1,
    LED_2
} led_id_t;

#define NO_OF_LEDS                      (3)


/* Enumerations used to identify buttons */
typedef enum button_id_tag
{
    BUTTON_0
} button_id_t;

#define NO_OF_BUTTONS                   (1)

/* === Externals ============================================================*/


/* === Macros ===============================================================*/

/*
 * The default CPU clock
 *
 * The board must be configured for 16 MHz clock, which can be fused
 * for either the 16 MHz internal RC oscillator, or for the 16 MHz
 * crystal oscillator that is required for the transceiver operation.
 */
#define F_CPU                           (16000000UL)

/**
 * This board does not support antenna diversity.
 */
//#define ANTENNA_DIVERSITY               (0)

/*
 * Wait for 65 ns.
 * time t7: SLP_TR time (see data sheet or SWPM).
 * In case the system clock is slower than 32 MHz we do not need
 * to have this value filled.
 */
#define PAL_WAIT_65_NS()                // empty

/*
 * PORT where LEDs are connected
 */
#define LED_PORT                        (PORTG)
#define LED_PORT_DIR                    (DDRG)

/*
 * PINs where LEDs are connected
 */
#define LED_PIN_0                       (PG0)
#define LED_PIN_1                       (PG1)
#define LED_PIN_2                       (PG5)

/*
 * PORT where button is connected
 */
#define BUTTON_PORT                     (PORTG)
#define BUTTON_PORT_DIR                 (DDRG)
#define BUTTON_INPUT_PINS               (PING)

/*
 * PINs where buttons are connected
 */
#define BUTTON_PIN_0                    (PG2)

/*
 * UART0 and UART1 non-generic (board specific) initialization part.
 */
#define UART_0_INIT_NON_GENERIC()       // Empty macro
#define UART_1_INIT_NON_GENERIC()       // Empty macro


/*
 * This board does NOT have an external eeprom available.
 */
#ifndef EXTERN_EEPROM_AVAILABLE
#define EXTERN_EEPROM_AVAILABLE            (0)
#endif

/*
 * Alert initialization
 */
#define ALERT_INIT()                    do {    \
        LED_PORT = 0;                           \
        LED_PORT_DIR = 0xFF;                    \
} while (0)

/*
 * Alert indication
 * One loop cycle below is about 6 clock cycles (or more, depending
 * on optimization settings), so it takes 2...3 loop iterations per
 * microsecond.  60000 cycles thus results in 20...30 ms of delay,
 * so the LEDs should be seen as flickering.
 */
#define ALERT_INDICATE()                do {    \
        LED_PORT ^= 0xFF;                       \
} while (0)

/*
 * E2PROM Defines
 */
#define		EE_BOOTLOADER_OFFSET							(64)							// 64 - Bootloader offset
#define		EE_IEEE_ADDR									(EE_BOOTLOADER_OFFSET)			// 8  - IEEE address of board in EEPROM
#define		EE_RADIO_PAGE									(EE_IEEE_ADDR + 8)				// 1  - Radio channel page
#define		EE_RADIO_CHANNEL								(EE_RADIO_PAGE + 1)				// 1  - Radio virtual channel
#define		EE_RADIO_TXGAIN									(EE_RADIO_CHANNEL + 1)			// 1  - Radio TX gain
#define		EE_RADIO_RXGAIN									(EE_RADIO_TXGAIN + 1)			// 1  - Radio RX gain
#define		EE_RADIO_PANID									(EE_RADIO_RXGAIN + 1)			// 2  - Network Personal Area Network Identification
#define		EE_RADIO_SHORTSCAN								(EE_RADIO_PANID + 2)			// 1  - Network Short Scan Interval
#define		EE_RADIO_LONGSCAN								(EE_RADIO_SHORTSCAN + 1)		// 1  - Network Long Scan Interval
#define		EE_RADIO_VERBOSE								(EE_RADIO_LONGSCAN + 1)			// 1  - System Verbose
#define		EE_RADIO_SHORT_ADDR								(EE_RADIO_VERBOSE + 1)			// 2  - Network Short Address
#define		EE_RADIO_COORDINATOR							(EE_RADIO_SHORT_ADDR + 2)		// 1  - Network Device Coordinator
#define		EE_RADIO_BEACON_ORDER							(EE_RADIO_COORDINATOR + 1)		// 1  - Network Beacon Order
#define		EE_RADIO_SUPERFRAME_ORDER						(EE_RADIO_BEACON_ORDER + 1)		// 1  - Network Superframe Order
#define		EE_COMMUNICATION_TIMEOUT						(EE_RADIO_SUPERFRAME_ORDER + 1)	// 2  - Timeout for all communication interfaces
#define		EE_COMMUNICATION_PARENT							(EE_COMMUNICATION_TIMEOUT + 2)	// 2  - Network parent for Tree Route Service
#define		EE_COMMUNICATION_TYPE							(EE_COMMUNICATION_PARENT + 2)	// 1  - Communication interface type
#define		EE_USER_SPACE_OFFSET							(EE_COMMUNICATION_TYPE + 1)		//    - Start of EEPROM User Space

/* === Prototypes ===========================================================*/
#ifdef __cplusplus
extern "C" {
#endif

double		pal_read_temperature(void);

#ifdef __cplusplus
} /* extern "C" */
#endif


/*
 * MCU pins used for debugging
 */
#define TST_PIN_0_HIGH()
#define TST_PIN_0_LOW()
#define TST_PIN_1_HIGH()
#define TST_PIN_1_LOW()
#define TST_PIN_2_HIGH()
#define TST_PIN_2_LOW()
#define TST_PIN_3_HIGH()
#define TST_PIN_3_LOW()
#define TST_PIN_4_HIGH()
#define TST_PIN_4_LOW()
#define TST_PIN_5_HIGH()
#define TST_PIN_5_LOW()
#define TST_PIN_6_HIGH()
#define TST_PIN_6_LOW()
#define TST_PIN_7_HIGH()
#define TST_PIN_7_LOW()
#define TST_PIN_8_HIGH()
#define TST_PIN_8_LOW()

#endif /* KITRFA1 */

#endif  /* PAL_CONFIG_H */
/* EOF */
