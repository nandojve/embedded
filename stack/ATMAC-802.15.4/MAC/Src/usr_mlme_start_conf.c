/**
 * @file usr_mlme_start_conf.c
 *
 * @brief This file contains user call back function for MLME-START.confirm.
 *
 * $Id: usr_mlme_start_conf.c 20399 2010-02-18 08:10:29Z sschneid $
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

#if (!defined(USR_MLME_START_CONF) || USR_MLME_START_CONF > 2)
#error User must define USR_MLME_START_CONF (0-Not compile, 1-Compile Stub, 2-User Defined Callback)
#elif (USR_MLME_START_CONF == 1)

/* === Macros ============================================================== */

/* === Globals ============================================================= */

/* === Prototypes ========================================================== */

/* === Implementation ====================================================== */

void usr_mlme_start_conf(uint8_t status)
{
    status = status;    /* Keep compiler happy. */
}

#endif /* (USR_MLME_START_CONF == 1) */

/* EOF */
