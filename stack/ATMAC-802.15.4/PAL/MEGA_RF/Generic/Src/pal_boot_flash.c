/**
 * @file pal_flash.c
 *
 * @brief Implements functionality handling flash functions implemented in
 * Bootloader.
 *
 * $Id: pal_boot_flash.c 28569 2011-09-14 12:40:18Z pawan.jadia $
 *
 * @author    Atmel Corporation: http://www.atmel.com
 * @author    Support email: avr@atmel.com
 */
/*
 * Copyright (c) 2010, Atmel Corporation All rights reserved.
 *
 * Licensed under Atmel's Limited License Agreement --> EULA.txt
 */

#ifdef BOOT_FLASH

#include "pal.h"
#include "pal_boot_flash.h"

static   rf4ce_boot_req_t   boot_req;

/* The entry point is correct only for atmega1281 and atmega128rfa1. */
#define BOOT_ENTRY_POINT       (FLASHEND - 1ul)

void flash_fill_page_buffer(uint16_t start_offset, uint16_t length, uint8_t *data)
{

#if defined(__ICCAVR__)
  boot_entry_point_t boot_entry_point;
  memcpy_P((void *)&boot_entry_point, (__farflash void *)BOOT_ENTRY_POINT, sizeof(void *));
#elif defined(__GNUC__)
  boot_entry_point_t boot_entry_point = (void *)pgm_read_word_far(BOOT_ENTRY_POINT);
#endif
  //rf4ce_boot_req_t   boot_req;
  uint8_t gpior0;
  uint8_t gpior1;

  boot_req.request_type = FILL_PAGE_BUFFER;
  boot_req.common_boot_req.fill_page_buffer_req.data = data;
  boot_req.common_boot_req.fill_page_buffer_req.start_offset = start_offset;
  boot_req.common_boot_req.fill_page_buffer_req.length = length;

  gpior0 = GPIOR0;
  gpior1 = GPIOR1;

  GPIOR1 = ((uint16_t)&boot_req >> 8);
  GPIOR0 = (uint16_t)&boot_req;
  boot_entry_point();

  GPIOR0 = gpior0;
  GPIOR1 = gpior1;
}


void flash_program_page(uint32_t page_start_addr)
{
#if defined(__ICCAVR__)
  boot_entry_point_t boot_entry_point;
  memcpy_P((void *)&boot_entry_point, (__farflash void *)BOOT_ENTRY_POINT, sizeof(void *));
#elif defined(__GNUC__)
  boot_entry_point_t boot_entry_point = (void *)pgm_read_word_far(BOOT_ENTRY_POINT);
#endif
  //rf4ce_boot_req_t   boot_req;
  uint8_t gpior0;
  uint8_t gpior1;

  boot_req.request_type = PROGRAM_PAGE;
  boot_req.common_boot_req.program_page_req.page_start_addr = page_start_addr;

  gpior0 = GPIOR0;
  gpior1 = GPIOR1;

  GPIOR1 = ((uint16_t)&boot_req >> 8);
  GPIOR0 = (uint16_t)&boot_req;
  boot_entry_point();

  GPIOR0 = gpior0;
  GPIOR1 = gpior1;
}


void flash_swap(uint32_t image_start_addr, uint32_t image_size)
{
#if defined(__ICCAVR__)
  boot_entry_point_t boot_entry_point;
  memcpy_P((void *)&boot_entry_point, (__farflash void *)BOOT_ENTRY_POINT, sizeof(void *));
#elif defined(__GNUC__)
  boot_entry_point_t boot_entry_point = (void *)pgm_read_word_far(BOOT_ENTRY_POINT);
#endif
  //rf4ce_boot_req_t   boot_req;

  boot_req.request_type = SWAP_IMAGE;
  boot_req.common_boot_req.swap_images_req.image_size = image_size;
  boot_req.common_boot_req.swap_images_req.image_start_addr = image_start_addr;

  GPIOR1 = ((uint16_t)&boot_req >> 8);
  GPIOR0 = (uint16_t)&boot_req;
  boot_entry_point();
}

void flash_erase_page(uint32_t page_start_addr)
{
#if defined(__ICCAVR__)
  boot_entry_point_t boot_entry_point;
  memcpy_P((void *)&boot_entry_point, (__farflash void *)BOOT_ENTRY_POINT, sizeof(void *));
#elif defined(__GNUC__)
  boot_entry_point_t boot_entry_point = (void *)pgm_read_word_far(BOOT_ENTRY_POINT);
#endif
  //rf4ce_boot_req_t   boot_req;
  uint8_t gpior0;
  uint8_t gpior1;

  boot_req.request_type = ERASE_PAGE;
  boot_req.common_boot_req.erase_page_req.page_start_addr = page_start_addr;

  gpior0 = GPIOR0;
  gpior1 = GPIOR1;

  GPIOR1 = ((uint16_t)&boot_req >> 8);
  GPIOR0 = (uint16_t)&boot_req;
  boot_entry_point();

  GPIOR0 = gpior0;
  GPIOR1 = gpior1;
}

void flash_write_page(uint32_t page_start_addr)
{
#if defined(__ICCAVR__)
  boot_entry_point_t boot_entry_point;
  memcpy_P((void *)&boot_entry_point, (__farflash void *)BOOT_ENTRY_POINT, sizeof(void *));
#elif defined(__GNUC__)
  boot_entry_point_t boot_entry_point = (void *)pgm_read_word_far(BOOT_ENTRY_POINT);
#endif
  //rf4ce_boot_req_t   boot_req;
  uint8_t gpior0;
  uint8_t gpior1;

  boot_req.request_type = WRITE_PAGE;
  boot_req.common_boot_req.write_page_req.page_start_addr = page_start_addr;

  gpior0 = GPIOR0;
  gpior1 = GPIOR1;

  GPIOR1 = ((uint16_t)&boot_req >> 8);
  GPIOR0 = (uint16_t)&boot_req;
  boot_entry_point();

  GPIOR0 = gpior0;
  GPIOR1 = gpior1;
}

#endif  /* #ifdef BOOT_FLASH */
