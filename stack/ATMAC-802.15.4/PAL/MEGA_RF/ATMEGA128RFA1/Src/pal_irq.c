/**
 * @file pal_irq.c
 *
 * @brief PAL IRQ functionality
 *
 * This file contains functions to initialize, enable, disable and install
 * handler for the transceiver interrupts. It also contains functions to enable,
 * disable and get the status of global interrupt
 *
 * $Id: pal_irq.c 29716 2011-12-08 17:11:01Z uwalter $
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
#include "pal.h"

#if (PAL_TYPE == ATMEGA128RFA1)

/* === Types =============================================================== */


/* === Externals =========================================================== */

#ifdef WATCHDOG_TIMER
#include "pal_timer.h"
extern volatile bool timer_trigger;
extern timer_info_t timer_array[TOTAL_NUMBER_OF_TIMERS];
extern uint8_t running_timers;
#endif

/* === Globals ============================================================= */

/*
 * Function pointers to store the callback function of
 * the transceiver interrupt
 */
/** Function pointer to store callback for transceiver TX_END interrupt. */
static irq_handler_t irq_hdl_trx_tx_end;
/** Function pointer to store callback for transceiver RX_END interrupt. */
static irq_handler_t irq_hdl_trx_rx_end;
/** Function pointer to store callback for transceiver CCA_ED interrupt. */
static irq_handler_t irq_hdl_trx_cca_ed;

#if (defined BEACON_SUPPORT) || (defined ENABLE_TSTAMP) || (defined DOXYGEN)
/** Function pointer to store callback for transceiver timestamp (RX_START) interrupt. */
static irq_handler_t irq_hdl_trx_tstamp;
#endif

#if (defined ENABLE_ALL_TRX_IRQS) || (defined DOXYGEN)
/** Function pointer to store callback for transceiver AWAKE interrupt. */
static irq_handler_t irq_hdl_trx_awake;
/** Function pointer to store callback for transceiver AMI interrupt. */
static irq_handler_t irq_hdl_trx_ami;
/** Function pointer to store callback for transceiver BATMON interrupt. */
static irq_handler_t irq_hdl_trx_batmon;
/** Function pointer to store callback for transceiver PLL_LOCK interrupt. */
static irq_handler_t irq_hdl_trx_pll_lock;
/** Function pointer to store callback for transceiver PLL_UNLOCK interrupt. */
static irq_handler_t irq_hdl_trx_pll_unlock;
/** Function pointer to store callback for transceiver AES_READY interrupt. */
static irq_handler_t irq_hdl_trx_aes_ready;
#endif  /* ENABLE_ALL_TRX_IRQS */

/* === Prototypes ========================================================== */


/* === Implementation ====================================================== */

/**
 * @brief Initializes the transceiver TX END interrupt
 *
 * This function sets the microcontroller specific registers
 * responsible for handling the transceiver TX END interrupt
 *
 * @param trx_irq_cb Callback function for the transceiver TX END interrupt
 */
void pal_trx_irq_init_tx_end(FUNC_PTR trx_irq_cb)
{
    irq_hdl_trx_tx_end = (irq_handler_t)trx_irq_cb;
}


/**
 * @brief Initializes the transceiver RX END interrupt
 *
 * This function sets the microcontroller specific registers
 * responsible for handling the transceiver RX END interrupt
 *
 * @param trx_irq_cb Callback function for the transceiver RX END interrupt
 */
void pal_trx_irq_init_rx_end(FUNC_PTR trx_irq_cb)
{
    irq_hdl_trx_rx_end = (irq_handler_t)trx_irq_cb;
}


/**
 * @brief Initializes the transceiver CCA ED END interrupt
 *
 * This function sets the microcontroller specific registers
 * responsible for handling the transceiver CCA ED END interrupt
 *
 * @param trx_irq_cb Callback function for the transceiver CCA ED END interrupt
 */
void pal_trx_irq_init_cca_ed(FUNC_PTR trx_irq_cb)
{
    irq_hdl_trx_cca_ed = (irq_handler_t)trx_irq_cb;
}


