/**
 * @file tal_ed.c
 *
 * @brief This file implements ED Scan
 *
 * $Id: tal_ed.c 29199 2011-11-09 14:11:27Z uwalter $
 *
 * @author    Atmel Corporation: http://www.atmel.com
 * @author    Support email: avr@atmel.com
 */
/*
 * Copyright (c) 2009, Atmel Corporation All rights reserved.
 *
 * Licensed under Atmel's Limited License Agreement --> EULA.txt
 */

/* === INCLUDES ============================================================ */

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include "pal.h"
#include "return_val.h"
#include "tal.h"
#include "ieee_const.h"
#include "tal_constants.h"
#include "atmega128rfa1.h"
#include "tal_internal.h"
#include "tal_irq_handler.h"
#include "mac_build_config.h"

#if (MAC_SCAN_ED_REQUEST_CONFIRM == 1)

/* === TYPES =============================================================== */


/* === MACROS ============================================================== */

/* Constant define for the ED scaling: register value at -35dBm */
#define CLIP_VALUE_REG                  (55)

/*
 * Scan duration formula: \f$aBaseSuperframeDuration (2^SD + 1)\f$
 * where \f$0 <= SD <= 14\f$
 */
#define CALCULATE_SYMBOL_TIME_SCAN_DURATION(SD) \
    (aBaseSuperframeDuration * ((1UL<<(SD))+1))

/* === GLOBALS ============================================================= */

/**
 * The peak_ed_level is the maximum ED value received from the transceiver for
 * the specified Scan Duration.
 */
static uint8_t max_ed_level;
static uint32_t sampler_counter;

/* === PROTOTYPES ========================================================== */

static void trx_ed_irq_handler_cb(void);

/* === IMPLEMENTATION ====================================================== */

/**
 * @brief Starts ED Scan
 *
 * This function starts an ED Scan for the scan duration specified by the
 * MAC layer.
 *
 * @param scan_duration Specifies the ED scan duration in symbols
 *
 * @return MAC_SUCCESS - ED scan duration timer started successfully
 *         TAL_BUSY - TAL is busy servicing the previous request from MAC
 *         TAL_TRX_ASLEEP - Transceiver is currently sleeping
 *         FAILURE otherwise
 */
retval_t tal_ed_start(uint8_t scan_duration)
{
    /*
     * Check if the TAL is in idle state. Only in idle state it can
     * accept and ED request from the MAC.
     */
    if (TAL_IDLE != tal_state)
    {
        if (tal_trx_status == TRX_SLEEP)
        {
            return TAL_TRX_ASLEEP;
        }
        else
        {
            ASSERT("TAL is TAL_BUSY" == 0);
            return TAL_BUSY;
        }
    }

    set_trx_state(CMD_FORCE_PLL_ON);
    pal_trx_bit_write(SR_RX_PDT_DIS, RX_DISABLE);
    pal_trx_irq_flag_clr_cca_ed();
    pal_trx_irq_init_cca_ed((FUNC_PTR)trx_ed_irq_handler_cb);
    pal_trx_reg_write(RG_IRQ_MASK, TRX_IRQ_CCA_ED_READY);

    /* Make sure that receiver is switched on. */
    if (set_trx_state(CMD_RX_ON) != RX_ON)
    {
        /* Restore previous configuration */
        pal_trx_bit_write(SR_RX_PDT_DIS, RX_ENABLE);
        pal_trx_reg_write(RG_IRQ_MASK, TRX_IRQ_DEFAULT);

        return FAILURE;
    }

    /* Perform ED in TAL_ED_RUNNING state. */
    tal_state = TAL_ED_RUNNING;

    max_ed_level = 0;   // reset max value

    sampler_counter = CALCULATE_SYMBOL_TIME_SCAN_DURATION(scan_duration) / ED_SAMPLE_DURATION_SYM;

    // write dummy value to start measurement
    pal_trx_reg_write(RG_PHY_ED_LEVEL, 0xFF);

    return MAC_SUCCESS;
}


/**
 * @brief ED Scan Interrupt
 *
 * This function handles an ED done interrupt from the transceiver.
 */
static void trx_ed_irq_handler_cb(void)
{
    uint8_t ed_value;

    /* Read the ED Value. */
    ed_value = pal_trx_reg_read(RG_PHY_ED_LEVEL);

    /*
     * Update the peak ED value received, if greater than the previously
     * read ED value.
     */
    if (ed_value > max_ed_level)
    {
        max_ed_level = ed_value;
    }

    /* Start next ED sampling */
    sampler_counter--;
    if (sampler_counter > 0)
    {
        // write dummy value to start measurement
        pal_trx_reg_write(RG_PHY_ED_LEVEL, 0xFF);
    }
    else
    {
        tal_state = TAL_ED_DONE;
    }
}


/**
 * @brief Scan done
 *
 * This function updates the max_ed_level and invokes the callback function
 * tal_ed_end_cb().
 *
 * @param parameter unused callback parameter
 */
void ed_scan_done(void)
{
    /* Restore previous configuration */
    pal_trx_bit_write(SR_RX_PDT_DIS, RX_ENABLE);
    pal_trx_reg_write(RG_IRQ_MASK, TRX_IRQ_DEFAULT);

    tal_state = TAL_IDLE;   // ed scan is done
    set_trx_state(CMD_RX_AACK_ON);

#ifndef TRX_REG_RAW_VALUE
    /*
     * Scale ED result.
     * Clip values to 0xFF if > -35dBm
     */
    if (max_ed_level > CLIP_VALUE_REG)
    {
        max_ed_level = 0xFF;
    }
    else
    {
        max_ed_level = (uint8_t)(((uint16_t)max_ed_level * 0xFF) / CLIP_VALUE_REG);
    }
#endif
    tal_ed_end_cb(max_ed_level);
}

#endif /* (MAC_SCAN_ED_REQUEST_CONFIRM == 1) */

/* EOF */
