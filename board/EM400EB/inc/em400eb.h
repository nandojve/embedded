/**
 * \file
 *
 * \brief EM400EB Board Definition.
 *
 * Copyright (c) 2014 EFICID Technology. All rights reserved.
 *
 * \asf_license_start
 *
 * \page License
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice,
 *    this list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 *
 * 3. The name of Eficid may not be used to endorse or promote products derived
 *    from this software without specific prior written permission.
 *
 * 4. This software may only be redistributed and used in connection with an
 *    Eficid module product.
 *
 * THIS SOFTWARE IS PROVIDED BY EFICID "AS IS" AND ANY EXPRESS OR IMPLIED
 * WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NON-INFRINGEMENT ARE
 * EXPRESSLY AND SPECIFICALLY DISCLAIMED. IN NO EVENT SHALL EFICID BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
 * STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN
 * ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 *
 * \asf_license_stop
 *
 */

#ifndef _EM400EB_H_
#define _EM400EB_H_

#include <conf_board.h>

/*----------------------------------------------------------------------------*/
/**
 *  \page em400eb_opfreq "EM400EB - Operating frequencies"
 *  This page lists several definition related to the board operating frequency
 *
 *  \section Definitions
 *  - \ref BOARD_FREQ_*
 *  - \ref BOARD_MCK
 */

/** Board oscillator settings */
#define BOARD_FREQ_SLCK_XTAL            (32768U)
#define BOARD_FREQ_SLCK_BYPASS          (32768U)
#define BOARD_FREQ_MAINCK_XTAL          (12000000U)
#define BOARD_FREQ_MAINCK_BYPASS        (12000000U)

/** Master clock frequency */
#define BOARD_MCK                       CHIP_FREQ_CPU_MAX

/** board main clock xtal startup time */
#define BOARD_OSC_STARTUP_US            15625

/*----------------------------------------------------------------------------*/
/**
 * \page em400eb_board_info "EM400EB - Board informations"
 * This page lists several definition related to the board description.
 *
 * \section Definitions
 * - \ref BOARD_NAME
 */

/** Name of the board */
#define BOARD_NAME "EM400EB"
/** Family definition (already defined) */
#define sam4e
/** Core definition */
#define cortexm4

/*----------------------------------------------------------------------------*/

/** UART0 pins (UTXD0 and URXD0) definitions, PA10,9. */
#define PINS_UART0        (PIO_PA9A_URXD0 | PIO_PA10A_UTXD0)
#define PINS_UART0_FLAGS  (IOPORT_MODE_MUX_A)

#define PINS_UART0_PORT   IOPORT_PIOA
#define PINS_UART0_MASK   (PIO_PA9A_URXD0 | PIO_PA10A_UTXD0)
#define PINS_UART0_PIO    PIOA
#define PINS_UART0_ID     ID_PIOA
#define PINS_UART0_TYPE   PIO_PERIPH_A
#define PINS_UART0_ATTR   PIO_DEFAULT

/** UART1 pins (UTXD1 and URXD1) definitions, PA21,22. */
#define PINS_UART1        (PIO_PA21A_RXD1 | PIO_PA22A_TXD1)
#define PINS_UART1_FLAGS  (IOPORT_MODE_MUX_A)

#define PINS_UART1_PORT   IOPORT_PIOA
#define PINS_UART1_MASK   (PIO_PA21A_RXD1 | PIO_PA22A_TXD1)
#define PINS_UART1_PIO    PIOA
#define PINS_UART1_ID     ID_PIOA
#define PINS_UART1_TYPE   PIO_PERIPH_A
#define PINS_UART1_ATTR   PIO_DEFAULT

/** LED #0 pin definition (Blue). */
#define LED_0_NAME      "Red LED"
#define PIN_LED_0       {PIO_PA19, PIOA, ID_PIOA, PIO_OUTPUT_1, PIO_DEFAULT}
#define PIN_LED_0_MASK  PIO_PA19
#define PIN_LED_0_PIO   PIOA
#define PIN_LED_0_ID    ID_PIOA
#define PIN_LED_0_TYPE  PIO_OUTPUT_1
#define PIN_LED_0_ATTR  PIO_DEFAULT

