/**
 * @file pal.c
 *
 * @brief General PAL functions for SAM4S MCUs
 *
 * This file implements generic PAL function for SAM4S MCUs.
 *
 * $Id: pal.c 30341 2012-01-20 07:31:44Z mahendran.p $
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
#include "pal.h"
#include "pal_config.h"
#include "pal_timer.h"
#include "pal_internal.h"
#ifdef INTERN_FLASH_ENABLED
#include "pal_flash.h"
#endif

/* === Macros =============================================================== */

/* Mask to act on all the interrupts at once. */
#define ALL_INTERRUPTS_MASK         (0xFFFFFFFF)

/* Maximum number of interrupt sources that can be defined. This
constant can be increased, but the current value is the smallest possible
that will be compatible with all existing projects */
#define MAX_INTERRUPT_SOURCES       (7)

/* Watchdog control register key password*/
#define WDT_CR_PASSWORD             (0xA5)
/* === Globals ============================================================= */

#ifdef TIMESTAMP_UPPER_16BIT_SW
/*
 * This is the most significant part of the system time.
 */
volatile uint16_t sys_time;
#endif

/* List of interrupt sources */
static interruptSource_t pSources[MAX_INTERRUPT_SOURCES];

/* Number of currently defined interrupt sources */
static uint32_t numSources = 0;

/* === Prototypes ========================================================== */
static void pal_pio_setperiphA(Pio *pio, uint32_t mask, uint8_t enablePullUp);
static void pal_pio_setperiphB(Pio *pio, uint32_t mask, uint8_t enablePullUp);
static void pal_pio_setperiphC(Pio *pio, uint32_t mask, uint8_t enablePullUp);
static void pal_pio_setperiphD(Pio *pio, uint32_t mask, uint8_t enablePullUp);
static void pal_pio_setinput(Pio *pio, uint32_t mask, uint8_t attribute);
static void pal_pio_setoutput(Pio *pio, uint32_t mask, uint8_t defaultValue,
                              uint8_t enableMultiDrive,
                              uint8_t enablePullUp);
static void pal_pio_interrupt_handler(uint32_t id, Pio *pPio);
void PIOA_Handler(void);
void PIOB_Handler(void);
void PIOC_Handler(void);

#ifdef WATCHDOG
static void pal_wdt_init(void);
static void pal_wdt_reset(void);
#else
static void pal_wdt_disable(void);
#endif

/* === Implementation ====================================================== */

/**
 * @brief Initialization of PAL
 *
 * This function initializes the PAL.
 *
 * @return MAC_SUCCESS  if PAL initialization is successful, FAILURE otherwise
 */
retval_t pal_init(void)
{

#ifdef WATCHDOG
    /* Watchdog Initialization. Enables the watchdog and sets the timeout period */
    pal_wdt_init();
#else
    /* Disabling watch dog*/
    pal_wdt_disable();
#endif

    /* Initialising PIO pins */
    gpio_init();

    /* Initialising PIO interrupts with Zero prority*/
    pal_pio_initialize_interrupts(0);

    /* Initialising tranceiver interface */
    trx_interface_init();

    /* Initialising timer for PAL */
    timer_init();

    return MAC_SUCCESS;
}


/**
 * @brief Services timer
 *
 * This function calls timer handling functions.
 */
