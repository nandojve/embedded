/**
 * @file tal_pib.c
 *
 * @brief This file handles the TAL PIB attributes, set/get and initialization
 *
 * $Id: tal_pib.c 32115 2012-05-18 11:23:47Z yogesh.bellan $
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

#include <stdint.h>
#include <string.h>
#include <stdbool.h>
#include "pal.h"
#include "return_val.h"
#include "tal.h"
#include "ieee_const.h"
#include "tal_constants.h"
#include "tal_pib.h"
#include "at86rf230b.h"
#include "tal_internal.h"

/* === TYPES =============================================================== */


/* === MACROS ============================================================== */

/*
 * Translation table converting register values to power levels (dBm).
 */
FLASH_DECLARE(int8_t tx_pwr_table[16]) =
{
    3, /* 3.2 */
    2, /* 2.6 */
    2, /* 2.1 */
    1, /* 1.6 */
    1, /* 1.1 */
    0, /* 0.5 */
    0, /* -0.2 */
    -1, /* -1.2 */
    -2, /* -2.2 */
    -3, /* -3.2 */
    -4, /* -4.2 */
    -5, /* -5.2 */
    -7, /* -7.2 */
    -9, /* -9.2 */
    -12, /* -12.2 */
    -17, /* -17.2 */
};

/* === GLOBALS ============================================================= */


/* === PROTOTYPES ========================================================== */

static uint8_t limit_tx_pwr(uint8_t tal_pib_TransmitPower);
static uint8_t convert_phyTransmitPower_to_reg_value(uint8_t phyTransmitPower_value);

/* === IMPLEMENTATION ====================================================== */

/**
 * @brief Initialize the TAL PIB
 *
 * This function initializes the TAL information base attributes
 * to their default values.
 */
void init_tal_pib(void)
{
    tal_pib.MaxCSMABackoffs = TAL_MAX_CSMA_BACKOFFS_DEFAULT;
    tal_pib.MinBE = TAL_MINBE_DEFAULT;
    tal_pib.PANId = TAL_PANID_BC_DEFAULT;
    tal_pib.ShortAddress = TAL_SHORT_ADDRESS_DEFAULT;
    tal_pib.CurrentChannel = TAL_CURRENT_CHANNEL_DEFAULT;
    tal_pib.SupportedChannels = TRX_SUPPORTED_CHANNELS;
    tal_pib.CurrentPage = TAL_CURRENT_PAGE_DEFAULT;
    tal_pib.MaxFrameDuration = TAL_MAX_FRAME_DURATION_DEFAULT;
    tal_pib.SHRDuration = TAL_SHR_DURATION_DEFAULT;
    tal_pib.SymbolsPerOctet = TAL_SYMBOLS_PER_OCTET_DEFAULT;
    tal_pib.MaxBE = TAL_MAXBE_DEFAULT;
    tal_pib.MaxFrameRetries = TAL_MAXFRAMERETRIES_DEFAULT;
    tal_pib.TransmitPower = limit_tx_pwr(TAL_TRANSMIT_POWER_DEFAULT);
    tal_pib.CCAMode = TAL_CCA_MODE_DEFAULT;
    tal_pib.PrivatePanCoordinator = TAL_PAN_COORDINATOR_DEFAULT;
#ifdef BEACON_SUPPORT
    tal_pib.BattLifeExt = TAL_BATTERY_LIFE_EXTENSION_DEFAULT;
    tal_pib.BeaconOrder = TAL_BEACON_ORDER_DEFAULT;
    tal_pib.SuperFrameOrder = TAL_SUPERFRAME_ORDER_DEFAULT;
#endif  /* BEACON_SUPPORT */

#ifdef PROMISCUOUS_MODE
    tal_pib.PromiscuousMode = TAL_PIB_PROMISCUOUS_MODE_DEFAULT;
#endif
}


/**
 * @brief Write all shadow PIB variables to the transceiver
 *
 * This function writes all shadow PIB variables to the transceiver.
 * It is assumed that the radio does not sleep.
 */
