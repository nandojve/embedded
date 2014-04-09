/**
 * @file mac_rx_enable.c
 *
 * @brief Implements the MLME-RX-ENABLE functionality.
 *
 * $Id: mac_rx_enable.c 30017 2012-01-04 03:05:10Z yogesh.bellan $
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

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include "pal.h"
#include "return_val.h"
#include "bmm.h"
#include "qmm.h"
#include "tal.h"
#include "ieee_const.h"
#include "mac_msg_const.h"
#include "mac_api.h"
#include "mac_msg_types.h"
#include "mac_data_structures.h"
#include "stack_config.h"
#include "mac_internal.h"
#include "mac.h"
#include "mac_config.h"
#include "mac_build_config.h"

#if (MAC_RX_ENABLE_SUPPORT == 1)

/* === Macros ============================================================== */


/* === Globals ============================================================= */

#ifdef BEACON_SUPPORT
/* Time when the receiver is switched off again in symbols. */
static uint32_t rx_off_time_symbols;
#endif  /* BEACON_SUPPORT */

/* === Prototypes ========================================================== */

static void mac_t_rx_off_cb(void *callback_parameter);
#ifdef BEACON_SUPPORT
static void mac_t_rx_on_cb(void *req_buffer);
#endif  /* BEACON_SUPPORT */
static uint8_t mac_rx_enable(void);
static void gen_rx_enable_conf(buffer_t *buf, uint8_t status);
static void handle_rx_on(uint32_t rx_on_duration_symbols, uint8_t *m);

/* === Implementation ====================================================== */

/**
 * @brief Implement the MLME-RX-ENABLE.request primitive.
 *
 * The MLME-RX-ENABLE.request primitive is generated by the next
 * higher layer and issued to MAC to enable the receiver for a
 * fixed duration, at a time relative to the start of the current or
 * next superframe on a beacon-enabled PAN or immediately on a
 * nonbeacon-enabled PAN. The receiver is enabled exactly once per
 * primitive request.
 *
 * @param m Pointer to the MLME-RX-ENABLE.request message
 */