#define LED0_GPIO            (PIO_PA19_IDX)
#define LED0_FLAGS           (0)
#define LED0_ACTIVE_LEVEL    IOPORT_PIN_LEVEL_HIGH
#define LED0_INACTIVE_LEVEL  IOPORT_PIN_LEVEL_LOW

/** LED #1 pin definition (Amber). */
#define LED_1_NAME      "Green LED"
#define PIN_LED_1       {PIO_PA20, PIOD, ID_PIOD, PIO_OUTPUT_1, PIO_DEFAULT}
#define PIN_LED_1_MASK  PIO_PA20
#define PIN_LED_1_PIO   PIOA
#define PIN_LED_1_ID    ID_PIOA
#define PIN_LED_1_TYPE  PIO_OUTPUT_1
#define PIN_LED_1_ATTR  PIO_DEFAULT

#define LED1_GPIO            (PIO_PA20_IDX)
#define LED1_FLAGS           (0)
#define LED1_ACTIVE_LEVEL    IOPORT_PIN_LEVEL_HIGH
#define LED1_INACTIVE_LEVEL  IOPORT_PIN_LEVEL_LOW

#define BOARD_NUM_OF_LED 2

/** HSMCI pins definition. */
/*! Number of slot connected on HSMCI interface */
#define SD_MMC_HSMCI_MEM_CNT      1
#define SD_MMC_HSMCI_SLOT_0_SIZE  4
#define PINS_HSMCI						{0x3fUL << 26, PIOA, ID_PIOA, PIO_PERIPH_C, PIO_PULLUP}
/** HSMCI MCCDA pin definition. */
#define PIN_HSMCI_MCCDA_GPIO            (PIO_PA28_IDX)
#define PIN_HSMCI_MCCDA_FLAGS           (IOPORT_MODE_MUX_C)
/** HSMCI MCCK pin definition. */
#define PIN_HSMCI_MCCK_GPIO             (PIO_PA29_IDX)
#define PIN_HSMCI_MCCK_FLAGS            (IOPORT_MODE_MUX_C)
/** HSMCI MCDA0 pin definition. */
#define PIN_HSMCI_MCDA0_GPIO            (PIO_PA30_IDX)
#define PIN_HSMCI_MCDA0_FLAGS           (IOPORT_MODE_MUX_C)
/** HSMCI MCDA1 pin definition. */
#define PIN_HSMCI_MCDA1_GPIO            (PIO_PA31_IDX)
#define PIN_HSMCI_MCDA1_FLAGS           (IOPORT_MODE_MUX_C)
/** HSMCI MCDA2 pin definition. */
#define PIN_HSMCI_MCDA2_GPIO            (PIO_PA26_IDX)
#define PIN_HSMCI_MCDA2_FLAGS           (IOPORT_MODE_MUX_C)
/** HSMCI MCDA3 pin definition. */
#define PIN_HSMCI_MCDA3_GPIO            (PIO_PA27_IDX)
#define PIN_HSMCI_MCDA3_FLAGS           (IOPORT_MODE_MUX_C)

/** SD/MMC card detect pin definition. */
#define PIN_HSMCI_CD					{PIO_PD25, PIOD, ID_PIOD, PIO_INPUT, PIO_PULLUP}
#define SD_MMC_0_CD_GPIO				(PIO_PD25_IDX)
#define SD_MMC_0_CD_PIO_ID				ID_PIOD
#define SD_MMC_0_CD_FLAGS				(IOPORT_MODE_PULLUP)
#define SD_MMC_0_CD_DETECT_VALUE		0

