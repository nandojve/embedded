/**
 * @file pal_config.h
 *
 * @brief PAL configuration for ATmega256RFR2
 *
 * This header file contains configuration parameters for ATmega256RFR2.
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

#if (BOARD_TYPE == WM100)

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
 * This board uses Antenna Diversity.
 * The antenna diversity support can be overwritten by Makefile setting.
 */
#ifndef ANTENNA_DIVERSITY
#define ANTENNA_DIVERSITY               (1)
#endif

/**
 * This board uses Front End Module
 * The Front End Module support can be can be overwritten by Makefile setting.
 */

#define EXT_RF_FRONT_END_CTRL

/**
 * This board uses a custom power table
 */
#define CUSTOM_PWR_TABLE

/*
 * Value of an external LNA gain.
 * If no external LNA is available, the value is 0.
 */
#define EXT_LNA_HIGH_GAIN    (14)
#define EXT_LNA_LOW_GAIN     (-2)

/**
  * RSSI BASE VAL based on External LNA Gain.
  */
#define PAL_RSSI_BASE_VAL      (RSSI_BASE_VAL_DBM - EXT_LNA_HIGH_GAIN)

#define CUSTOM_DEFAULT_TX_PWR

#ifdef CUSTOM_DEFAULT_TX_PWR
/*
 * Default value of transmit power of transceiver: Preset
 *    - definition acct. IEEE802.15.4 PHY PIB attribute phyTransmitPower
 *    - TX Pout init value based on validation
 */
#define TAL_TRANSMIT_POWER_DEFAULT      (TX_PWR_TOLERANCE | 0x14)
#endif

/*
 * Default tx power for Ch26 to meet FCC compliance
  */
#define DEFAULT_TX_POWER_CH26             (0x80 | 0x0d)

/*
 * ED threshold level for CCA in subregister CCA_ED_THRES.
 * Standard value is 0x7, due to filter loss, the value is
 * decreased roughly 4dB. See datasheet for more details.
 * Enable the following line, if required CCA ED threshold differs
 * from transceiver's reset value.
 */
/* #define CCA_ED_THRESHOLD                (5) */

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
#define LED_PORT                        (PORTE)
#define LED_PORT_DIR                    (DDRE)

/*
 * PINs where LEDs are connected
 */
#define LED_PIN_0                       (PE3)
#define LED_PIN_1                       (PE4)
#define LED_PIN_2                       (PE5)

/*
 * PORT where button is connected
 */
#define BUTTON_PORT                     (PORTB)
#define BUTTON_PORT_DIR                 (DDRB)
#define BUTTON_INPUT_PINS               (PINB)

/*
 * PINs where buttons are connected
 */
#define BUTTON_PIN_0                    (PB6)

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

#endif /* WM100 */

#endif  /* PAL_CONFIG_H */
/* EOF */