void mlme_rx_enable_request(uint8_t *m)
{
    mlme_rx_enable_req_t *rxe;

    rxe = (mlme_rx_enable_req_t *)BMM_BUFFER_POINTER((buffer_t *)m);

    /* If RxOnDuration is zero, the receiver shall be disabled */
    if (0 == rxe->RxOnDuration)
    {
        /*
         * Turn the radio off. This is doney by calling the
         * same function as for the expiration of the Rx on timer.
         */
        mac_t_rx_off_cb(NULL);

        /* Send the confirm immediately. */
        gen_rx_enable_conf((buffer_t *)m, (uint8_t)MAC_SUCCESS);
        return;
    }

    /*
     * Reject the request when the MAC is currently in any of the
     * polling states or scanning.
     */
    if ((MAC_POLL_IDLE != mac_poll_state) ||
        (MAC_SCAN_IDLE != mac_scan_state)
       )
    {
        /* Send the confirm immediately. */
        gen_rx_enable_conf((buffer_t *)m, (uint8_t)MAC_TX_ACTIVE);
        return;
    }

#ifdef BEACON_SUPPORT
    if (NON_BEACON_NWK == tal_pib.BeaconOrder)
    {
        handle_rx_on(rxe->RxOnDuration, m);
    }
    else
    {
        /* We have a beacon-enabled network. */
        uint32_t curr_beacon_int_time_symbols = TAL_GET_BEACON_INTERVAL_TIME(tal_pib.BeaconOrder);
        uint32_t now_time_symbols;
        uint32_t symbols_since_beacon;
        uint32_t rx_on_time_symbols;
        retval_t timer_status;

        /*
         * Determine if (RxOnTime + RxOnDuration) is less than the beacon
         * interval.
         * According to 7.1.10.1.3:
         * On a beacon-enabled PAN, the MLME first determines whether
         * (RxOnTime + RxOnDuration) is less than the beacon interval, defined
         * by macBeaconOrder. If it is not less, the MLME issues the
         * MLME-RX-ENABLE.confirm primitive with a status of MAC_INVALID_PARAMETER.
         */

        rx_off_time_symbols = rxe->RxOnTime + rxe->RxOnDuration;

        if (rx_off_time_symbols >= curr_beacon_int_time_symbols)
        {
            /* Send the confirm immediately. */
            gen_rx_enable_conf((buffer_t *)m, (uint8_t)MAC_INVALID_PARAMETER);
            return;
        }

        pal_get_current_time(&now_time_symbols);
        now_time_symbols = TAL_CONVERT_US_TO_SYMBOLS(now_time_symbols);

        symbols_since_beacon  = tal_sub_time_symbols(now_time_symbols, tal_pib.BeaconTxTime);

        /*
         * Actually, MLME-RX-ENABLE.request in a beacon enabled PAN does
         * only make sense if the MAC is currently tracking beacons, so
         * that macBeaconTxTime is up to date. If it appears that
         * the last known macBeaconTxTime does not relate to the
         * current superframe, reject the request.
         */
        if (symbols_since_beacon > curr_beacon_int_time_symbols)
        {
            /* Send the confirm immediately. */
            gen_rx_enable_conf((buffer_t *)m, (uint8_t)MAC_INVALID_PARAMETER);
            return;
        }

        rx_on_time_symbols = tal_add_time_symbols(tal_pib.BeaconTxTime, rxe->RxOnTime);

        /* Check whether RxOnTime can still be handled in current CAP. */
        pal_get_current_time(&now_time_symbols);
        now_time_symbols = TAL_CONVERT_US_TO_SYMBOLS(now_time_symbols);

        if (tal_add_time_symbols(rx_on_time_symbols, TAL_CONVERT_US_TO_SYMBOLS(MIN_TIMEOUT))
            < now_time_symbols)
        {
            /* RxOnTime not possible within this CAP, see whether deferred
             * handling is allowed or not.. */
            if (!(rxe->DeferPermit))
            {
                gen_rx_enable_conf((buffer_t *)m, (uint8_t)MAC_PAST_TIME);
                return;
            }
            else
            {
                /*
                 * The MAC defers until the next superframe and attempts to enable
                 * the receiver in that superframe.
                 */
                rx_on_time_symbols = tal_add_time_symbols(rx_on_time_symbols,
                                                          curr_beacon_int_time_symbols);
            }
        }

        /*
         * Since the Rx-Enable timer could already be running,
         * it is stopped first, before it will be started (again).
         */
        pal_timer_stop(T_Rx_Enable);

        do
        {
            /*
             * Start a timer to turn Rx ON at the time "rxe->RxOnTime" from the start
             * of the next superframe.
             * Return value to be checked, because Rx on time could be too short
             * or in the past already.
             */
            timer_status =
                pal_timer_start(T_Rx_Enable,
                                TAL_CONVERT_SYMBOLS_TO_US(rx_on_time_symbols),
                                TIMEOUT_ABSOLUTE,
                                (FUNC_PTR)mac_t_rx_on_cb,
                                (void *)m);

            if (MAC_SUCCESS != timer_status)
            {
                rx_on_time_symbols = tal_add_time_symbols(rx_on_time_symbols,
                                                          curr_beacon_int_time_symbols);
            }
        }
        while (MAC_SUCCESS != timer_status);

        /* Remember the time to turn off the receiver. */
        rx_off_time_symbols = tal_add_time_symbols(rx_on_time_symbols, rxe->RxOnDuration);

        /* The remaining stuff will be done once the Rx On Timer expires. */
    }
#else   /* No BEACON_SUPPORT */
    handle_rx_on(rxe->RxOnDuration, m);
#endif  /* BEACON_SUPPORT / No BEACON_SUPPORT */
} /* mlme_rx_enable_request() */



/*
 * @brief Continues handling of MLME-RX-ENABLE.request by
 * waking radio up and setting it into rx on state.
 *
 * @param rxe_buff_ptr Pointer to the MLME-RX-ENABLE buffer allocated by the
 * NHLE.
 */
static uint8_t mac_rx_enable()
{
    uint8_t status;

    /* Wake up the radio first */
    mac_trx_wakeup();

    /* Turn the receiver on immediately. */
    status = tal_rx_enable(PHY_RX_ON);

    /* Rx is enabled */
    mac_rx_enabled = true;

    return (status);

} /* mac_rx_enable() */



#ifdef BEACON_SUPPORT
/*
 * @brief The RX "on" timer.
 *
 * This actually turns the radio receiver on - i.e. this is the
 * beginning of the PHY_RX_ON period.
 *
 * @param req_buffer Pointer to the MLME-RX-ENABLE.Request buffer
 * allocated by the NHLE.
 */
static void mac_t_rx_on_cb(void *req_buffer)
{
    handle_rx_on(rx_off_time_symbols, req_buffer);
}
#endif  /* BEACON_SUPPORT */



/*
 * @brief Set the transceiver state to PHY_TRX_OFF
 *
 * This actually turns the radio receiver off - i.e. this is the end
 * of the PHY_RX_ON period.
 *
 * @param callback_parameter Callback parameter
 */
