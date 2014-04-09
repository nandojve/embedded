/**
 * @file pal.c
 *
 * @brief General file for PAL for AVR32 MCUs
 *
 * This file implements generic PAL function and performs
 * USB related handling if required for AVR32 AT32UC3X MCUs.
 *
 * $Id: pal.c 29398 2011-11-18 08:18:51Z yogesh.bellan $
 *
 * @author    Atmel Corporation: http://www.atmel.com
 * @author    Support email: avr@atmel.com
 */
/*
 * Copyright (c) 2010, Atmel Corporation All rights reserved.
 *
 * Licensed under Atmel's Limited License Agreement --> EULA.txt
 */
/* === Includes ============================================================ */
#include <stdint.h>
#include <stdbool.h>
#include "pal_types.h"
#include "pal_config.h"
#include "pal_timer.h"
#include "pal_internal.h"
#include "pal_trx_access.h"
#include "pal.h"

#ifdef FLASH_NVRAM
#include "pal_flash.h"
#endif

#ifdef USB0
#include "pal_usb.h"
#endif /* USB0 */

/* === Macros ============================================================== */

/* === Globals ============================================================= */
/**
 * This is the most significant part of the system time. The least one is
 * timer1 itself.  Exported since input capture units might want
 * to know about this value as well.
 */
volatile uint16_t sys_time;

/* NVRAM data structure located in the flash array. */
#ifdef FLASH_NVRAM
#if defined (__GNUC__)
__attribute__((__section__(".flash_nvram_adr")))
static uint8_t flash_nvram[8]
#endif
#if defined (__ICCAVR32__)
static __no_init uint8_t flash_nvram[1]
@ flash_nvram_adr
#endif
;
#endif // FLASH_NVRAM


/* === Prototypes ========================================================== */

/* === Implementation ====================================================== */

/**
 * @brief Initialization of PAL
 *
 * This function initializes the PAL.
 *
 * @return SUCCESS if PAL initialization is successful, FAILURE otherwise
  */
retval_t pal_init(void)
{
    /*Initializing the clocks for AVR32 MCUs*/
    clock_init();
    /*Initializing the pin configurations for AVR32 MCUs*/
    gpio_init();
    /*Initializing the AVR32 MCUs spi interface to the transceiver*/
    trx_interface_init();
    /*Initializing the timers of AVR32 MCUs for output & input capture*/
    timer_init();
    /*Initializing the interrupt system of AVR32 MCUs*/
    interrupt_system_init();
#if (defined WATCHDOG)
    wdt_init();
#endif
#if (defined SLEEPING_TIMER)
    sleeping_timer_init();
#elif (defined WATCHDOG)
    wdt_parallel_timer_init();
#endif
    /*Returning success after processor & RF initialization*/
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
    /*
     * If the serial communication is done using USB, only then the 'usb_handler'
     * needs to be called. For UART, the data handling is done in the UART ISR.
     */
#ifdef USB0
    usb_handler();
#endif /* USB0 */
#ifdef WATCHDOG
    wdt_clear();
#endif
}

/**
 * @brief Get data from persistence storage
 *
 * @param[in]  ps_type Persistence storage type
 * @param[in]  start_addr Start offset within EEPROM
 * @param[in]  length Number of bytes to read from EEPROM
 * @param[out] value Data from persistence storage
 *
 * @return MAC_SUCCESS if everything went OK else FAILURE
 */
retval_t pal_ps_get(ps_type_t ps_type, uint16_t start_addr, uint16_t length, void *value)
{
#if (EXTERN_EEPROM_AVAILABLE == 1)
    if (ps_type == EXTERN_EEPROM)
    {
        /* No function is available to handle external EEEPROM and
         therefore a failure is return */
        return FAILURE;
    }
    else
#endif

    if (ps_type == INTERN_EEPROM)
    {
    #ifdef FLASH_NVRAM
        uint16_t index;
        uint8_t *data_ptr;

        data_ptr = (uint8_t *)(value);

        for (index = 0; index < length; index++)
        {
            data_ptr[index]=flash_nvram[start_addr];
            start_addr++;
        }
    #else
        /* Commented to get the Random IEEE Address */
       // *(uint64_t *)value = IEEE_ADDR;
    #endif
    }
    else    /* no internal eeprom and no external eeprom available */
    {
        return MAC_INVALID_PARAMETER;
    }

    /* Keep compiler happy */
    start_addr = start_addr;
    length = length;

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
#ifdef FLASH_NVRAM
    flash_memcpy((void *) & flash_nvram[start_addr], value, length, true);
#else
    /* Keep compiler happy. */
    /* Board does not support eeprom */
    start_addr = start_addr;
    length = length;
    value = value;
#endif

    return MAC_SUCCESS;
}


/*
 * @brief Alert indication
 *
 * This Funciton can be used by any application to indicate an error condition.
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

/*
 * @brief __unhandled_interrupt
 *
 * This Funciton added in order to avoid linker error messages for AVR32 Latest
 * version - 4.10.2.
 */
#if defined (__ICCAVR32__)
__interrupt void __unhandled_interrupt(void)
{
    pal_alert();
}
#endif

/* EOF */
