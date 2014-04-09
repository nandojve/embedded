/**
 * @file pal_nvm_multi_write.h
 *
 * @brief API function for multiple NVM write access
 *
 * $Id: pal_nvm_multi_write.h 30145 2012-01-10 13:27:10Z yogesh.bellan $
 *
 * @author    Atmel Corporation: http://www.atmel.com
 * @author    Support email: avr@atmel.com
 */
/*
 * Copyright (c) 2011, Atmel Corporation All rights reserved.
 *
 * Licensed under Atmel's Limited License Agreement --> EULA.txt
 */

/* Prevent double inclusion */
#ifndef PAL_NVM_MULTI_WRITE_H
#define PAL__NVM_MULTI_WRITE_H

/* === Includes ============================================================= */

#include "pal.h"

/* === Prototypes =========================================================== */


/**
 * @brief Initializes NVM used for multiple write operations
 *
 * @param var_id  Identifier of the variable to be intialized
 *
 * @ingroup apiPalApi
 */
void pal_nvm_multi_init(uint8_t var_id);


/**
 * @brief Writes/Stores a 32bit value to the NVM used for multiple write operations
 *
 * @param var_id    Identifier of the variable to be stored
 * @param value     Pointer to 32bit value
 *
 * @ingroup apiPalApi
 */
void pal_nvm_multi_write32(uint8_t var_id, uint32_t *value);


/**
 * @brief Reads a 32bit value from the NVM used for multiple write operations
 *
 * The read function is used during warm start.
 *
 * @param var_id    Identifier of the variable to be read
 * @param value     Pointer to 32bit value to used for storing
 *
 * @ingroup apiPalApi
 */
void pal_nvm_multi_read32(uint8_t var_id, uint32_t **value);


/**
 * @brief Gets a 32bit value from the NVM used for multiple write operations
 *
 * The get function is used after warm start.
 *
 * @param var_id    Identifier of the variable to be read
 * @param value     Pointer to 32bit value to used for storing
 *
 * @ingroup apiPalApi
 */
void pal_nvm_multi_get32(uint8_t var_id, uint32_t **value);

#endif /* #define PAL__NVM_MULTI_WRITE_H */
/* EOF */