/** SD/MMC VBUS enable pin definition. */
#define PIN_HSMCI_VBUS					{PIO_PD27, PIOD, ID_PIOD, PIO_OUTPUT_1, PIO_DEFAULT}
#define SD_MMC_0_VBUS_GPIO				(PIO_PD27_IDX)
#define SD_MMC_0_VBUS_PIO_ID			ID_PIOD
#define SD_MMC_0_VBUS_FLAGS				(0)
#define SD_MMC_0_VBUS_ENABLE_LEVEL		IOPORT_PIN_LEVEL_LOW


#define PIN_TC0_TIOA0        (PIO_PA0_IDX)
#define PIN_TC0_TIOA0_MUX    (IOPORT_MODE_MUX_B)
#define PIN_TC0_TIOA0_FLAGS  (IOPORT_MODE_MUX_B)

#define PIN_TC0_TIOA1        (PIO_PA15_IDX)
#define PIN_TC0_TIOA1_MUX    (IOPORT_MODE_MUX_B)
#define PIN_TC0_TIOA1_FLAGS  (IOPORT_MODE_MUX_B)

#define PIN_TC0_TIOA1_PIO    PIOA
#define PIN_TC0_TIOA1_MASK   PIO_PA15
#define PIN_TC0_TIOA1_ID     ID_PIOA
#define PIN_TC0_TIOA1_TYPE   PIO_PERIPH_B
#define PIN_TC0_TIOA1_ATTR   PIO_DEFAULT

#define PIN_TC0_TIOA2        (PIO_PA26_IDX)
#define PIN_TC0_TIOA2_MUX    (IOPORT_MODE_MUX_B)
#define PIN_TC0_TIOA2_FLAGS  (IOPORT_MODE_MUX_B)

#define PIN_TC0_TIOA2_PIO    PIOA
#define PIN_TC0_TIOA2_MASK   PIO_PA26
#define PIN_TC0_TIOA2_ID     ID_PIOA
#define PIN_TC0_TIOA2_TYPE   PIO_PERIPH_B
#define PIN_TC0_TIOA2_ATTR   PIO_DEFAULT

/** PWM LED0 pin definitions. */
#define PIN_PWM_LED0_GPIO     PIO_PA19_IDX
#define PIN_PWM_LED0_FLAGS    (IOPORT_MODE_MUX_A)
#define PIN_PWM_LED0_CHANNEL  PWM_CHANNEL_0

/** PWM LED1 pin definitions. */
#define PIN_PWM_LED1_GPIO     PIO_PA20_IDX
#define PIN_PWM_LED1_FLAGS    (IOPORT_MODE_MUX_A)
#define PIN_PWM_LED1_CHANNEL  PWM_CHANNEL_1

/** SPI MISO pin definition. */
#define SPI_MISO_GPIO         (PIO_PA12_IDX)
#define SPI_MISO_FLAGS        (IOPORT_MODE_MUX_A)
/** SPI MOSI pin definition. */
#define SPI_MOSI_GPIO         (PIO_PA13_IDX)
#define SPI_MOSI_FLAGS        (IOPORT_MODE_MUX_A)
/** SPI SPCK pin definition. */
#define SPI_SPCK_GPIO         (PIO_PA14_IDX)
#define SPI_SPCK_FLAGS        (IOPORT_MODE_MUX_A)

/** SPI chip select 0 pin definition. (Only one configuration is possible) */
#define SPI_NPCS0_GPIO        (PIO_PA11_IDX)
#define SPI_NPCS0_FLAGS       (IOPORT_MODE_MUX_A)
/** SPI chip select 1 pin definition. (multiple configurations are possible) */
#define SPI_NPCS1_GPIO        (PIO_PD18_IDX)
#define SPI_NPCS1_FLAGS       (IOPORT_MODE_MUX_A)
/** SPI chip select 3 pin definition. (multiple configurations are possible) */
#define SPI_NPCS3_GPIO        (PIO_PD19_IDX)
#define SPI_NPCS3_FLAGS       (IOPORT_MODE_MUX_A)

