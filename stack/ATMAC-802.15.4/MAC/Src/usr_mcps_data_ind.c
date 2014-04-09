/**
 * @file usr_mcps_data_ind.c
 *
 * @brief This file contains user call back function for MCPS-DATA.indication.
 *
 * $Id: usr_mcps_data_ind.c 26610 2011-05-11 08:47:45Z sschneid $
 *
 * @author    Atmel Corporation: http://www.atmel.com
 * @author    Support email: avr@atmel.com
 */
/*
 * Copyright (c) 2009, Atmel Corporation All rights reserved.
 *
 * Licensed under Atmel's Limited License Agreement --> EULA.txt
 */

/* === Includes ============================================================= */

#include <stdint.h>
#include <stdbool.h>
#include "mac_api.h"

#if (!defined(USR_MCPS_DATA_IND) || USR_MCPS_DATA_IND > 2)
#error User must define USR_MCPS_DATA_IND (0-Not compile, 1-Compile Stub, 2-User Defined Callback)
#elif (USR_MCPS_DATA_IND == 1)

/* === Macros ============================================================== */


/* === Globals ============================================================= */


/* === Prototypes ========================================================== */


/* === Implementation ====================================================== */
#ifdef MAC_SECURITY_ZIP
void usr_mcps_data_ind(wpan_addr_spec_t *SrcAddrSpec,
                       wpan_addr_spec_t *DstAddrSpec,
                       uint8_t msduLength,
                       uint8_t *msdu,
                       uint8_t mpduLinkQuality,
                       uint8_t DSN,
    #ifdef ENABLE_TSTAMP
                       uint32_t Timestamp,
    #endif  /* ENABLE_TSTAMP */
                       uint8_t SecurityLevel,
                       uint8_t KeyIdMode,
                       uint8_t KeyIndex)
#else   /* No MAC_SECURITY */
void usr_mcps_data_ind(wpan_addr_spec_t *SrcAddrSpec,
                       wpan_addr_spec_t *DstAddrSpec,
                       uint8_t msduLength,
                       uint8_t *msdu,
                       uint8_t mpduLinkQuality,
    #ifdef ENABLE_TSTAMP
                       uint8_t DSN,
                       uint32_t Timestamp)
    #else
                       uint8_t DSN)
    #endif  /* ENABLE_TSTAMP */
#endif  /* MAC_SECURITY */
{
    /* Keep compiler happy. */
    SrcAddrSpec = SrcAddrSpec;
    DstAddrSpec = DstAddrSpec;
    msduLength = msduLength;
    msdu = msdu;
    mpduLinkQuality = mpduLinkQuality;
    DSN = DSN;
#ifdef ENABLE_TSTAMP
    Timestamp = Timestamp;
#endif  /* ENABLE_TSTAMP */
#ifdef MAC_SECURITY_ZIP
    SecurityLevel = SecurityLevel;
    KeyIdMode = KeyIdMode;
    KeyIndex = KeyIndex;
#endif  /* MAC_SECURITY */
}

#endif /* (USR_MCPS_DATA_IND == 1) */

/* EOF */
