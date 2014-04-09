/**
 * @file pal_internal.h
 *
 * @brief PAL  internal functions prototypes for AVR32 MCUs
 *
 * $Id: pal_internal.h 26573 2011-05-10 06:11:21Z yogesh.bellan $
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
#ifndef PAL_INTERNAL_H
#define PAL_INTERNAL_H

/* === Includes ============================================================= */

#include "pal.h"

/* === Types ================================================================ */


/* === Externals ============================================================ */


/* === Macros ================================================================ */


/* === Prototypes =========================================================== */

#ifdef __cplusplus
extern "C" {
#endif

/* Initialize the GPIO of AT32UC3 Series of MCUs*/
void gpio_init(void);

/* To Enable GPIO output of AT32UC3 Series of MCUs*/
void gpio_enable_output(uint32_t);

/* To Enable GPIO input of AT32UC3 Series of MCUs*/
void gpio_enable_input(uint32_t);

/* To Set GPIO Pin of AT32UC3 Series of MCUs*/
void gpio_set_gpio_pin(uint32_t);

/* To Clear GPIO Pin of AT32UC3 Series of MCUs*/
void gpio_clr_gpio_pin(uint32_t);

/* To Toggle GPIO Pin of AT32UC3 Series of MCUs*/
void gpio_tgl_gpio_pin(uint32_t);

/* To Get GPIO Pin Status of AT32UC3 Series of MCUs*/
pin_state_t gpio_get(uint32_t);

/* Initialize the clock system of AT32UC3 Series of MCUs*/
void clock_init(void);

/* Initialize the Interrupt System of AT32UC3 Series of MCUs*/
void interrupt_system_init(void);

/* TO read the timestamp Value */
void read_timestamp();

#if ((PAL_TYPE == AT32UC3B1128) || (PAL_TYPE == AT32UC3B164))
/**
 * @brief External Interrupt Controller Initialization
 *
 * Initializes the External Interrupt Controller Module
 *
 * @param *eic Pointer to EIC module address
 *        *opt Array containing the varialble for the external interrupt lines
 *         nb_lines Number of lines used for the External Interupt
 * @ingroup apiPalApi
 */
void eic_init(volatile avr32_eic_t *eic, const eic_options_t *opt, uint32_t nb_lines);

/**
 * @brief External Interrupt Controller Line Enable
 *
 * Enables the line used in External Interrupt Controller Module
 *
 * @param *eic Pointer to EIC module address
 *        line_number Line number used for the External Interupt
 * @ingroup apiPalApi
 */
void eic_enable_line(volatile avr32_eic_t *eic, uint32_t line_number);

/**
 * @brief External Interrupt Controller Interrupt Line Enable
 *
 * Enables the interrupt line used in External Interrupt Controller Module
 *
 * @param *eic Pointer to EIC module address
 *        line_number Line number used for the External Interupt
 * @ingroup apiPalApi
 */
void eic_enable_interrupt_line(volatile avr32_eic_t *eic, uint32_t line_number);

/**
 * @brief External Interrupt Controller Interrupt Line Disable
 *
 * Disables the interrupt line used in External Interrupt Controller Module
 *
 * @param *eic Pointer to EIC module address
 *        line_number Line number used for the External Interupt
 * @ingroup apiPalApi
 */
void eic_disable_interrupt_line(volatile avr32_eic_t *eic, uint32_t line_number);

/**
 * @brief External Interrupt Controller Interrupt Line Clear
 *
 * Clears the interrupt line used in External Interrupt Controller Module
 *
 * @param *eic Pointer to EIC module address
 *        line_number Line number used for the External Interupt
 * @ingroup apiPalApi
 */
void eic_clear_interrupt_line(volatile avr32_eic_t *eic, uint32_t line_number);

/**
 * @brief Sleep/Power handling
 *
 * Handles Sleep/power modes for entire system
 *
 * @param sleep_mode desired sleep/power mode
 *
 * @ingroup apiPalApi
 */
void pal_sleep_mode(uint8_t sleep_mode);
#endif /*(PAL_TYPE == AT32UC3B1128) || (PAL_TYPE == AT32UC3B164)*/

#if defined(WATCHDOG) || defined(SLEEPING_TIMER) || defined(DOXYGEN)
#if (PAL_GENERIC_TYPE == AVR32)

/**
 * @brief Initialize the watchdog timer of the ATxmega1281
 */
void wdt_init(void);

/**
 * @brief Parallel Software timer initialization if the Sleeping timer (RTC) is
 * not enabled to reset the Watchdog timer periodically.
 */
void wdt_parallel_timer_init(void);

/**
 * @brief Initialization of Sleeping timer (RTC) of the Atxmega1281 when Watchdog Timer
 * is not enabled. The period for the Sleeping timer should be given by the user in this case.
 */
void sleeping_timer_without_wdt(void);

/**
 * @brief Initialization of Sleeping timer (RTC) of the Atxmega1281 when Watchdog Timer
 * is enabled. The period for the Sleeping timer should be given by the user in this case
 * and the period of the sleeping timer should be less than the time-out period of the
 * watchdog timer.
 * @note It is required that the period of the Sleeping Timer (RTC) is less than the Time-out period
 * of Watchdog Timer (If enabled) to avoid the unintentional reset of the device.
 */
void sleeping_timer_with_wdt(void);

/**
 * @brief Initialization of sleeping timer (RTC) of the Atxmega1281
 */
void sleeping_timer_init(void);

/**
 * @brief Resets the watchdog timer of the ATxmega1281 before it
 * reaches its time-out period
 */
void wdt_clear(void);

/**
 * @brief Enable the watchdog timer and set the desired time-out period
 *
 * @param Time-out period to be set for the Watchdog Timer
 */
void pal_wdt_enable(unsigned long us_timeout_period);

/**
 * @brief Set the CTRL register of the watchdog timer of the ATUC3A3256
 */
void wdt_set_ctrl(unsigned long ctrl);

/**
 * @brief Check whether RTC is busy
 */
int rtc_is_busy(volatile avr32_rtc_t *rtc);

/**
 * @brief Initialization of the RTC
 * @param osc_type
 */
void pal_rtc_init(volatile avr32_rtc_t *rtc, unsigned char osc_type);

/**
 * @brief Sets the value in the VAL register of the RTC module
 * @param val value
 */
void rtc_set_value(volatile avr32_rtc_t *rtc, unsigned long val);

/**
 * @brief Enables the RTC interrupt
 */
void rtc_enable_interrupt(volatile avr32_rtc_t *rtc);


/**
 * @brief Sets the top value in the TOP register of the RTC module
 * @param top
 */
void rtc_set_top_value(volatile avr32_rtc_t *rtc, unsigned long top);

/**
 * @brief Enables the RTC module
 */
void rtc_enable(volatile avr32_rtc_t *rtc);

/**
 * @brief Clears the RTC interrupt
 */
void rtc_clear_interrupt(volatile avr32_rtc_t *rtc);

#endif
#endif

#if (defined __GNUC__)
    void ext_int_isr(void);
#elif (defined __ICCAVR32__)
    #pragma handler = EXT_INT_ISR_GROUP, EXT_INT_ISR_PRIORITY
    __interrupt void ext_int_isr(void);
#endif

#if ((PAL_TYPE != AT32UC3B1128) && (PAL_TYPE != AT32UC3B164))
#if (defined __GNUC__)
    void TC_isr(void);
#elif (defined __ICCAVR32__)
    #pragma handler=TC_CH1_ISR_GROUP,TC_CH1_ISR_PRIORITY
    __interrupt void TC_isr(void);
#endif
#endif

#if (defined __GNUC__)
    void TC2_isr(void);
#elif (defined __ICCAVR32__)
    #pragma handler=TC_CH2_ISR_GROUP,TC_CH2_ISR_PRIORITY
    __interrupt void TC2_isr(void);
#endif

#if( (defined UART0) ||(defined UART1) ||(defined UART2) || (defined UART3))
#if (defined __GNUC__)
void usart0_isr(void);
#elif (defined __ICCAVR32__)
    #pragma handler=USART_ISR_GROUP,USART_ISR_PRIORITY
    __interrupt void usart0_isr(void);
#endif
#endif

#if (defined USB0)
#if (defined __GNUC__)
    void usb_general_interrupt(void);
#elif (defined __ICCAVR32__)
    #pragma handler=17, 0
    __interrupt void usb_general_interrupt(void);
#endif
#endif

#if (defined __GNUC__)
    void eic_int_handler(void);
#elif (defined __ICCAVR32__)
#pragma handler = 1, 1 //ToDo - Put macros EXTERNAL_INTERRUPT_ISR_GROUP EXTERNAL_INTERRUPT_PRIORITY
    __interrupt void eic_int_handler(void);
#endif

/* Real time clock APIs*/
void Enable_RTC();

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif  /* PAL_INTERNAL_H */
/* EOF */
