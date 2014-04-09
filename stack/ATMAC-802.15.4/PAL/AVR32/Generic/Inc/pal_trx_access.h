/**
 * @file pal_trx_access.h
 *
 * @brief PAL trx access internal functions prototypes for AVR32 MCUs
 *
 * This is the header file for the trx access for AVR32 MCUs.
 *
 * $Id: pal_trx_access.h 25330 2011-02-03 06:48:22Z yogesh.bellan $
 *
 * @author    Atmel Corporation: http://www.atmel.com
 * @author    Support email: avr@atmel.com
 */
/*
 * Copyright (c) 2010, Atmel Corporation All rights reserved.
 *
 * Licensed under Atmel's Limited License Agreement --> EULA.txt
 */


/* Prevent double inclusion */
#ifndef PAL_TRX_ACCESS_H
#define PAL_TRX_ACCESS_H

/* === Includes ============================================================= */

/* === Types ================================================================ */

/* === Externals ============================================================ */

/* === Macros ================================================================ */

/* === Prototypes =========================================================== */

#ifdef __cplusplus
extern "C" {
#endif

/* To initialize the transceiver interface */
void trx_interface_init(void);

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif  /* PAL_TRX_ACCESS_H */
/* EOF */
