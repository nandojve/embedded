/**
 * @file pal.c
 *
 * @brief General PAL functions for AVR 8-bit MegaRF single chips
 *
 * This file implements generic PAL function and performs
 * USB related handling if required for AVR 8-bit MegaRF single chips.
 *
 * $Id: pal.c 27848 2011-07-26 12:14:45Z uwalter $
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

#include <stdint.h>
#include <stdbool.h>
#include "pal.h"
#include "pal_config.h"
#include "pal_timer.h"
#include "pal_internal.h"
#include "app_config.h"

#ifdef USB0
#include "pal_usb.h"
#endif /* USB0 */

/* === Globals ============================================================= */

/* === Prototypes ========================================================== */

#ifdef WATCHDOG
static inline void wdt_init(void);
#endif

/* === Implementation ====================================================== */

/**
 * @brief Initialization of PAL
 *
 * This function initializes the PAL. The RC oscillator is calibrated.
 *
 * @return MAC_SUCCESS  if PAL initialization is successful, FAILURE otherwise
 */
retval_t pal_init(void)
{
    /* Clear any pending watchdog reset flag */
    uint8_t temp = MCUSR;
    temp++; /* keep compiler happy */
    MCUSR = 0;

#ifdef WATCHDOG
    /* Watchdog Initialization. Enables the watchdog and sets the timeout period */
    wdt_init();
#else
    WDT_RESET();
    WDT_DISABLE();
#endif

    mcu_init();     // see pal_mcu_generic.c
    timer_init();   // see pal_timer.c
    gpio_init();    // see pal_board.c
	
	driver_init();

    return MAC_SUCCESS;
}



/**
 * @brief Services timer and sio handler
 *
 * This function calls sio & timer handling functions.
 */
void pal_task(void)
{
#if (TOTAL_NUMBER_OF_TIMERS > 0)
    timer_service();
#endif

#ifdef USB0
    /*
     * If the serial communication is done using USB, only then the 'usb_handler'
     * needs to be called. For UART, the data handling is done in the UART ISR.
     */
    usb_handler();
#endif /* USB0 */

#ifdef WATCHDOG
    WDT_RESET();
#endif

	driver_task();
}



/**
 * @brief Get data from persistence storage
 *
 * @param[in]  ps_type Persistence storage type
 * @param[in]  start_addr Start offset within EEPROM
 * @param[in]  length Number of bytes to read from EEPROM
 * @param[out] value Data from persistence storage
 *
 * @return MAC_SUCCESS  if everything went OK else FAILURE
 */
retval_t pal_ps_get(ps_type_t ps_type, uint16_t start_addr, uint16_t length, void *value)
{
#if (EXTERN_EEPROM_AVAILABLE == 1)
    if (ps_type == EXTERN_EEPROM)
    {
        return extern_eeprom_get(start_addr, length, value);
    }
    else
#endif

    if (ps_type == INTERN_EEPROM)
    {
        uint16_t index;
        uint8_t *data_ptr;

        if ((start_addr + length) > (E2END + 1))
        {
            return FAILURE;
        }

        data_ptr = (uint8_t *)(value);
        for (index = 0; index < length; index++)
        {
            EEGET(*data_ptr, start_addr);
            start_addr++;
            data_ptr++;
        }
    }
    else    // no internal eeprom and no external eeprom available
    {
        return MAC_INVALID_PARAMETER;
    }

    return MAC_SUCCESS;
}



/**
 * @brief Write data to persistence storage
 *
 * @param[in]  start_addr Start address offset within EEPROM
 * @param[in]  length Number of bytes to be written to EEPROM
 * @param[in]  value Data to persistence storage
 *
 * @return MAC_SUCCESS  if everything went OK else FAILURE
 */
retval_t pal_ps_set(uint16_t start_addr, uint16_t length, void *value)
{
    uint8_t *data_ptr;
    uint16_t i;
    uint8_t read_data;

    if ((start_addr + length) > (E2END + 1))
    {
        return FAILURE;
    }

    data_ptr = (uint8_t *)(value);
    for (i = 0; i < length; i++)
    {
        EEGET(read_data, start_addr);
        if (read_data != *data_ptr)
        {
            EEPUT(start_addr, *data_ptr);
        }
        data_ptr++;
        start_addr++;
    }

    return MAC_SUCCESS;
}



/*
 * @brief Alert indication
 *
 * This Function can be used by any application to indicate an error condition.
 * The function is blocking and does never return.
 */
void pal_alert(void)
{
#if (DEBUG > 0)
    bool debug_flag = false;
#endif
    ALERT_INIT();

    while (1)
    {
        pal_timer_delay(0xFFFF);
        ALERT_INDICATE();

#if (DEBUG > 0)
        /* Used for debugging purposes only */
        if (debug_flag == true)
        {
            break;
        }
#endif
    }
}


/**
 * @brief Initialize the watchdog timer of the ATmega128rfa1
 */
#if defined(WATCHDOG) || defined(DOXYGEN)
static inline void wdt_init(void)
{
    /* Initialize the Watchdog with the mode and the time-out period */
#ifdef __ICCAVR__
    WDT_SYSTEM_RESET_MODE(WDT_TIMEOUT_PERIOD);
#endif
#ifdef __GNUC__
    wdt_enable(WDT_TIMEOUT_PERIOD);
#endif
}
#endif


/* EOF */