#if (defined BEACON_SUPPORT) || (defined ENABLE_TSTAMP) || (defined DOXYGEN)
/**
 * @brief Initializes the transceiver timestamp interrupt (RX START interrupt)
 *
 * This function sets the microcontroller specific registers
 * responsible for handling the transceiver timestamp interrupt (RX START interrupt)
 *
 * @param trx_irq_cb Callback function for the transceiver
 * timestamp interrupt (RX START interrupt)
 */
void pal_trx_irq_init_tstamp(FUNC_PTR trx_irq_cb)
{
    irq_hdl_trx_tstamp = (irq_handler_t)trx_irq_cb;
}
#endif  /* #if (defined BEACON_SUPPORT) || (defined ENABLE_TSTAMP) || (defined DOXYGEN) */


#if defined(ENABLE_ALL_TRX_IRQS) || defined(DOXYGEN)
/**
 * @brief Initializes the transceiver AWAKE interrupt
 *
 * This function sets the microcontroller specific registers
 * responsible for handling the transceiver AWAKE interrupt
 *
 * @param trx_irq_cb Callback function for the transceiver AWAKE interrupt
 */
void pal_trx_irq_init_awake(FUNC_PTR trx_irq_cb)
{
    irq_hdl_trx_awake = (irq_handler_t)trx_irq_cb;
}


/**
 * @brief Initializes the transceiver AMI interrupt
 *
 * This function sets the microcontroller specific registers
 * responsible for handling the transceiver AMI interrupt
 *
 * @param trx_irq_cb Callback function for the transceiver AMI interrupt
 */
void pal_trx_irq_init_ami(FUNC_PTR trx_irq_cb)
{
    irq_hdl_trx_ami = (irq_handler_t)trx_irq_cb;
}


/**
 * @brief Initializes the transceiver BATMON interrupt
 *
 * This function sets the microcontroller specific registers
 * responsible for handling the transceiver BATMON interrupt
 *
 * @param trx_irq_cb Callback function for the transceiver BATMON interrupt
 */
void pal_trx_irq_init_batmon(FUNC_PTR trx_irq_cb)
{
    irq_hdl_trx_batmon = (irq_handler_t)trx_irq_cb;
}


/**
 * @brief Initializes the transceiver PLL_LOCK interrupt
 *
 * This function sets the microcontroller specific registers
 * responsible for handling the transceiver PLL_LOCK interrupt
 *
 * @param trx_irq_cb Callback function for the transceiver PLL_LOCK interrupt
 */
void pal_trx_irq_init_pll_lock(FUNC_PTR trx_irq_cb)
{
    irq_hdl_trx_pll_lock = (irq_handler_t)trx_irq_cb;
}


/**
 * @brief Initializes the transceiver PLL_UNLOCK interrupt
 *
 * This function sets the microcontroller specific registers
 * responsible for handling the transceiver PLL_UNLOCK interrupt
 *
 * @param trx_irq_cb Callback function for the transceiver PLL_UNLOCK interrupt
 */
void pal_trx_irq_init_pll_unlock(FUNC_PTR trx_irq_cb)
{
    irq_hdl_trx_pll_unlock = (irq_handler_t)trx_irq_cb;
}


/**
 * @brief Initializes the transceiver AES_READY interrupt
 *
 * This function sets the microcontroller specific registers
 * responsible for handling the transceiver AES_READY interrupt
 *
 * @param trx_irq_cb Callback function for the transceiver AES_READY interrupt
 */
void pal_trx_irq_init_aes_ready(FUNC_PTR trx_irq_cb)
{
    irq_hdl_trx_aes_ready = (irq_handler_t)trx_irq_cb;
}
#endif  /* ENABLE_ALL_TRX_IRQS */


/**
 * @brief ISR for transceiver's transmit end interrupt
 */
ISR(TRX24_TX_END_vect)
{
    irq_hdl_trx_tx_end();
}

/**
 * @brief ISR for transceiver's receive end interrupt
 */
ISR(TRX24_RX_END_vect)
{
    irq_hdl_trx_rx_end();
}

/**
 * @brief ISR for transceiver's CCA/ED measurement done interrupt
 */
ISR(TRX24_CCA_ED_DONE_vect)
{
    irq_hdl_trx_cca_ed();
}

/**
 * @brief ISR for transceiver's Awake interrupt
 */
ISR(TRX24_AWAKE_vect)
{
#ifdef ENABLE_ALL_TRX_IRQS
    irq_hdl_trx_awake();
#endif
}

