/**
 * @file pal_flash.h
 *
 * @brief Flash writting support functionality
 *
 * $Id: pal_flash.h 25330 2011-02-03 06:48:22Z yogesh.bellan $
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
#ifndef PAL_FLASH_H
#define PAL_FLASH_H

/* === Includes =============================================================*/
#include <avr32/io.h>
#include <stddef.h>

/* === Globals ============================================================= */
/* Number of flash regions defined by the FLASHC */
#define AVR32_FLASHC_REGIONS  (AVR32_FLASHC_FLASH_SIZE /\
                               (AVR32_FLASHC_PAGES_PR_REGION * AVR32_FLASHC_PAGE_SIZE))

/* === Prototypes =========================================================== */
/**
 * @brief Gets the size of the whole flash array.
 *
 * @return The size of the whole flash array in bytes.
 */
extern uint32_t flashc_get_flash_size(void);


/**
 * @brief Tells whether the FLASHC is ready to run a new command.
 *
 * @return Whether the FLASHC is ready to run a new command.
 */
extern bool flashc_is_ready(void);


/**
 * @brief Waits actively until the FLASHC is ready to run a new command.
 *
 * This is the default function assigned to \ref flashc_wait_until_ready.
 */
extern void flashc_default_wait_until_ready(void);


/**
 * Pointer to the function used by the driver when it needs to wait until the
 * FLASHC is ready to run a new command.
 */
extern void (*volatile flashc_wait_until_ready)(void);


/**
 * @brief Tells whether a Lock Error has occurred during the last function
 *        called that issued one or more FLASHC commands.
 *
 * @return Whether a Lock Error has occurred during the last function called
 *         that issued one or more FLASHC commands.
 */
extern bool flashc_is_lock_error(void);


/**
 * @brief Tells whether a Programming Error has occurred during the last
 *        function called that issued one or more FLASHC commands.
 *
 * @return Whether a Programming Error has occurred during the last function
 *         called that issued one or more FLASHC commands.
 */
extern bool flashc_is_programming_error(void);


/**
 * @brief Gets the last issued FLASHC command.
 *
 * @return The last issued FLASHC command.
 */
extern uint32_t flashc_get_command(void);


/**
 * @brief Gets the current FLASHC page number.
 *
 * @return The current FLASHC page number.
 */
extern uint32_t flashc_get_page_number(void);


/**
 * @brief Issues a FLASHC command.
 *
 * @param command The command: \c AVR32_FLASHC_FCMD_CMD_x.
 * @param page_number The page number to apply the command to
 */
extern void flashc_issue_command(uint32_t command, int page_number);


/**
 * @brief Clears the page buffer.
 *
 * This command resets all bits in the page buffer to one. Write accesses to the
 * page buffer can only change page buffer bits from one to zero.
 *
 * @warning The page buffer is not automatically reset after a page write.
 *
 * @note The FLASHC error status returned by \ref flashc_is_lock_error and
 *       \ref flashc_is_programming_error is updated.
 */
extern void flashc_clear_page_buffer(void);


/**
 * @brief Tells whether the page to which the last Quick Page Read or Quick
 *         Page Read User Page command was applied was erased.
 *
 * @return Whether the page to which the last Quick Page Read or Quick Page Read
 *         User Page command was applied was erased.
 */
extern bool flashc_is_page_erased(void);


/**
 * @brief Applies the Quick Page Read command to a page.
 *
 * @param page_number The page number
 * @return Whether the specified page is erased.
 *
 * @note The FLASHC error status returned by \ref flashc_is_lock_error and
 *       \ref flashc_is_programming_error is updated.
 */
extern bool flashc_quick_page_read(int page_number);


/**
 * @brief Erases a page.
 *
 * @param page_number The page number:
 * @param check Whether to check erase: \c TRUE or \c FALSE.
 *
 * @return Whether the erase succeeded or always \c TRUE if erase check was not
 *         requested.
 */
extern bool flashc_erase_page(int page_number, bool check);


/**
 * @brief Writes a page from the page buffer.
 *
 * @param page_number The page number
 *
 * @warning A Lock Error is issued if the command is applied to a page belonging
 *          to a locked region or to the bootloader protected area.
 *
 * @warning The page buffer is not automatically reset after a page write.
 *
 * @note The FLASHC error status returned by \ref flashc_is_lock_error and
 *       \ref flashc_is_programming_error is updated.
 *
 * @note A write operation can only clear bits.
 */
extern void flashc_write_page(int page_number);


/**
 * @brief Issues a Quick Page Read User Page command to the FLASHC.
 *
 * @return Whether the User page is erased.
 *
 * @note The FLASHC error status returned by \ref flashc_is_lock_error and
 *       \ref flashc_is_programming_error is updated.
 */
extern bool flashc_quick_user_page_read(void);


/**
 * @brief Erases the User page.
 *
 * @param check Whether to check erase: \c TRUE or \c FALSE.
 *
 * @return Whether the erase succeeded or always \c TRUE if erase check was not
 *         requested.
 *
 * @note The FLASHC error status returned by \ref flashc_is_lock_error and
 *       \ref flashc_is_programming_error is updated.
 *
 * @note An erase operation can only set bits.
 */
extern bool flashc_erase_user_page(bool check);


/**
 * @brief Writes the User page from the page buffer.
 *
 * @warning The page buffer is not automatically reset after a page write.
 *
 * @note The FLASHC error status returned by \ref flashc_is_lock_error and
 *       \ref flashc_is_programming_error is updated.
 *
 * @note A write operation can only clear bits.
 */
extern void flashc_write_user_page(void);


/**
 * @brief Copies \a nbytes bytes to the flash destination pointed to by \a dst
 *         from the source pointed to by \a src.
 *
 * @param dst Pointer to flash destination.
 * @param src Pointer to source data.
 * @param nbytes Number of bytes to copy.
 * @param erase Whether to erase before writing: \c TRUE or \c FALSE.
 *
 * @return The value of \a dst.
 *
 * @warning If copying takes place between areas that overlap, the behavior is
 *          undefined.
 *
 * @warning This function may be called with \a erase set to \c FALSE only if
 *          the destination consists only of erased words
 *
 *
 * @note The FLASHC error status returned by \ref flashc_is_lock_error and
 *       \ref flashc_is_programming_error is updated.
 */
extern volatile void *flash_memcpy(volatile void *dst, const void *src, size_t nbytes, bool erase);

#endif  // PAL_FLASH_H

/* EOF */
