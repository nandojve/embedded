/**
 * @file usr_mlme_disassociate_conf.c
 *
 * @brief This file contains user call back function for
 * MLME-DISASSOCIATE.confirm
 *
 * $Id: usr_mlme_disassociate_conf.c 20399 2010-02-18 08:10:29Z sschneid $
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

#if (!defined(USR_MLME_DISASSOCIATE_CONF) || USR_MLME_DISASSOCIATE_CONF > 2)
#error User must define USR_MLME_DISASSOCIATE_CONF (0-Not compile, 1-Compile Stub, 2-User Defined Callback)
#elif (USR_MLME_DISASSOCIATE_CONF == 1)

#if (MAC_DISASSOCIATION_BASIC_SUPPORT == 1)

/* === Macros ============================================================== */

/* === Globals ============================================================= */

/* === Prototypes ========================================================== */

/* === Implementation ====================================================== */

void usr_mlme_disassociate_conf(uint8_t status,
                                wpan_addr_spec_t *DeviceAddrSpec)
{
    /* Keep compiler happy. */
    DeviceAddrSpec = DeviceAddrSpec;
    status = status;
}

#endif  /* (MAC_DISASSOCIATION_BASIC_SUPPORT == 1) */

#endif /* (USR_MLME_DISASSOCIATE_CONF == 1) */

/* EOF */