void write_all_tal_pib_to_trx(void)
{
    uint8_t *ptr_to_reg;

    /* configure RX_AACK */
    ptr_to_reg = (uint8_t *)&tal_pib.PANId;
    for (uint8_t i = 0; i < 2; i++)
    {
        pal_trx_reg_write((RG_PAN_ID_0 + i), *ptr_to_reg);
        ptr_to_reg++;
    }

    ptr_to_reg = (uint8_t *)&tal_pib.IeeeAddress;
    for (uint8_t i = 0; i < 8; i++)
    {
        pal_trx_reg_write((RG_IEEE_ADDR_0 + i), *ptr_to_reg);
        ptr_to_reg++;
    }

    ptr_to_reg = (uint8_t *)&tal_pib.ShortAddress;
    for (uint8_t i = 0; i < 2; i++)
    {
        pal_trx_reg_write((RG_SHORT_ADDR_0 + i), *ptr_to_reg);
        ptr_to_reg++;
    }

    /* configure TX_ARET; CSMA and CCA */
    {
        uint8_t reg_value;

        reg_value = convert_phyTransmitPower_to_reg_value(tal_pib.TransmitPower);
        pal_trx_bit_write(SR_TX_PWR, reg_value);
    }
    pal_trx_bit_write(SR_CCA_MODE, tal_pib.CCAMode);
    pal_trx_bit_write(SR_CHANNEL, tal_pib.CurrentChannel);
    pal_trx_bit_write(SR_MIN_BE, tal_pib.MinBE);

#ifdef PROMISCUOUS_MODE
    if (tal_pib.PromiscuousMode)
    {
        set_trx_state(CMD_RX_ON);
    }
#endif
}


/**
 * @brief Gets a TAL PIB attribute
 *
 * This function is called to retrieve the transceiver information base
 * attributes.
 *
 * @param[in] attribute TAL infobase attribute ID
 * @param[out] value TAL infobase attribute value
 *
 * @return MAC_UNSUPPORTED_ATTRIBUTE if the TAL infobase attribute is not found
 *         MAC_SUCCESS otherwise
 */
#if (HIGHEST_STACK_LAYER == TAL)
retval_t tal_pib_get(uint8_t attribute, uint8_t *value)
{
    switch (attribute)
    {
        case macMaxCSMABackoffs:
            *value = tal_pib.MaxCSMABackoffs;
            break;

        case macMinBE:
            *value = tal_pib.MinBE;
            break;

        case macPANId:
            *(uint16_t *)value = tal_pib.PANId;
            break;
#ifdef PROMISCUOUS_MODE
        case macPromiscuousMode:
            *(uint16_t *)value = tal_pib.PromiscuousMode;
            break;
#endif
        case macShortAddress:
            *(uint16_t *)value = tal_pib.ShortAddress;
            break;

        case phyCurrentChannel:
            *value = tal_pib.CurrentChannel;
            break;

        case phyChannelsSupported:
            *(uint32_t *)value = tal_pib.SupportedChannels;
            break;

        case phyTransmitPower:
            *value = tal_pib.TransmitPower;
            break;

        case phyCCAMode:
            *value = tal_pib.CCAMode;
            break;

        case phyCurrentPage:
            *value = tal_pib.CurrentPage;
            break;

        case phyMaxFrameDuration:
            *(uint16_t *)value = tal_pib.MaxFrameDuration;
            break;

        case phySymbolsPerOctet:
            *value = tal_pib.SymbolsPerOctet;
            break;

        case phySHRDuration:
            *value = tal_pib.SHRDuration;
            break;

        case macMaxBE:
            *value = tal_pib.MaxBE;
            break;

        case macMaxFrameRetries:
            *value = tal_pib.MaxFrameRetries;
            break;

        case macIeeeAddress:
            *(uint64_t *)value = tal_pib.IeeeAddress;
            break;
#ifdef BEACON_SUPPORT
        case macBattLifeExt:
            *(bool *)value = tal_pib.BattLifeExt;
            break;

        case macBeaconOrder:
            *value = tal_pib.BeaconOrder;
            break;

        case macSuperframeOrder:
            *value = tal_pib.SuperFrameOrder;
            break;

        case macBeaconTxTime:
            *(uint32_t *)value = tal_pib.BeaconTxTime;
            break;
#endif  /* BEACON_SUPPORT */
        case mac_i_pan_coordinator:
            *(bool *)value = tal_pib.PrivatePanCoordinator;
            break;

        case macAckWaitDuration:
            /* Rev.B does not support changing this value */
            return MAC_UNSUPPORTED_ATTRIBUTE;

        default:
            /* Invalid attribute id */
            return MAC_UNSUPPORTED_ATTRIBUTE;
    }

    return MAC_SUCCESS;
} /* tal_pib_get() */
#endif  /* (HIGHEST_STACK_LAYER != MAC) */


