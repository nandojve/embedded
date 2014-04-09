/**
 * @file pal_flash.c
 *
 * @brief Implements functionality handling internal flash writting
 *
 * $Id: pal_flash.c 28569 2011-09-14 12:40:18Z pawan.jadia $
 *
 * @author    Atmel Corporation: http://www.atmel.com
 * @author    Support email: avr@atmel.com
 */
/*
 * Copyright (c) 2010, Atmel Corporation All rights reserved.
 *
 * Licensed under Atmel's Limited License Agreement --> EULA.txt
 */

#ifdef FLASH_SUPPORT

#include "pal.h"
#include "pal_flash.h"

#ifdef BOOT_FLASH
#include "pal_boot_flash.h"
#endif


#ifdef STORE_NIB_TO_FLASH
void pal_flash_write(uint32_t start_addr, uint32_t length, uint8_t *data)
{
    uint32_t remaining_len = length;
    uint8_t *ptr = data;

    do
    {
        uint16_t current_len;

        /* Fill page buffer */
        if (remaining_len > SPM_PAGESIZE)
        {
            current_len = SPM_PAGESIZE;
        }
        else
        {
            current_len = (uint16_t)remaining_len;
        }
        flash_fill_page_buffer(0x0000, current_len, ptr);
        ptr += SPM_PAGESIZE;
        remaining_len -= current_len;
        /* Erase and program flash page */
        flash_program_page(start_addr);
        start_addr += SPM_PAGESIZE;
    } while (remaining_len > 0);
}
#endif

#ifndef BOOT_FLASH

#ifdef NVM_MULTI_WRITE
#ifdef __ICCAVR__
#pragma optimize = no_inline
#endif
void flash_erase_page(uint32_t page_start_addr)
{
    ENTER_CRITICAL_REGION();

    EEPROM_BUSY_WAIT();

    FLASH_ERASE(page_start_addr);
    SPM_BUSY_WAIT();  // Wait until the memory is erased.

    // Reenable RWW-section again. We need this if we want to jump back
    // to the application after bootloading.
    ENABLE_RWW_SECTION();

    LEAVE_CRITICAL_REGION();
}
#endif  /* #ifdef _NVM_MULTI_WRITE */


#ifdef NVM_MULTI_WRITE
#ifdef __ICCAVR__
#pragma optimize = no_inline
#endif
void flash_write_page(uint32_t page_start_addr)
{
    ENTER_CRITICAL_REGION();

    EEPROM_BUSY_WAIT();

    FLASH_PAGE_WRITE(page_start_addr);  // Store buffer in flash page.
    SPM_BUSY_WAIT();  // Wait until the memory is written.

    // Reenable RWW-section again. We need this if we want to jump back
    // to the application after bootloading.
    ENABLE_RWW_SECTION();

    LEAVE_CRITICAL_REGION();
}
#endif /* #ifdef _NVM_MULTI_WRITE */


#ifdef __ICCAVR__
#pragma optimize = no_inline
#endif
void flash_fill_page_buffer(uint16_t start_offset, uint16_t length, uint8_t *data)
{
    for (uint16_t i = start_offset; i < length + start_offset; i+=2)
    {
        uint16_t w = *data++;
        w += (*data++) << 8;
        FLASH_PAGE_FILL(i, w);
    }
}


#ifdef __ICCAVR__
#pragma optimize = no_inline
#endif
void flash_program_page(uint32_t page_start_addr)
{
    ENTER_CRITICAL_REGION();

    EEPROM_BUSY_WAIT();

    FLASH_ERASE(page_start_addr);
    SPM_BUSY_WAIT();  // Wait until the memory is erased.

    FLASH_PAGE_WRITE(page_start_addr);  // Store buffer in flash page.
    SPM_BUSY_WAIT();  // Wait until the memory is written.

    // Reenable RWW-section again. We need this if we want to jump back
    // to the application after bootloading.
    ENABLE_RWW_SECTION();

    LEAVE_CRITICAL_REGION();
}


#ifdef __ICCAVR__
#pragma optimize = no_inline
#endif
void flash_swap(uint32_t image_start_addr, uint32_t image_size)
{
    uint8_t flash_page[SPM_PAGESIZE];
    uint16_t total_num_pages;

    cli();  // Disable global interrupts

    EEPROM_BUSY_WAIT();

    /* Move image to current application code flash pages */
    total_num_pages = (image_size + 255) / 256;
    for (uint16_t i = 0; i < total_num_pages; i++)
    {
        /* Erase application code flash page */
        uint32_t page_start_addr = ((uint32_t)SPM_PAGESIZE * i);
        FLASH_ERASE(page_start_addr);
        SPM_BUSY_WAIT();  // Wait until the memory is erased.

        /* Read image data to flash_page variable */
        page_start_addr = image_start_addr + ((uint32_t)SPM_PAGESIZE * i);
        for (uint16_t k = 0; k < SPM_PAGESIZE ; k++)
        {
            flash_page[k] = PGM_READ_BYTE_FAR(page_start_addr);
            page_start_addr++;
        }
        // Fill temporary page buffer
        uint8_t *data;
        data = flash_page;
        for (uint16_t k = 0; k < SPM_PAGESIZE; k+=2)
        {
            uint16_t w = *data++;
            w += (*data++) << 8;
            FLASH_PAGE_FILL(k, w);
        }

        // Store buffer in flash page - move data from temporary buffer to flash.
        FLASH_PAGE_WRITE((uint32_t)SPM_PAGESIZE * i);
        SPM_BUSY_WAIT();  // Wait until the memory is written.
    }

    // reset the MCU
    //wdt_enable(WDTO_15MS);

    while (1)
    {
        // block until watchdog timer is fired
    }
}

#endif /* BOOT_FLASH */

/*
 * Function prototype ensures that function gets called during MCU initialization
 */
#if 0
void get_mcusr(void) \
  __attribute__((naked)) \
  __attribute__((section(".init3")));
#endif

#if 0
uint8_t mcusr_mirror __attribute__ ((section (".noinit")));
#endif

/*
 * Disable the Watchdog; function used during MCU initialization
 */
#if 0
void get_mcusr(void)
{
    mcusr_mirror = MCUSR;
    MCUSR = 0;
    wdt_disable();
}
#endif

#endif  /* #ifdef FLASH_SUPPORT */

