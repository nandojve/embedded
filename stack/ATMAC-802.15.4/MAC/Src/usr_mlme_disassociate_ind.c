/**
 * @file usr_mlme_disassociate_ind.c
 *
 * @brief This file contains user call back function for
 * MLME-DISASSOCIATE.indication
 *
 * $Id: usr_mlme_disassociate_ind.c 20399 2010-02-18 08:10:29Z sschneid $
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

#if (!defined(USR_MLME_DISASSOCIATE_IND) || USR_MLME_DISASSOCIATE_IND > 2)
#error User must define USR_MLME_DISASSOCIATE_IND (0-Not compile, 1-Compile Stub, 2-User Defined Callback)
#elif (USR_MLME_DISASSOCIATE_IND == 1)

#if (MAC_DISASSOCIATION_BASIC_SUPPORT == 1)

/* === Macros ============================================================== */


/* === Globals ============================================================= */


/* === Prototypes ========================================================== */


/* === Implementation ====================================================== */

void usr_mlme_disassociate_ind(uint64_t DeviceAddress,
                               uint8_t DisassociateReason)
{
    /* Keep compiler happy. */
    DeviceAddress = DeviceAddress;
    DisassociateReason = DisassociateReason;
}

#endif  /* (MAC_DISASSOCIATION_BASIC_SUPPORT == 1) */

#endif /* (USR_MLME_DISASSOCIATE_IND == 1) */

/* EOF */