/**
 * @brief Sets a TAL PIB attribute
 *
 * This function is called to set the transceiver information base
 * attributes.
 *
 * @param attribute TAL infobase attribute ID
 * @param value TAL infobase attribute value to be set
 *
 * @return MAC_UNSUPPORTED_ATTRIBUTE if the TAL info base attribute is not found
 *         TAL_BUSY if the TAL is not in TAL_IDLE state. An exception is
 *         macBeaconTxTime which can be accepted by TAL even if TAL is not
 *         in TAL_IDLE state.
 *         MAC_SUCCESS if the attempt to set the PIB attribute was successful
 *         TAL_TRX_ASLEEP if trx is in SLEEP mode and access to trx is required
 */
retval_t tal_pib_set(uint8_t attribute, pib_value_t *value)
{
    /*
     * Do not allow any changes while ED or TX is done.
     * We allow changes during RX, but it's on the user's own risk.
     */
#if (MAC_SCAN_ED_REQUEST_CONFIRM == 1)
    if (tal_state == TAL_ED)
    {
        ASSERT("TAL is busy" == 0);
        return TAL_BUSY;
    }
#endif /* (MAC_SCAN_ED_REQUEST_CONFIRM == 1) */

    /*
     * Distinguish between PIBs that need to be changed in trx directly
     * and those that are simple variable udpates.
     * Ensure that the transceiver is not in SLEEP.
     * If it is in SLEEP, change it to TRX_OFF.
     * For all other state force TRX_OFF.
     * Abort any TAL_BUSY state, because the change might have an impact to
     * ongoing transactions.
     */

    switch (attribute)
    {
        case macMaxFrameRetries:
            /*
             * The new PIB value is not immediately written to the
             * transceiver. This is done on a frame-by-frame base.
             */
            tal_pib.MaxFrameRetries = value->pib_value_8bit;
            break;

        case macMaxCSMABackoffs:
            /*
             * The new PIB value is not immediately written to the
             * transceiver. This is done on a frame-by-frame base.
             */
            tal_pib.MaxCSMABackoffs = value->pib_value_8bit;
            break;

#ifdef BEACON_SUPPORT
        case macBattLifeExt:
            tal_pib.BattLifeExt = value->pib_value_bool;
            break;

        case macBeaconOrder:
            tal_pib.BeaconOrder = value->pib_value_8bit;
            break;

        case macSuperframeOrder:
            tal_pib.SuperFrameOrder = value->pib_value_8bit;
            break;

        case macBeaconTxTime:
            tal_pib.BeaconTxTime = value->pib_value_32bit;
            break;
#endif  /* BEACON_SUPPORT */
#ifdef PROMISCUOUS_MODE
        case macPromiscuousMode:
            tal_pib.PromiscuousMode = value->pib_value_8bit;
            if (tal_pib.PromiscuousMode)
            {
                tal_trx_wakeup();

                /* Check if receive buffer is available or queue is not full. */
                if (NULL == tal_rx_buffer)
                {
                    set_trx_state(CMD_PLL_ON);
                    tal_rx_on_required = true;
                }
                else
                {
                    set_trx_state(CMD_RX_ON);
                }
            }
            else
            {
                set_trx_state(CMD_TRX_OFF);
                tal_rx_on_required = false;
            }
            break;
#endif

        default:
            /*
             * Following PIBs require access to trx.
             * Therefore trx must be at least in TRX_OFF.
             */

            if (tal_trx_status == TRX_SLEEP)
            {
                /* While trx is in SLEEP, register cannot be accessed. */
                return TAL_TRX_ASLEEP;
            }

            switch (attribute)
            {
                case macMinBE:
                    tal_pib.MinBE = value->pib_value_8bit;

#ifndef REDUCED_PARAM_CHECK
                    /*
                     * macMinBE must not be larger than macMaxBE or calculation
                     * of macMaxFrameWaitTotalTime will fail.
                     */
                    if (tal_pib.MinBE > tal_pib.MaxBE)
                    {
                        tal_pib.MinBE = tal_pib.MaxBE;
                    }
#endif  /* REDUCED_PARAM_CHECK */

                    pal_trx_bit_write(SR_MIN_BE, tal_pib.MinBE);
                    break;

                case macPANId:
                    tal_pib.PANId = value->pib_value_16bit;
                    uint8_t *ptr_pan;
                    ptr_pan = (uint8_t *)&tal_pib.PANId;
                    for (uint8_t i = 0; i < 2; i++)
                    {
                        pal_trx_reg_write((RG_PAN_ID_0 + i), *ptr_pan);
                        ptr_pan++;
                    }
                    break;

                case macShortAddress:
                    tal_pib.ShortAddress = value->pib_value_16bit;
                    uint8_t *ptr_shrt;
                    ptr_shrt = (uint8_t *)&tal_pib.ShortAddress;
                    for (uint8_t i = 0; i < 2; i++)
                    {
                        pal_trx_reg_write((RG_SHORT_ADDR_0 + i), *ptr_shrt);
                        ptr_shrt++;
                    }
                    break;

                case phyCurrentChannel:
                    if (tal_state != TAL_IDLE)
                    {
                        return TAL_BUSY;
                    }
                    if ((uint32_t)TRX_SUPPORTED_CHANNELS & ((uint32_t)0x01 << value->pib_value_8bit))
                    {
                        tal_trx_status_t previous_trx_status = TRX_OFF;
                        /*
                         * Set trx to "soft" off avoiding that ongoing
                         * transaction (e.g. ACK) are interrupted.
                         */
                        if (tal_trx_status != TRX_OFF)
                        {
                            previous_trx_status = RX_AACK_ON;   /* any other than TRX_OFF state */
                            do
                            {
                                /* set TRX_OFF until it could be set;
                                 * trx might be busy */
                            }
                            while (set_trx_state(CMD_TRX_OFF) != TRX_OFF);
                        }
                        tal_pib.CurrentChannel = value->pib_value_8bit;
                        pal_trx_bit_write(SR_CHANNEL, tal_pib.CurrentChannel);
                        /* Re-store previous trx state */
                        if (previous_trx_status != TRX_OFF)
                        {
                            /* Set to default state */
                            set_trx_state(CMD_RX_AACK_ON);
                        }
                    }
                    else
                    {
                        return MAC_INVALID_PARAMETER;
                    }
                    break;

                case phyCurrentPage:


                    if (tal_state != TAL_IDLE)
                    {
                        return TAL_BUSY;
                    }
                    else
                    {
                        uint8_t page;

                        page = value->pib_value_8bit;
                        if (page != 0)
                        {
                            return MAC_INVALID_PARAMETER;
                        }
                    }
                    break;

                case macMaxBE:
                    tal_pib.MaxBE = value->pib_value_8bit;
#ifndef REDUCED_PARAM_CHECK
                    /*
                     * macMinBE must not be larger than macMaxBE or calculation
                     * of macMaxFrameWaitTotalTime will fail.
                     */
                    if (tal_pib.MaxBE < tal_pib.MinBE)
                    {
                        tal_pib.MinBE = tal_pib.MaxBE;
                    }
#endif  /* REDUCED_PARAM_CHECK */
                    break;

                case phyTransmitPower:
                    {
                        uint8_t reg_value;

                        tal_pib.TransmitPower = value->pib_value_8bit;

                        /* Limit tal_pib.TransmitPower to max/min trx values */
                        tal_pib.TransmitPower = limit_tx_pwr(tal_pib.TransmitPower);
                        reg_value = convert_phyTransmitPower_to_reg_value(tal_pib.TransmitPower);
                        pal_trx_bit_write(SR_TX_PWR, reg_value);
                    }
                    break;

                case phyCCAMode:
                    tal_pib.CCAMode = value->pib_value_8bit;
                    pal_trx_bit_write(SR_CCA_MODE, tal_pib.CCAMode);
                    break;

                case macIeeeAddress:
                    {
                        uint8_t *ptr;
                        tal_pib.IeeeAddress = value->pib_value_64bit;
                        ptr = (uint8_t *)&tal_pib.IeeeAddress;
                        for (uint8_t i = 0; i < 8; i++)
                        {
                            pal_trx_reg_write((RG_IEEE_ADDR_0 + i), *ptr);
                            ptr++;
                        }
                    }
                    break;

                case mac_i_pan_coordinator:
                    tal_pib.PrivatePanCoordinator = value->pib_value_bool;
                    pal_trx_bit_write(SR_I_AM_COORD, tal_pib.PrivatePanCoordinator);
                    break;

                case macAckWaitDuration:
                    /* AT86RF230B does not support changing this value */
                    return MAC_UNSUPPORTED_ATTRIBUTE;

                default:
                    return MAC_UNSUPPORTED_ATTRIBUTE;
            }

            break; /* end of 'default' from 'switch (attribute)' */
    }
    return MAC_SUCCESS;
} /* tal_pib_set() */