/**
 * @brief ISR for transceiver's rx start interrupt
 *
 * By the time the SFD is detected, the hardware timestamps the
 * current frame in the SCTSR register.
 */
ISR(TRX24_RX_START_vect)
{
#if (defined BEACON_SUPPORT) || (defined ENABLE_TSTAMP)
    irq_hdl_trx_tstamp();
#endif  /* #if (defined BEACON_SUPPORT) || (defined ENABLE_TSTAMP) */
}

/**
 * @brief ISR for transceiver's PLL lock interrupt
 */
ISR(TRX24_PLL_LOCK_vect)
{
#ifdef ENABLE_ALL_TRX_IRQS
    irq_hdl_trx_pll_lock();
#endif  /*  ENABLE_ALL_TRX_IRQS*/
}

/**
 * @brief ISR for transceiver's PLL unlock interrupt
 */
ISR(TRX24_PLL_UNLOCK_vect)
{
#ifdef ENABLE_ALL_TRX_IRQS
    irq_hdl_trx_pll_unlock();
#endif  /*  ENABLE_ALL_TRX_IRQS*/
}

/**
 * @brief ISR for transceiver's address match interrupt
 */
ISR(TRX24_XAH_AMI_vect)
{
#ifdef ENABLE_ALL_TRX_IRQS
    irq_hdl_trx_ami();
#endif  /*  ENABLE_ALL_TRX_IRQS*/
}

/**
 * @brief ISR for transceiver's battery low interrupt
 */
ISR(BAT_LOW_vect)
{
#ifdef ENABLE_ALL_TRX_IRQS
    irq_hdl_trx_batmon();
#endif  /*  ENABLE_ALL_TRX_IRQS*/
}

/**
 * @brief ISR for transceiver's AES interrupt
 */
ISR(AES_READY_vect)
{
#ifdef ENABLE_ALL_TRX_IRQS
    irq_hdl_trx_aes_ready();
#endif  /*  ENABLE_ALL_TRX_IRQS*/
}


#if defined(WATCHDOG_TIMER) || defined(DOXYGEN)
/**
 * @brief ISR for Watchdog Timer Interrupt
 */
ISR(WDT_vect)
{
    /* Disable Watchdog Interrupt Mode */
    WDTCSR &= ~(1 << WDIE);

#ifdef NO_32KHZ_CRYSTAL
    /* Wakeup the trx to enable clock source for symbol counter. */
    PAL_SLP_TR_LOW();

    /* Adjust system time */
    uint32_t cnt_val = SC_READ32(SCCNT) & MASK_LOW28;
    cnt_val += WDT_WAKEUP_INTERVAL_US / PAL_US_PER_SYMBOLS;
    cnt_val &= MASK_LOW28;
    SC_WRITE32(SCCNT, cnt_val);

    /* Check for any pending timer event. */
    uint32_t now;
    pal_get_current_time(&now);

    /* Check for any pending timer event */
    if (running_timers > 0)
    {
        for (uint8_t i = 0; i < TOTAL_NUMBER_OF_TIMERS; i++)
        {
            if (timer_array[i].timer_cb != NULL)
            {
                if (now >= timer_array[i].abs_exp_timer)
                {
                    /* Trigger timer event handling */
                    timer_trigger = true;
                    break;
                }
            }
        }
    }

    /* Wait until 16MHz oscillator is ready. */
    while (pal_trx_bit_read(SR_TRX_STATUS) != TRX_OFF)
    {
        /* wait */
        /* Reduce CPU clock speed (1 MHz) to consume less power during wait period. */
        clock_prescale_set(clock_div_8);
    }
    /* Restore to 16MHz CPU clock speed. */
#if (F_CPU == 16000000)
    clock_prescale_set(0x0F);
#endif
#if (F_CPU == 8000000)
    clock_prescale_set(clock_div_1);
#endif
#if (F_CPU == 4000000)
    clock_prescale_set(clock_div_2);
#endif

    /* Toggle AS2 to trigger internal 32kHz crystal oscillator output. */
    ASSR = _BV(AS2);
    ASSR &= ~_BV(AS2);
#endif
}
#endif


#endif /* (PAL_TYPE == ATMEGA128RFA1) */

/* EOF */