/** TWI0 pins definition */
#define TWI0_DATA_GPIO   PIO_PA3_IDX
#define TWI0_DATA_FLAGS  (IOPORT_MODE_MUX_A)
#define TWI0_CLK_GPIO    PIO_PA4_IDX
#define TWI0_CLK_FLAGS   (IOPORT_MODE_MUX_A)

/** USART0 pin RX */
#define PIN_USART0_RXD        {PIO_PB0C_RXD0, PIOB, ID_PIOB, PIO_PERIPH_C, PIO_DEFAULT}
#define PIN_USART0_RXD_IDX    (PIO_PB0_IDX)
#define PIN_USART0_RXD_FLAGS  (IOPORT_MODE_MUX_C)
/** USART0 pin TX */
#define PIN_USART0_TXD        {PIO_PB1C_TXD0, PIOB, ID_PIOB, PIO_PERIPH_C, PIO_DEFAULT}
#define PIN_USART0_TXD_IDX    (PIO_PB1_IDX)
#define PIN_USART0_TXD_FLAGS  (IOPORT_MODE_MUX_C)
/** USART0 pin CTS */
#define PIN_USART0_CTS        {PIO_PB2C_CTS0, PIOB, ID_PIOB, PIO_PERIPH_C, PIO_DEFAULT}
#define PIN_USART0_CTS_IDX    (PIO_PB2_IDX)
#define PIN_USART0_CTS_FLAGS  (IOPORT_MODE_MUX_C)
/** USART0 pin RTS */
#define PIN_USART0_RTS        {PIO_PB3C_RTS0, PIOB, ID_PIOB, PIO_PERIPH_C, PIO_DEFAULT}
#define PIN_USART0_RTS_IDX    (PIO_PB3_IDX)
#define PIN_USART0_RTS_FLAGS  (IOPORT_MODE_MUX_C)

/** USART1 pin RX */
#define PIN_USART1_RXD        {PIO_PA21A_RXD1, PIOA, ID_PIOA, PIO_PERIPH_A, PIO_DEFAULT}
#define PIN_USART1_RXD_IDX    (PIO_PA21_IDX)
#define PIN_USART1_RXD_FLAGS  (IOPORT_MODE_MUX_A)
/** USART1 pin TX */
#define PIN_USART1_TXD        {PIO_PA22A_TXD1, PIOA, ID_PIOA, PIO_PERIPH_A, PIO_DEFAULT}
#define PIN_USART1_TXD_IDX    (PIO_PA22_IDX)
#define PIN_USART1_TXD_FLAGS  (IOPORT_MODE_MUX_A)
/** USART1 pin CTS */
#define PIN_USART1_CTS        {PIO_PA25A_CTS1, PIOA, ID_PIOA, PIO_PERIPH_A, PIO_DEFAULT}
#define PIN_USART1_CTS_IDX    (PIO_PA25_IDX)
#define PIN_USART1_CTS_FLAGS  (IOPORT_MODE_MUX_A)
/** USART1 pin RTS */
#define PIN_USART1_RTS        {PIO_PA24A_RTS1, PIOA, ID_PIOA, PIO_PERIPH_A, PIO_DEFAULT}
#define PIN_USART1_RTS_IDX    (PIO_PA24_IDX)
#define PIN_USART1_RTS_FLAGS  (IOPORT_MODE_MUX_A)
/** USART1 pin SCK */
#define PIN_USART1_SCK        {PIO_PA23A_SCK1, PIOA, ID_PIOA, PIO_PERIPH_A, PIO_DEFAULT}
#define PIN_USART1_SCK_IDX    (PIO_PA23_IDX)
#define PIN_USART1_SCK_FLAGS  (IOPORT_MODE_MUX_A)

/** USB VBus monitoring pin definition. */
#define PIN_USB_VBUS          {PIO_PD31, PIOD, ID_PIOD, PIO_INPUT, PIO_PULLUP}
#define USB_VBUS_FLAGS        (PIO_INPUT | PIO_DEBOUNCE | PIO_IT_EDGE)
#define USB_VBUS_PIN_IRQn     (PIOC_IRQn)
#define USB_VBUS_PIN          (PIO_PD31_IDX)
#define USB_VBUS_PIO_ID       (ID_PIOD)
#define USB_VBUS_PIO_MASK     (PIO_PD31)
/* This pin can not be used as fast wakeup source such as
 * USB_VBUS_WKUP PMC_FSMR_FSTT7 */

