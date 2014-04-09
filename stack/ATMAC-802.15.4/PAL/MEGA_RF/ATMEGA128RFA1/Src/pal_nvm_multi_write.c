/**
 * @file pal_nvm_multi_write.c
 *
 * @brief Implementation of the multiple NVM write access
 *
 * $Id: pal_nvm_multi_write.c 28616 2011-09-20 08:37:55Z uwalter $
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

#ifdef BOOT_FLASH
#include "pal_boot_flash.h"
#endif

/* === Macros =============================================================== */

/* === Globals ============================================================= */

static uint32_t var_addr[NVM_NUM_VARS];

/* === Prototypes =========================================================== */

static void nvm_var_clear(uint8_t var_id);
static bool get_current_location(uint8_t var_id, uint32_t *addr);

/* === Implementation ======================================================= */


void pal_nvm_multi_init(uint8_t var_id)
{
    nvm_var_clear(var_id);   // Erase/Clear entire flash page
    uint32_t value = 0x00000000;    // Set the first value to 0x00000000

    // Write first value to page buffer
    var_addr[var_id] = NVM_MULTI_WRITE_START + (var_id * NVM_MULTI_WRITE_NUM_PG_PER_VAR * SPM_PAGESIZE);
    flash_fill_page_buffer(0, sizeof(uint32_t), (uint8_t *)&value);

    // Write page buffer to actual flash page
    flash_write_page(var_addr[var_id]);
}


void pal_nvm_multi_write32(uint8_t var_id, uint32_t *value)
{
    bool clear = false;

    // Check if current flash address is the last one of the flash section used for this variable.
    if (var_addr[var_id] == (NVM_MULTI_WRITE_START +
        ((var_id +1) * NVM_MULTI_WRITE_NUM_PG_PER_VAR * SPM_PAGESIZE) - sizeof(uint32_t)))
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

    // Write new value to page buffer
    flash_fill_page_buffer((var_addr[var_id] % SPM_PAGESIZE), sizeof(uint32_t), (uint8_t*)value);

    // Write page buffer to actual flash page
    flash_write_page(var_addr[var_id]);
}


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
    for (uint8_t i = 0; i < sizeof(uint32_t); i++)
    {
        *ptr = PGM_READ_BYTE_FAR(addr);
        ptr++;
        addr++;
    }
}


void pal_nvm_multi_get32(uint8_t var_id, uint32_t **value)
{
    uint32_t addr = var_addr[var_id];

    // Read 32 bit value
    uint8_t *ptr;
    ptr = (uint8_t *)value;
    for (uint8_t i = 0; i < sizeof(uint32_t); i++)
    {
        *ptr = PGM_READ_BYTE_FAR(addr);
        ptr++;
        addr++;
    }
}


// Clears/Erases for a given variable id the flash pages used for the NVM
static void nvm_var_clear(uint8_t var_id)
{
    uint32_t addr;

    addr = NVM_MULTI_WRITE_START + (var_id * NVM_MULTI_WRITE_NUM_PG_PER_VAR * SPM_PAGESIZE);
    for (uint8_t i = 0; i < NVM_MULTI_WRITE_NUM_PG_PER_VAR; i++)
    {
        flash_erase_page(addr);
        addr += SPM_PAGESIZE;
    }
}


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
            for (uint8_t i = 0; i < sizeof(uint32_t); i++)
            {
                *ptr = PGM_READ_BYTE_FAR(temp_addr);
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