/**
 * @brief Limit the phyTransmitPower to the trx limits
 *
 * @param phyTransmitPower phyTransmitPower value
 *
 * @return limited tal_pib_TransmitPower
 */
static uint8_t limit_tx_pwr(uint8_t tal_pib_TransmitPower)
{
    uint8_t ret_val = tal_pib_TransmitPower;
    int8_t dbm_value;

    dbm_value = CONV_phyTransmitPower_TO_DBM(tal_pib_TransmitPower);
    if (dbm_value > (int8_t)PGM_READ_BYTE(&tx_pwr_table[0]))
    {
        dbm_value = (int8_t)PGM_READ_BYTE(&tx_pwr_table[0]);
        ret_val = CONV_DBM_TO_phyTransmitPower(dbm_value);

    }
    else if (dbm_value < (int8_t)PGM_READ_BYTE(&tx_pwr_table[sizeof(tx_pwr_table) - 1]))
    {
        dbm_value = (int8_t)PGM_READ_BYTE(&tx_pwr_table[sizeof(tx_pwr_table) - 1]);
        ret_val = CONV_DBM_TO_phyTransmitPower(dbm_value);
    }

    return (ret_val | TX_PWR_TOLERANCE);
}


/**
 * @brief Converts a phyTransmitPower value to a register value
 *
 * @param phyTransmitPower_value phyTransmitPower value
 *
 * @return register value
 */
static uint8_t convert_phyTransmitPower_to_reg_value(uint8_t phyTransmitPower_value)
{
    int8_t dbm_value;
    uint8_t i;
    int8_t trx_tx_level;

    dbm_value = CONV_phyTransmitPower_TO_DBM(phyTransmitPower_value);

    /* Compare to the register value to identify the value that matches. */
    for (i = 0; i < sizeof(tx_pwr_table); i++)
    {
        trx_tx_level = (int8_t)PGM_READ_BYTE(&tx_pwr_table[i]);
        if (trx_tx_level <= dbm_value)
        {
            if (trx_tx_level < dbm_value)
            {
                return (i - 1);
            }
            return i;
        }
    }

    /* This code should never be reached. */
    return 0;
}


/* EOF */