/** USB D- pin (System function) */
#define PIN_USB_DM            {PIO_PB10}
/** USB D+ pin (System function) */
#define PIN_USB_DP            {PIO_PB11}

/* KSZ8051MNL relate PIN definition */
#define PIN_KSZ8051MNL_RXC_IDX                PIO_PD14_IDX
#define PIN_KSZ8051MNL_RXC_FLAGS             (IOPORT_MODE_MUX_A)
#define PIN_KSZ8051MNL_TXC_IDX                PIO_PD0_IDX
#define PIN_KSZ8051MNL_TXC_FLAGS             (IOPORT_MODE_MUX_A)
#define PIN_KSZ8051MNL_TXEN_IDX               PIO_PD1_IDX
#define PIN_KSZ8051MNL_TXEN_FLAGS            (IOPORT_MODE_MUX_A)
#define PIN_KSZ8051MNL_TXD3_IDX               PIO_PD16_IDX
#define PIN_KSZ8051MNL_TXD3_FLAGS            (IOPORT_MODE_MUX_A)
#define PIN_KSZ8051MNL_TXD2_IDX               PIO_PD15_IDX
#define PIN_KSZ8051MNL_TXD2_FLAGS            (IOPORT_MODE_MUX_A)
#define PIN_KSZ8051MNL_TXD1_IDX               PIO_PD3_IDX
#define PIN_KSZ8051MNL_TXD1_FLAGS            (IOPORT_MODE_MUX_A)
#define PIN_KSZ8051MNL_TXD0_IDX               PIO_PD2_IDX
#define PIN_KSZ8051MNL_TXD0_FLAGS            (IOPORT_MODE_MUX_A)
#define PIN_KSZ8051MNL_RXD3_IDX               PIO_PD12_IDX
#define PIN_KSZ8051MNL_RXD3_FLAGS            (IOPORT_MODE_MUX_A)
#define PIN_KSZ8051MNL_RXD2_IDX               PIO_PD11_IDX
#define PIN_KSZ8051MNL_RXD2_FLAGS            (IOPORT_MODE_MUX_A)
#define PIN_KSZ8051MNL_RXD1_IDX               PIO_PD6_IDX
#define PIN_KSZ8051MNL_RXD1_FLAGS            (IOPORT_MODE_MUX_A)
#define PIN_KSZ8051MNL_RXD0_IDX               PIO_PD5_IDX
#define PIN_KSZ8051MNL_RXD0_FLAGS            (IOPORT_MODE_MUX_A)
#define PIN_KSZ8051MNL_RXER_IDX               PIO_PD7_IDX
#define PIN_KSZ8051MNL_RXER_FLAGS            (IOPORT_MODE_MUX_A)
#define PIN_KSZ8051MNL_RXDV_IDX               PIO_PD4_IDX
#define PIN_KSZ8051MNL_RXDV_FLAGS            (IOPORT_MODE_MUX_A)
#define PIN_KSZ8051MNL_CRS_IDX                PIO_PD10_IDX
#define PIN_KSZ8051MNL_CRS_FLAGS             (IOPORT_MODE_MUX_A)
#define PIN_KSZ8051MNL_COL_IDX                PIO_PD13_IDX
#define PIN_KSZ8051MNL_COL_FLAGS             (IOPORT_MODE_MUX_A)
#define PIN_KSZ8051MNL_MDC_IDX                PIO_PD8_IDX
#define PIN_KSZ8051MNL_MDC_FLAGS             (IOPORT_MODE_MUX_A)
#define PIN_KSZ8051MNL_MDIO_IDX               PIO_PD9_IDX
#define PIN_KSZ8051MNL_MDIO_FLAGS            (IOPORT_MODE_MUX_A)
#define PIN_KSZ8051MNL_INTRP_IDX              PIO_PD28_IDX

