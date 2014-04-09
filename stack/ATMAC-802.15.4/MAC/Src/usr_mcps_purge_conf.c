/**
 * @file usr_mcps_purge_conf.c
 *
 * @brief This file contains user call back function for MCPS-PURGE.confirm.
 *
 * $Id: usr_mcps_purge_conf.c 20399 2010-02-18 08:10:29Z sschneid $
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

#if (!defined(USR_MCPS_PURGE_CONF) || USR_MCPS_PURGE_CONF > 2)
#error User must define USR_MCPS_PURGE_CONF (0-Not compile, 1-Compile Stub, 2-User Defined Callback)
#elif (USR_MCPS_PURGE_CONF == 1)

#if ((MAC_PURGE_REQUEST_CONFIRM == 1) && (MAC_INDIRECT_DATA_BASIC == 1))

/* === Macros ============================================================== */


/* === Globals ============================================================= */


/* === Prototypes ========================================================== */


/* === Implementation ====================================================== */

void usr_mcps_purge_conf(uint8_t msduHandle, uint8_t status)
{
    /* Keep compiler happy. */
    msduHandle = msduHandle;
    status = status;
}

#endif /* ((MAC_PURGE_REQUEST_CONFIRM == 1) && (MAC_INDIRECT_DATA_BASIC == 1)) */

#endif /* (USR_MCPS_PURGE_CONF == 1) */

/* EOF */
