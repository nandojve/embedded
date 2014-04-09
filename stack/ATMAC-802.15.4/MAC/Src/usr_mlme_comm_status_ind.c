/**
 * @file usr_mlme_comm_status_ind.c
 *
 * @brief This file contains user call back function for
 * MLME-COMM-STATUS.indication.
 *
 * $Id: usr_mlme_comm_status_ind.c 20399 2010-02-18 08:10:29Z sschneid $
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

#if (!defined(USR_MLME_COMM_STATUS_IND) || USR_MLME_COMM_STATUS_IND > 2)
#error User must define USR_MLME_COMM_STATUS_IND (0-Not compile, 1-Compile Stub, 2-User Defined Callback)
#elif (USR_MLME_COMM_STATUS_IND == 1)

/* === Macros ============================================================== */

/* === Globals ============================================================= */

/* === Prototypes ========================================================== */

/* === Implementation ====================================================== */

void usr_mlme_comm_status_ind(wpan_addr_spec_t *SrcAddrSpec,
                              wpan_addr_spec_t *DstAddrSpec,
                              uint8_t status)
{
    /* Keep compiler happy. */
    SrcAddrSpec = SrcAddrSpec;
    DstAddrSpec = DstAddrSpec;
    status = status;
}

#endif /* (USR_MLME_COMM_STATUS_IND == 1) */

/* EOF */