/*----------------------------------------------------------------------------*/
/**
 * \page sam4e_ek_usb "SAM4E-EK - USB device"
 *
 * \section Definitions
 * - \ref BOARD_USB_BMATTRIBUTES
 * - \ref CHIP_USB_UDP
 * - \ref CHIP_USB_PULLUP_INTERNAL
 * - \ref CHIP_USB_NUMENDPOINTS
 * - \ref CHIP_USB_ENDPOINTS_MAXPACKETSIZE
 * - \ref CHIP_USB_ENDPOINTS_BANKS
 */

/**
 * USB attributes configuration descriptor (bus or self powered,
 * remote wakeup)
 */
#define BOARD_USB_BMATTRIBUTES  USBConfigurationDescriptor_SELFPOWERED_RWAKEUP

/** Indicates chip has an UDP Full Speed. */
#define CHIP_USB_UDP

/** Indicates chip has an internal pull-up. */
#define CHIP_USB_PULLUP_INTERNAL

/** Number of USB endpoints */
#define CHIP_USB_NUMENDPOINTS 8

/** Endpoints max packet size */
#define CHIP_USB_ENDPOINTS_MAXPACKETSIZE(i) \
   ((i == 0) ? 64 : \
   ((i == 1) ? 64 : \
   ((i == 2) ? 64 : \
   ((i == 3) ? 64 : \
   ((i == 4) ? 512 : \
   ((i == 5) ? 512 : \
   ((i == 6) ? 64 : \
   ((i == 7) ? 64 : 0 ))))))))

/** Endpoints Number of Bank */
#define CHIP_USB_ENDPOINTS_BANKS(i) \
   ((i == 0) ? 1 : \
   ((i == 1) ? 2 : \
   ((i == 2) ? 2 : \
   ((i == 3) ? 1 : \
   ((i == 4) ? 2 : \
   ((i == 5) ? 2 : \
   ((i == 6) ? 2 : \
   ((i == 7) ? 2 : 0 ))))))))

/*----------------------------------------------------------------------------*/
/**
 * \page sam4e_ek_extcomp "SAM4E-EK - External components"
 * This page lists the definitions related to external on-board components
 * located in the board.h file for the SAM4E-EK.
 *
 * SD Card
 * - \ref BOARD_SD_PINS
 * - \ref BOARD_SD_PIN_CD
 *
 */

/** HSMCI pins that shall be configured to access the SD card. */
#define BOARD_SD_PINS               PINS_HSMCI
/** HSMCI Card Detect pin. */
#define BOARD_SD_PIN_CD             PIN_HSMCI_CD

/*----------------------------------------------------------------------------*/

#define CONSOLE_UART               UART0
#define CONSOLE_UART_ID            ID_UART0

/* RE pin. */
#define PIN_RE_IDX                 PIN_USART1_CTS_IDX
#define PIN_RE_FLAGS               (0)

/* IRDA SD pin. */
#define PIN_IRDA_SD_IDX            PIN_USART1_CTS_IDX
#define PIN_IRDA_SD_FLAGS          (0)

/* TXD pin configuration. */
#define PIN_USART_TXD_IDX          PIN_USART1_TXD_IDX
#define PIN_USART_TXD_FLAGS        (IOPORT_MODE_MUX_A)
#define PIN_USART_TXD_IO_FLAGS     (0)

/*----------------------------------------------------------------------------*/
/* GMAC HW configurations */
#define BOARD_GMAC_PHY_ADDR 0

/*----------------------------------------------------------------------------*/
/**
 * \page sam4e_ek_CAN "SAM4E-EK - CAN"
 * This page lists definitions related to CAN0 and CAN1.
 *
 * CAN
 * - \ref PIN_CAN0_TRANSCEIVER_RXEN
 * - \ref PIN_CAN0_TRANSCEIVER_RS
 * - \ref PIN_CAN0_TXD
 * - \ref PIN_CAN0_RXD
 * - \ref PINS_CAN0
 */