void pal_task(void)
{
#if (TOTAL_NUMBER_OF_TIMERS > 0)
    timer_service();
#endif
#ifdef WATCHDOG
    pal_wdt_reset();
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
 * @return MAC_SUCCESS  if everything went OK else FAILURE
 */
retval_t pal_ps_get(ps_type_t ps_type, uint16_t start_addr, uint16_t length, void *value)
{
    /* Board does not has EEPROM */
#if (EXTERN_EEPROM_AVAILABLE == 1)
    if (ps_type == EXTERN_EEPROM)
    {
        /* eeprom get function */
    }
    else
#endif

        if (ps_type == INTERN_EEPROM)
        {
#ifdef INTERN_FLASH_ENABLED
            pal_flash_read(start_addr, length, value);
#endif
        }
        else
        {
            return MAC_INVALID_PARAMETER;
        }
    /* Keep compiler happy */
    start_addr = start_addr;
    length = length;
    value = value;

    return FAILURE;
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
    /* Board does not has EEPROM */
#ifdef INTERN_FLASH_ENABLED
    uint8_t status;
    status = pal_flash_write(start_addr, value, length);
    ASSERT(!status);
    if (!status)
    {
        return MAC_SUCCESS;
    }
#endif
    /* Keep compiler happy. */
    start_addr = start_addr;
    length = length;
    value = value;

    return FAILURE;
}


/**
 * @brief Alert indication
 *
 * This Function can be used by any application to indicate an error condition.
 * The function is blocking and never returns.
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
 * @brief Get Periph Status for the given peripheral ID.
 *
 * @param peripheral_id  Peripheral ID (ID_xxx).
 * @return 32bit value - status of the clock
 */
uint32_t pal_get_peripheral_clock_status(uint8_t peripheral_id)
{
#if (DEBUG > 0)
    ASSERT(peripheral_id < 35);
#endif
    if (peripheral_id < 32)
    {
        return (PMC->PMC_PCSR0 & (1 << peripheral_id));
    }
    else
    {
        return (PMC->PMC_PCSR1 & (1 << (peripheral_id - 32)));
    }
}


/**
 * @brief Enables the clock of a peripheral
 *
 * This function enables the clock of peripheral. The peripheral ID is used
 * to identify which peripheral is targetted.
 *
 * @param peripheral_id  Peripheral ID (ID_xxx).
 */
void pal_peripheral_clock_enable(uint8_t peripheral_id)
{
#if (DEBUG > 0)
    ASSERT(peripheral_id < 35);
#endif
    if (peripheral_id < 32)
    {
        if ((PMC->PMC_PCSR0 & (1 << peripheral_id)) == (1 << peripheral_id))
        {
            /* Already Enabled .Do nothing */
        }
        else
        {
            PMC->PMC_PCER0 = 1 << peripheral_id;
        }
    }
    else
    {
        peripheral_id -= 32;
        if ((PMC->PMC_PCSR1 & (1 << peripheral_id)) == (1 << peripheral_id))
        {
            /* Already Enabled .Do nothing */
        }
        else
        {
            PMC->PMC_PCER1 = 1 << peripheral_id;
        }
    }
}


/**
 * @brief Disables the clock of a peripheral. The peripheral ID is used
 * to identify which peripheral is targetted.
 *
 * @param peripheral_id  Peripheral ID (ID_xxx).
 */
void pal_peripheral_clock_disable(uint8_t peripheral_id)
{
#if (DEBUG > 0)
    ASSERT(peripheral_id < 35);
#endif
    if (peripheral_id < 32)
    {
        if ((PMC->PMC_PCSR0 & (1 << peripheral_id)) != (1 << peripheral_id))
        {
            /* Already Disabled .Do nothing */
        }
        else
        {
            PMC->PMC_PCDR0 = 1 << peripheral_id;
        }
    }
    else
    {
        peripheral_id -= 32;
        if ((PMC->PMC_PCSR1 & (1 << peripheral_id)) != (1 << peripheral_id))
        {
            /* Already Disabled .Do nothing */
        }
        else
        {
            PMC->PMC_PCDR1 = 1 << peripheral_id;
        }
    }
}


/**
 * @brief Configures a list of Pin instances
 *
 * This function configures list of pins each of which can either hold a single
 * pin or a group of pins, depending on the mask value; all pins are configured
 * by this function. The size of the array must also be provided and is easily
 * computed using PIO_LISTSIZE whenever its length is not known in advance.
 *
 * @param size  Size of the Pin list (calculated using PIO_LISTSIZE)
 * @param list  Pointer to a list of Pin instances.
 *
 * @return 1 if the pins have been configured properly; otherwise 0
 */
bool pal_pio_configure(const pin_t *list, uint32_t size)
{
    while (size > 0)
    {
        switch (list->type)
        {
            case PIO_PERIPH_A:
                pal_pio_setperiphA(list->pio,
                                   list->mask,
                                   (list->attribute & PIO_PULLUP) ? 1 : 0);
                break;
            case PIO_PERIPH_B:
                pal_pio_setperiphB(list->pio,
                                   list->mask,
                                   (list->attribute & PIO_PULLUP) ? 1 : 0);
                break;
            case PIO_PERIPH_C:
                pal_pio_setperiphC(list->pio,
                                   list->mask,
                                   (list->attribute & PIO_PULLUP) ? 1 : 0);
                break;
            case PIO_PERIPH_D:
                pal_pio_setperiphD(list->pio,
                                   list->mask,
                                   (list->attribute & PIO_PULLUP) ? 1 : 0);
                break;

            case PIO_INPUT:
                pal_peripheral_clock_enable(list->id);
                pal_pio_setinput(list->pio,
                                 list->mask,
                                 list->attribute);
                break;

            case PIO_OUTPUT_0:
            case PIO_OUTPUT_1:
                pal_pio_setoutput(list->pio,
                                  list->mask,
                                  (list->type == PIO_OUTPUT_1),
                                  (list->attribute & PIO_OPENDRAIN) ? 1 : 0,
                                  (list->attribute & PIO_PULLUP) ? 1 : 0);
                break;

            default:
                return 0;
        }

        list++;
        size--;
    }

    return 1;
}


/**
 * @brief Configures one or more pin(s) of PIOA
 *
 * This function configures one or more pin(s) of a PIO controller as
 * being controlled by peripheral A. Optionally, the corresponding internal
 * pull-up(s) can be enabled.
 *
 * @param pio  Pointer to a PIO controller.
 * @param mask  Bitmask of one or more pin(s) to configure.
 * @param enablePullUp  Indicates if the pin(s) internal pull-up shall be
 *                      configured.
 */
static void pal_pio_setperiphA(Pio *pio, uint32_t mask, uint8_t enablePullUp)
{
    uint32_t abcdsr;

    /* Enable the pull-up(s) if necessary */
    if (enablePullUp)
    {
        pio->PIO_PUER = mask;
    }
    else
    {
        pio->PIO_PUDR = mask;
    }
    abcdsr = pio->PIO_ABCDSR[0];
    pio->PIO_ABCDSR[0] &= (~mask & abcdsr);
    abcdsr = pio->PIO_ABCDSR[1];
    pio->PIO_ABCDSR[1] &= (~mask & abcdsr);
    pio->PIO_PDR = mask;
}


/**
 * @brief Configures one or more pin(s) of PIOB
 *
 * This function configures one or more pin(s) of a PIO controller as
 * being controlled by peripheral B. Optionally, the corresponding internal
 * pull-up(s) can be enabled.
 *
 * @param pio  Pointer to a PIO controller.
 * @param mask  Bitmask of one or more pin(s) to configure.
 * @param enablePullUp  Indicates if the pin(s) internal pull-up shall be
 *                      configured.
 */
static void pal_pio_setperiphB(Pio *pio, uint32_t mask, uint8_t enablePullUp)
{
    uint32_t abcdsr;

    /* Enable the pull-up(s) if necessary */
    if (enablePullUp)
    {
        pio->PIO_PUER = mask;
    }
    else
    {
        pio->PIO_PUDR = mask;
    }
    abcdsr = pio->PIO_ABCDSR[0];
    pio->PIO_ABCDSR[0] = (mask | abcdsr);
    abcdsr = pio->PIO_ABCDSR[1];
    pio->PIO_ABCDSR[1] &= (~mask & abcdsr);
    pio->PIO_PDR = mask;
}


/**
 * @brief Configures one or more pin(s) of PIOC
 *
 * This function configures one or more pin(s) of a PIO controller as
 * being controlled by peripheral C. Optionally, the corresponding internal
 * pull-up(s) can be enabled.
 *
 * @param pio  Pointer to a PIO controller.
 * @param mask  Bitmask of one or more pin(s) to configure.
 * @param enablePullUp  Indicates if the pin(s) internal pull-up shall be
 *                      configured.
 */
static void pal_pio_setperiphC(Pio *pio, uint32_t mask, uint8_t enablePullUp)
{
    uint32_t abcdsr;

    /* Enable the pull-up(s) if necessary */
    if (enablePullUp)
    {
        pio->PIO_PUER = mask;
    }
    else
    {
        pio->PIO_PUDR = mask;
    }
    abcdsr = pio->PIO_ABCDSR[0];
    pio->PIO_ABCDSR[0] &= (~mask & abcdsr);
    abcdsr = pio->PIO_ABCDSR[1];
    pio->PIO_ABCDSR[1] = (mask | abcdsr);
    pio->PIO_PDR = mask;
}


/**
 * @brief Configures one or more pin(s) of PIOD
 *
 * This function configures one or more pin(s) of a PIO controller as
 * being controlled by peripheral D. Optionally, the corresponding internal
 * pull-up(s) can be enabled.
 *
 * @param pio  Pointer to a PIO controller.
 * @param mask  Bitmask of one or more pin(s) to configure.
 * @param enablePullUp  Indicates if the pin(s) internal pull-up shall be
 *                      configured.
 */
static void pal_pio_setperiphD(Pio *pio, uint32_t mask, uint8_t enablePullUp)
{
    uint32_t abcdsr;

    /* Enable the pull-up(s) if necessary */
    if (enablePullUp)
    {
        pio->PIO_PUER = mask;
    }
    else
    {
        pio->PIO_PUDR = mask;
    }
    abcdsr = pio->PIO_ABCDSR[0];
    pio->PIO_ABCDSR[0] = (mask | abcdsr);
    abcdsr = pio->PIO_ABCDSR[1];
    pio->PIO_ABCDSR[1] = (mask | abcdsr);
    pio->PIO_PDR = mask;
}


/**
 * @brief Configures pin(s) of a PIO controller as inputs.
 *
 * This function configures one or more pin(s) or a PIO controller as inputs.
 * Optionally, the corresponding internal pull-up(s) and glitch filter(s)
 * can be enabled.
 *
 * @param pio  Pointer to a PIO controller.
 * @param mask  Bitmask of one or more pin(s) to configure.
 * @param attribute  Indicates if the pin(s) de-glitch/de-bounce shall be
 *                      configured.
 */
static void pal_pio_setinput(Pio *pio, uint32_t mask, uint8_t attribute)
{
    /* Enable pull-up(s) if necessary */
    if (attribute & PIO_PULLUP)
    {
        pio->PIO_PUER = mask;
    }
    else
    {
        pio->PIO_PUDR = mask;
    }

    /* Enable de-glitch(s) if necessary */
    if (attribute & PIO_DEGLITCH)
    {
        pio->PIO_IFER = mask;
    }
    else
    {
        pio->PIO_IFDR = mask;
    }

    /* Enable de-bounce if necessary*/
    if (attribute & PIO_DEBOUNCE)
    {
        pio->PIO_IFSCER = mask;
    }

    /* Configure pin as input */
    pio->PIO_ODR = mask;
    pio->PIO_PER = mask;
}


/**
 * @brief Configures pin(s) of a PIO controller as outputs
 *
 * This function configures one or more pin(s) or a PIO controller as outputs
 * Optionally,the corresponding internal pull-up(s) and glitch filter(s)
 * can be enabled.
 *
 * @param pio  Pointer to a PIO controller.
 * @param mask  Bitmask indicating which pin(s) to configure.
 * @param defaultValue  Default level on the pin(s).
 * @param enableMultiDrive  Indicates if the pin(s) shall be configured as
 *                        open-drain.
 * @param enablePullUp  Indicates if the pin shall have its pull-up activated.
 */
static void pal_pio_setoutput(Pio *pio, uint32_t mask, uint8_t defaultValue,
                              uint8_t enableMultiDrive,
                              uint8_t enablePullUp)
{
    /* Enable pull-up(s) if necessary */
    if (enablePullUp)
    {
        pio->PIO_PUER = mask;
    }
    else
    {
        pio->PIO_PUDR = mask;
    }

    /* Enable multi-drive if necessary */
    if (enableMultiDrive)
    {
        pio->PIO_MDER = mask;
    }
    else
    {
        pio->PIO_MDDR = mask;
    }

    if (defaultValue)
    {
        pio->PIO_SODR = mask;
    }
    else
    {
        pio->PIO_CODR = mask;
    }

    /* Configure pin(s) as output(s) */
    pio->PIO_OER = mask;
    pio->PIO_PER = mask;
}


/**
 * @brief Check the Pin instance currently have a high or not
 *
 * This function returns 1 if one or more PIO of the given Pin instance
 * currently have a high level; otherwise returns 0. This method returns
 * the actual value that is being read on the pin.
 *
 * @param pin  Pointer to a Pin instance describing one or more pins.
 * @return 1 if the Pin instance contains at least one PIO that currently has
 *  a high level; otherwise 0.
 */
bool pal_pio_get(const pin_t *pin)
{
    uint32_t reg;
    /* Check the pin type is an output and get the Data register*/
    if ((pin->type == PIO_OUTPUT_0) || (pin->type == PIO_OUTPUT_1))
    {
        reg = pin->pio->PIO_ODSR;
    }
    else
    {
        reg = pin->pio->PIO_PDSR;
    }
    /* Check the level of the pin */
    if ((reg & pin->mask) == 0)
    {
        return 0;
    }
    else
    {
        return 1;
    }
}


/**
 * @brief Initializes the PIO interrupt management logic
 *
 * @param priority  PIO controller interrupts priority.
 */
void pal_pio_initialize_interrupts(uint32_t priority)
{
    /* Reset sources */
    numSources = 0;

    /* Configure PIO interrupt sources of PIOA */
    pal_peripheral_clock_enable(ID_PIOA);
    PIOA->PIO_ISR;
    PIOA->PIO_IDR = ALL_INTERRUPTS_MASK;
    NVIC_DisableIRQ(PIOA_IRQn);
    NVIC_ClearPendingIRQ(PIOA_IRQn);
    NVIC_SetPriority(PIOA_IRQn, priority);
    NVIC_EnableIRQ(PIOA_IRQn);

    /* Configure PIO interrupt sources of PIOB */
    pal_peripheral_clock_enable(ID_PIOB);
    PIOB->PIO_ISR;
    PIOB->PIO_IDR = ALL_INTERRUPTS_MASK;
    NVIC_DisableIRQ(PIOB_IRQn);
    NVIC_ClearPendingIRQ(PIOB_IRQn);
    NVIC_SetPriority(PIOB_IRQn, priority);
    NVIC_EnableIRQ(PIOB_IRQn);

    /* Configure PIO interrupt sources of PIOC */
    pal_peripheral_clock_enable(ID_PIOC);
    PIOC->PIO_ISR;
    PIOC->PIO_IDR = ALL_INTERRUPTS_MASK;
    NVIC_DisableIRQ(PIOC_IRQn);
    NVIC_ClearPendingIRQ(PIOC_IRQn);
    NVIC_SetPriority(PIOC_IRQn, priority);
    NVIC_EnableIRQ(PIOC_IRQn);
}


/**
 * @Configures a PIO interrupt
 *
 * This function configures a PIO or a group of PIO to generate an interrupt
 * on status change. The provided interrupt handler will be called with the
 * triggering pin as its parameter (enabling different pin instances
 * to share the same handler
 *
 * @param pPin  Pointer to a Pin instance.
 * @param handler  Interrupt handler function pointer.
 */
void pal_pio_configure_interrupt(const pin_t *pPin, void (*handler)(const pin_t *))
{
    uint8_t index;
    bool configured = false;
    Pio *pio = pPin->pio;
    interruptSource_t *pSource;

    /* Define new source */
    for (index = 0; index < numSources; index++)
    {
        if (pSources[index].pPin == pPin)
        {
            configured = true;
            pSources[index].pio_handler = handler;
        }
    }
    /* Check the pin already configured or not*/
    if (!configured)
    {
        pSource = &(pSources[numSources]);
        pSource->pPin = pPin;
        pSource->pio_handler = handler;
        numSources++;
    }
    /* PIO3 with additional interrupt support */
    /* Configure additional interrupt mode registers */
    if (pPin->attribute & PIO_IT_AIME)
    {
        /* Enable additional interrupt mode */
        pio->PIO_AIMER       = pPin->mask;

        /* if bit field of selected pin is 1, set as Rising Edge/High level
          detection event */
        if (pPin->attribute & PIO_IT_RE_OR_HL)
        {
            pio->PIO_REHLSR    = pPin->mask;
        }
        else
        {
            pio->PIO_FELLSR     = pPin->mask;
        }

        /* if bit field of selected pin is 1, set as edge detection source*/
        if (pPin->attribute & PIO_IT_EDGE)
        {
            pio->PIO_ESR     = pPin->mask;
        }
        else
        {
            pio->PIO_LSR     = pPin->mask;
        }
    }
    else
    {
        /* disable additional interrupt mode */
        pio->PIO_AIMDR       = pPin->mask;
    }
}


/**
 * @brief Disables a given interrupt source, with no added side effects.
 * @param pPin  Interrupt source to disable
 */
void pal_pio_disable_interrupt(const pin_t *pPin)
{
    pPin->pio->PIO_IDR = pPin->mask;
}


/**
 * @brief Enables the given interrupt source
 *
 * This function enables the given interrupt source if it has been configured.
 * The status register of the corresponding PIO controller is cleared prior
 * to enabling the interrupt
 *
 * @param pPin  Interrupt source to enable
 */
void pal_pio_enable_interrupt(const pin_t *pPin)
{
    pPin->pio->PIO_IER = pPin->mask;
}


/**
 * @brief Parallel IO Controller A interrupt handler
 */
void PIOA_Handler(void)
{
    pal_pio_interrupt_handler(ID_PIOA, PIOA);
}


/**
 * @brief Parallel IO Controller B interrupt handler
 */
void PIOB_Handler(void)
{
    pal_pio_interrupt_handler(ID_PIOB, PIOB);
}


/**
 * @brief Parallel IO Controller C interrupt handler
 */
void PIOC_Handler(void)
{
    pal_pio_interrupt_handler(ID_PIOC, PIOC);
}


/**
 * @brief Handles all interrupts on the given PIO controller.
 * @param id  PIO controller ID.
 * @param pPio  PIO controller base address.
 */
void pal_pio_interrupt_handler(uint32_t id, Pio *pPio)
{
    uint32_t status;
    uint32_t index;

    /* Read PIO controller status */
    status = pPio->PIO_ISR;
    status &= pPio->PIO_IMR;

    /* Check pending events */
    if (status != 0)
    {
        /* Find triggering source */
        index = 0;
        while (status != 0)
        {
            /* There cannot be an unconfigured source enabled */
            /* Source is configured on the same controller */
            if (pSources[index].pPin->id == id)
            {
                /* Source has PIOs whose status have changed */
                if ((status & pSources[index].pPin->mask) != 0)
                {
                    pSources[index].pio_handler(pSources[index].pPin);
                    status &= ~(pSources[index].pPin->mask);
                }
            }
            index++;
        }
    }
}


/**
 * @brief Initialize the watchdog timer
 */
#if defined(WATCHDOG)
static void pal_wdt_init(void)
{

    Wdt *pWDT = WDT;
    /* Initialize the Watchdog - Disabled in debug mode

                             period in milliseconds * 256
    watdog counter value =  ------------------------------  - 1
                             1000
    */
    pWDT->WDT_MR = (((WDT_TIMEOUT_PERIOD << 8) / 1000) - 1) |
                   WDT_MR_WDRSTEN |
                   WDT_MR_WDDBGHLT |
                   WDT_MR_WDD;

}

/**
 * @brief Resets the watchdog timer
 */
static void pal_wdt_reset(void)
{
    Wdt *pWDT = WDT;
    pWDT->WDT_CR = (uint32_t)(WDT_CR_WDRSTT | (WDT_CR_PASSWORD << 24));
}

#else /* #if defined(WATCHDOG) || defined(DOXYGEN) */

/**
 * @brief Disable watchdog.
 */
static void pal_wdt_disable(void)
{
    Wdt *pWDT = WDT;

    pWDT->WDT_MR = WDT_MR_WDDIS;
}
#endif
/* EOF */
