/**
 * @file mac_security.h
 *
 * @brief Declares MAC security related functions, globals, and macros.
 *
 * $Id: mac_security.h 26610 2011-05-11 08:47:45Z sschneid $
 *
 * @author    Atmel Corporation: http://www.atmel.com
 * @author    Support email: avr@atmel.com
 */
/*
 * Copyright (c) 2009, Atmel Corporation All rights reserved.
 *
 * Licensed under Atmel's Limited License Agreement --> EULA.txt
 */

/* Prevent double inclusion */
#ifndef MAC_SECURITY_H
#define MAC_SECURITY_H

#ifdef MAC_SECURITY_ZIP

/* === Includes ============================================================= */

#include "mac_api.h"

/* === Macros =============================================================== */

/**
 * Default value for PIB macKeyTableEntries
 */
#define macKeyTableEntries_def              (0)

/**
 * Default value for PIB macDeviceTableEntries
 */
#define macDeviceTable_def                  (0)

/**
 * Default value for PIB macSecurityLevelTableEntries
 */
#define macSecurityLevelTable_def           (0)

/**
 * Default value for PIB macFrameCounter
 */
#define macFrameCounter_def                 (0x00000000)

/**
 * Default value for PIB macDefaultKeySource
 */
#define macDefaultKeySource_def             {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF}

/**
 * Default value for KeyIdLookupListEntries
 */
#define KeyIdLookupListEntries_def          (0)

/**
 * Default value for KeyDeviceListEntries
 */
#define KeyDeviceListEntries_def            (0)

/**
 * Default value for KeyUsageListEntries
 */
#define KeyUsageListEntries_def             (0)

/* === Externals ============================================================ */

extern mac_sec_pib_t mac_sec_pib;

/* === Prototypes =========================================================== */

#ifdef __cplusplus
extern "C" {
#endif
/*@{*/



/*@}*/

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif  /* MAC_SECURITY_ZIP */

#endif /* MAC_SECURITY_H */
/* EOF */