/** CAN0 transceiver PIN RS. */
#define PIN_CAN0_TR_RS_IDX        PIO_PA17_IDX
#define PIN_CAN0_TR_RS_FLAGS      IOPORT_DIR_OUTPUT

/** CAN0 transceiver PIN EN. */
#define PIN_CAN0_TR_EN_IDX        PIO_PA18_IDX
#define PIN_CAN0_TR_EN_FLAGS      IOPORT_DIR_OUTPUT

/** CAN0 PIN RX. */
#define PIN_CAN0_RX_IDX           PIO_PB3_IDX
#define PIN_CAN0_RX_FLAGS         IOPORT_MODE_MUX_A

/** CAN0 PIN TX. */
#define PIN_CAN0_TX_IDX           PIO_PB2_IDX
#define PIN_CAN0_TX_FLAGS         IOPORT_MODE_MUX_A

/*! \name Connections of the AT86RFX transceiver
 */
//! @{

#define AT86RFX_SPI                  SPI
#define AT86RFX_RST_PIN              IOPORT_CREATE_PIN(PIOA, 15)
#define AT86RFX_IRQ_PIN              IOPORT_CREATE_PIN(PIOA, 11)
#define AT86RFX_SLP_PIN              IOPORT_CREATE_PIN(PIOD, 25)
#define AT86RFX_SPI_CS               1
#define AT86RFX_SPI_CS_PIN           SPI_NPCS1_PB14_GPIO
#define AT86RFX_SPI_CS_FLAGS         SPI_NPCS1_PB14_FLAGS
#define AT86RFX_SPI_MOSI             IOPORT_CREATE_PIN(PIOA, 13)
#define AT86RFX_SPI_MISO             IOPORT_CREATE_PIN(PIOA, 12)
#define AT86RFX_SPI_SCK              IOPORT_CREATE_PIN(PIOA, 14)

void at86rfx_isr(void);
#define AT86RFX_INTC_INIT()         ioport_set_pin_dir(AT86RFX_IRQ_PIN, IOPORT_DIR_INPUT);\
									ioport_set_pin_sense_mode(AT86RFX_IRQ_PIN, IOPORT_SENSE_RISING);\
									pmc_enable_periph_clk(ID_PIOA);\
									pio_set_debounce_filter(PIOA, PIO_PA11, 10);\
									pio_handler_set(PIOA, ID_PIOA, PIO_PA11, PIO_IT_HIGH_LEVEL, at86rfx_isr);\
									NVIC_EnableIRQ((IRQn_Type) ID_PIOA);\
									pio_enable_interrupt(PIOA, PIO_PA11);									

#define AT86RFX_ISR()               void at86rfx_isr(void)

/** Enables the transceiver main interrupt. */
#define ENABLE_TRX_IRQ()            pio_enable_pin_interrupt(AT86RFX_IRQ_PIN)

/** Disables the transceiver main interrupt. */
#define DISABLE_TRX_IRQ()           pio_disable_pin_interrupt(AT86RFX_IRQ_PIN)

/** Clears the transceiver main interrupt. */
#define CLEAR_TRX_IRQ()             NVIC_ClearPendingIRQ(PIOA_IRQn);
/*
 * This macro saves the trx interrupt status and disables the trx interrupt.
 */
#define ENTER_TRX_REGION()          pio_disable_pin_interrupt(AT86RFX_IRQ_PIN);
/*
 *  This macro restores the transceiver interrupt status
 */
#define LEAVE_TRX_REGION()         pio_enable_pin_interrupt(AT86RFX_IRQ_PIN)

//@}
/*----------------------------------------------------------------------------*/

// Device List
#define PN532_DEVICE					(0x48)
#define M24LR_DEVICE					(0x28)
#define SL3S40xx_DEVICE					(0x4A)

#endif  /* _EM400EB_H_ */