static void mac_t_rx_off_cb(void *callback_parameter)
{
    uint8_t status;

    /*
     * Rx is disabled.
     * This will make sure that the radio will be put to sleep in function
     * mac_sleep_trans().
     */
    mac_rx_enabled = false;

    /*
     * In case macRxOnWhenIdle is not set, the radio is put to PHY_TRX_OFF
     * state, until the return status does not match the desired radio state,
     * i.e. PHY_TRX_OFF
     */
    if (!mac_pib.mac_RxOnWhenIdle)
    {
        /*
         * In case the radio is awake, we need to switch RX off.
         */
        if (RADIO_AWAKE == mac_radio_sleep_state)
        {
            status = tal_rx_enable(PHY_TRX_OFF);

            if (status != PHY_TRX_OFF)
            {
                /*
                 * The TAL is still busy and cannot set the TRX to OFF.
                 * In order to get progress this requires another
                 * round of the TAL task being processed.
                 * Therefore the MAC task needs to stopp here and pass
                 * controll back to the TAL.
                 * This is reached by starting the Rx-Enable timer again
                 * for a very short time with the same callback returning
                 * here very soon.
                 */
                pal_timer_start(T_Rx_Enable,
                                MIN_TIMEOUT,
                                TIMEOUT_RELATIVE,
                                (FUNC_PTR)mac_t_rx_off_cb,
                                NULL);

                /*
                 * Return now, since the TAL is still busy, so radio cannot go
                 * to sleep for now.
                 */
                return;
            }
            else
            {
                /* Set radio to sleep if allowed */
                mac_sleep_trans();
            }
        }
    }

    callback_parameter = callback_parameter;  /* Keep compiler happy. */
}



/*
 * @brief Internal function to handle immediate RX_ON
 *
 * This function immediately enables the receiver with the given
 * RxOnDuration time in symbols from now.
 *
 * @param rx_on_duration_symbols Duration in symbols that the reciever is
 *                               switched on.
 */
static void handle_rx_on(uint32_t rx_on_duration_symbols, uint8_t *m)
{
    retval_t timer_status;
    uint8_t rx_enable_status = mac_rx_enable();

    /*
     * TODO: Once it is possible to restart a timer even if it is
     * already running, this could be improved by simply calling
     * function pal_timer_start() without this previous check using
     * function pal_is_timer_running().
     */
    if (pal_is_timer_running(T_Rx_Enable))
    {
        /*
         * Rx-Enable timer is already running, so we need to stopp it first
         * before it will be started.
         */
        pal_timer_stop(T_Rx_Enable);
    }

    /*
     * Start timer for the Rx On duration of the radio being on
     * in order to switch it off later again.
     */
    timer_status =
        pal_timer_start(T_Rx_Enable,
                        TAL_CONVERT_SYMBOLS_TO_US(rx_on_duration_symbols),
                        TIMEOUT_RELATIVE,
                        (FUNC_PTR)mac_t_rx_off_cb,
                        NULL);

    ASSERT(MAC_SUCCESS == timer_status);

    /*
     * Send the confirm immediately depending on the status of
     * the timer start and the Rx Status
     */
    if (MAC_SUCCESS != timer_status)
    {
        gen_rx_enable_conf((buffer_t *)m, (uint8_t)MAC_INVALID_PARAMETER);
        /* Do house-keeeping and turn radio off. */
        mac_t_rx_off_cb(NULL);
    }
    else if (PHY_RX_ON != rx_enable_status)
    {
        gen_rx_enable_conf((buffer_t *)m, (uint8_t)MAC_TX_ACTIVE);
    }
    else
    {
        gen_rx_enable_conf((buffer_t *)m, (uint8_t)MAC_SUCCESS);
    }

    return;
}



/*
 * @brief Internal function to initiate rx enable confirm message.
 *
 * This function creates the rx enable confirm structure,
 * and appends it into internal event queue.
 *
 * @param buf Buffer for rx enable confirmation.
 * @param status Status of attempt to switch receiver on.
 */
static void gen_rx_enable_conf(buffer_t *buf, uint8_t status)
{
    mlme_rx_enable_conf_t *rec = (mlme_rx_enable_conf_t *)BMM_BUFFER_POINTER(buf);

    rec->cmdcode = MLME_RX_ENABLE_CONFIRM;
    rec->status = status;
    qmm_queue_append(&mac_nhle_q, buf);
}

#endif /* (MAC_RX_ENABLE_SUPPORT == 1) */

/* EOF */
