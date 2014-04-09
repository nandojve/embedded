/**
 * @file usr_mcps_data_conf.c
 *
 * @brief This file contains user call back function for MCPS-DATA.confirm.
 *
 * $Id: usr_mcps_data_conf.c 21621 2010-04-15 08:09:37Z sschneid $
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

#if (!defined(USR_MCPS_DATA_CONF) || USR_MCPS_DATA_CONF > 2)
#error User must define USR_MCPS_DATA_CONF (0-Not compile, 1-Compile Stub, 2-User Defined Callback)
#elif (USR_MCPS_DATA_CONF == 1)

/* === Macros ============================================================== */


/* === Globals ============================================================= */


/* === Prototypes ========================================================== */


/* === Implementation ====================================================== */
#ifdef ENABLE_TSTAMP
void usr_mcps_data_conf(uint8_t msduHandle, uint8_t status, uint32_t Timestamp)
#else
void usr_mcps_data_conf(uint8_t msduHandle, uint8_t status)
#endif  /* ENABLE_TSTAMP */
{
    /* Keep compiler happy. */
    msduHandle = msduHandle;
    status = status;
#ifdef ENABLE_TSTAMP
    Timestamp = Timestamp;
#endif  /* ENABLE_TSTAMP */
}

#endif /* (USR_MCPS_DATA_CONF == 1) */

/* EOF */
