/**
 * @file pal_nvm_multi_write.c
 *
 * @brief Implementation of the multiple NVM write access
 *
 * $Id: pal_nvm_multi_write.c 31351 2012-03-20 05:26:45Z yogesh.bellan $
 *
 * @author    Atmel Corporation: http://www.atmel.com
 * @author    Support email: avr@atmel.com
 */
/*
 * Copyright (c) 2011, Atmel Corporation All rights reserved.
 *
 * Licensed under Atmel's Limited License Agreement --> EULA.txt
 */

#ifdef NVM_MULTI_WRITE

/* === Includes ============================================================= */

#include "pal_nvm_multi_write.h"
#include "pal_flash.h"
#include "pal_config.h"

#ifdef BOOT_FLASH
#include "pal_boot_flash.h"
#endif

/* === Macros =============================================================== */

/* === Globals ============================================================= */

static uint32_t var_addr[NVM_NUM_VARS];

/* === Prototypes =========================================================== */

static void nvm_var_clear(uint8_t var_id);
static bool get_current_location(uint8_t var_id, uint32_t *addr);
uint32_t flash_address_check(uint32_t addr);

/* === Implementation ======================================================= */

/**
 * @brief Initializes NVM used for multiple write operations
 *
 * @param var_id  Identifier of the variable to be intialized
 *
 * @ingroup apiPalApi
 */
void pal_nvm_multi_init(uint8_t var_id)
{
    nvm_var_clear(var_id);   // Erase/Clear entire flash page
    uint32_t value = 0x00000000;    // Set the first value to 0x00000000

    // Write first value to page buffer
    var_addr[var_id] = NVM_MULTI_WRITE_START + (var_id * NVM_MULTI_WRITE_NUM_PG_PER_VAR * SPM_PAGESIZE);

    // Write page buffer to actual flash page
    flash_memcpy((volatile void *)var_addr[var_id], (const void *)&value, sizeof(uint32_t), true);

}

/**
 * @brief Writes/Stores a 32bit value to the NVM used for multiple write operations
 *
 * @param var_id    Identifier of the variable to be stored
 * @param value     Pointer to 32bit value
 *
 * @ingroup apiPalApi
 */
void pal_nvm_multi_write32(uint8_t var_id, uint32_t *value)
{
    bool clear = false;

    // Check if current flash address is the last one of the flash section used for this variable.
    if (var_addr[var_id] == (NVM_MULTI_WRITE_START +
                             ((var_id + 1) * NVM_MULTI_WRITE_NUM_PG_PER_VAR * SPM_PAGESIZE) - sizeof(uint32_t)))
    {
        // Entire flash area for this variable is in use; clear flash pages used for this variable
        clear = true;
    }
    else
    {
        // Assign next storage location
        var_addr[var_id] += sizeof(uint32_t);
    }

    if (clear)
    {
        nvm_var_clear(var_id);
        var_addr[var_id] = NVM_MULTI_WRITE_START + (var_id * NVM_MULTI_WRITE_NUM_PG_PER_VAR * SPM_PAGESIZE);
    }

    // Write page buffer to actual flash page
    flash_memcpy((volatile void *)var_addr[var_id], (const void *)value, sizeof(uint32_t), true);
}


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
void pal_nvm_multi_read32(uint8_t var_id, uint32_t **value)
{
    uint32_t addr;

    if (get_current_location(var_id, &addr) == false)
    {
        // No empty area could be found; use the last entry of the last page
        addr = NVM_MULTI_WRITE_START + ((var_id + 1) * NVM_MULTI_WRITE_NUM_PG_PER_VAR * SPM_PAGESIZE) - sizeof(uint32_t);
    }

    var_addr[var_id] = addr;

    // Read 32 bit value
    uint8_t *ptr;
    ptr = (uint8_t *)value;
    uint8_t *index;
    for (uint8_t i = 0; i < sizeof(uint32_t); i++)
    {
        index = (uint8_t *)addr;
        *ptr = *index;
        ptr++;
        addr++;
    }
}

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
void pal_nvm_multi_get32(uint8_t var_id, uint32_t **value)
{
    uint32_t addr = var_addr[var_id];
    // Read 32 bit value
    uint8_t *ptr;
    ptr = (uint8_t *)value;
    uint8_t *index;
    for (uint8_t i = 0; i < sizeof(uint32_t); i++)
    {
        index = (uint8_t *)addr;
        *ptr = *index;
        ptr++;
        addr++;
    }
}


/**
 * @brief To get he page number of the flash
 *
 *
 * @param addr    Address of the flash in order to obtain the flah page
 *
 * @return Page number of the flash
 */
uint32_t flash_address_check(uint32_t addr)
{
    uint32_t flashbase_address = AVR32_FLASH_ADDRESS;
    uint32_t flash_page = 0;
    bool flash_page_identified = false;
    for (uint32_t page = 0 ; page < AVR32_FLASHC_USER_PAGE_SIZE; page++)
    {
        if (addr == (flashbase_address + 1))
        {
            flash_page = page;
            flash_page_identified = true;
        }
        else
        {
            flashbase_address += AVR32_FLASHC_PAGE_SIZE;
        }
        if (flash_page_identified)
        {
            break;
        }

    }
    return(flash_page);
}

/**
 * @brief Clears/Erases for a given variable id the flash pages used for the NVM
 *
 * @param var_id  Identifier of the variable to be cleared
 *
 */
static void nvm_var_clear(uint8_t var_id)
{
    uint32_t addr;
    uint32_t page_number;
    addr = NVM_MULTI_WRITE_START + (var_id * NVM_MULTI_WRITE_NUM_PG_PER_VAR * SPM_PAGESIZE);
    for (uint8_t i = 0; i < NVM_MULTI_WRITE_NUM_PG_PER_VAR; i++)
    {
        page_number = flash_address_check(addr);
        flashc_erase_page(page_number, true);
        addr += SPM_PAGESIZE;
    }

}

/**
 * @brief To get the current location of the flash
 *
s *
 * @param var_id    Identifier of the flash variable
 * @param *addr     Pointer to 32bit value of the flash address
 *
 */
static bool get_current_location(uint8_t var_id, uint32_t *addr)
{
    uint32_t temp_addr = NVM_MULTI_WRITE_START + (var_id * NVM_MULTI_WRITE_NUM_PG_PER_VAR * SPM_PAGESIZE);
    bool found = false;
    uint8_t pg_cnt;

    // Search for unused flash storage area
    for (pg_cnt = 0; pg_cnt < NVM_MULTI_WRITE_NUM_PG_PER_VAR; pg_cnt++)
    {
        for (uint8_t k = 0; k < SPM_PAGESIZE / sizeof(uint32_t); k++)
        {
            uint32_t val;
            uint8_t *ptr;
            ptr = (uint8_t *)&val;

            // Read next 32 bit value
            uint8_t *index;
            for (uint8_t i = 0; i < sizeof(uint32_t); i++)
            {
                index = (uint8_t *)temp_addr;
                *ptr = *index;
                ptr++;
                temp_addr++;
            }
            /* Check if storage location is already used. */
            if (val == 0xFFFFFFFF)
            {
                found = true;
                *addr = temp_addr - (2 * sizeof(uint32_t));  // move pointer to previous value
                break;    // leave inner loop
            }
        }
        if (found)
        {
            break; // leave outer loop
        }
    }

    return found;
}

#endif /* #ifdef NVM_MULTI_WRITE */

