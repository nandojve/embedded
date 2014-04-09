/**
 * @file pal_flash.c
 *
 * @brief Implements functionality handling internal flash writting
 *
 * $Id: pal_flash.c 25330 2011-02-03 06:48:22Z yogesh.bellan $
 *
 * @author    Atmel Corporation: http://www.atmel.com
 * @author    Support email: avr@atmel.com
 */
/*
 * Copyright (c) 2010, Atmel Corporation All rights reserved.
 *
 * Licensed under Atmel's Limited License Agreement --> EULA.txt
 */

/* === Includes ============================================================= */
#include "pal.h"
#include "pal_flash.h"

/* === Types =============================================================== */
/* FLASHC Writable Bit-Field Registers */
typedef union
{
    unsigned long                 fcr;
    avr32_flashc_fcr_t            FCR;
} u_avr32_flashc_fcr_t;

typedef union
{
    unsigned long                 fcmd;
    avr32_flashc_fcmd_t           FCMD;
} u_avr32_flashc_fcmd_t;

/* === Globals ============================================================= */
/* Sticky error status of the FLASHC.
 * This variable is updated by functions that issue FLASHC commands. It
 * contains the cumulated FLASHC error status of all the FLASHC commands issued
 * by a function.
 */
static uint32_t flashc_error_status = 0;

/**
 * Pointer to the function used by the driver when it needs to wait until the
 * FLASHC is ready to run a new command.
 */
void (*volatile flashc_wait_until_ready)(void) = flashc_default_wait_until_ready;

/* === Prototypes =========================================================== */

/* === Implementation ======================================================= */
/**
 * @brief Gets the size of the whole flash array.
 *
 * @return The size of the whole flash array in bytes.
 */
uint32_t flashc_get_flash_size(void)
{
#if (defined AVR32_FLASHC_300_H_INCLUDED)
    static const uint32_t FLASH_SIZE[1 << AVR32_FLASHC_PR_FSZ_SIZE] =
    {
        32 << 10,
        64 << 10,
        128 << 10,
        256 << 10,
        384 << 10,
        512 << 10,
        768 << 10,
        1024 << 10
    };
    return FLASH_SIZE[(AVR32_FLASHC.pr & AVR32_FLASHC_PR_FSZ_MASK) >> AVR32_FLASHC_PR_FSZ_OFFSET];
#else
    static const uint32_t FLASH_SIZE[1 << AVR32_FLASHC_FSR_FSZ_SIZE] =
    {
        32 << 10,
        64 << 10,
        128 << 10,
        256 << 10,
        384 << 10,
        512 << 10,
        768 << 10,
        1024 << 10
    };
    return FLASH_SIZE[(AVR32_FLASHC.fsr & AVR32_FLASHC_FSR_FSZ_MASK) >> AVR32_FLASHC_FSR_FSZ_OFFSET];
#endif
}


/**
 * @brief Tells whether the FLASHC is ready to run a new command.
 *
 * @return Whether the FLASHC is ready to run a new command.
 */
bool flashc_is_ready(void)
{
    return ((AVR32_FLASHC.fsr & AVR32_FLASHC_FSR_FRDY_MASK) != 0);
}


/**
 * @brief Waits actively until the FLASHC is ready to run a new command.
 *
 * This is the default function assigned to \ref flashc_wait_until_ready.
 */
void flashc_default_wait_until_ready(void)
{
    while (!flashc_is_ready());
}

/* Gets the error status of the FLASHC. */
static uint32_t flashc_get_error_status(void)
{
    return AVR32_FLASHC.fsr & (AVR32_FLASHC_FSR_LOCKE_MASK |
                             AVR32_FLASHC_FSR_PROGE_MASK);
}


/**
 * @brief Tells whether a Lock Error has occurred during the last function
 *        called that issued one or more FLASHC commands.
 *
 * @return Whether a Lock Error has occurred during the last function called
 *         that issued one or more FLASHC commands.
 */
bool flashc_is_lock_error(void)
{
    return ((flashc_error_status & AVR32_FLASHC_FSR_LOCKE_MASK) != 0);
}


/**
 * @brief Tells whether a Programming Error has occurred during the last
 *        function called that issued one or more FLASHC commands.
 *
 * @return Whether a Programming Error has occurred during the last function
 *         called that issued one or more FLASHC commands.
 */
bool flashc_is_programming_error(void)
{
    return ((flashc_error_status & AVR32_FLASHC_FSR_PROGE_MASK) != 0);
}


/**
 * @brief Gets the last issued FLASHC command.
 *
 * @return The last issued FLASHC command.
 */
uint32_t flashc_get_command(void)
{
    return (AVR32_FLASHC.fcmd & AVR32_FLASHC_FCMD_CMD_MASK) >> AVR32_FLASHC_FCMD_CMD_OFFSET;
}


/**
 * @brief Gets the current FLASHC page number.
 *
 * @return The current FLASHC page number.
 */
uint32_t flashc_get_page_number(void)
{
    return (AVR32_FLASHC.fcmd & AVR32_FLASHC_FCMD_PAGEN_MASK) >> AVR32_FLASHC_FCMD_PAGEN_OFFSET;
}


/**
 * @brief Issues a FLASHC command.
 *
 * @param command The command: \c AVR32_FLASHC_FCMD_CMD_x.
 * @param page_number The page number to apply the command to
 */
void flashc_issue_command(uint32_t command, int page_number)
{
    u_avr32_flashc_fcmd_t u_avr32_flashc_fcmd;
    flashc_wait_until_ready();
    u_avr32_flashc_fcmd.fcmd = AVR32_FLASHC.fcmd;
    u_avr32_flashc_fcmd.FCMD.cmd = command;
    if (page_number >= 0)
    {
        u_avr32_flashc_fcmd.FCMD.pagen = page_number;
    }
    u_avr32_flashc_fcmd.FCMD.key = AVR32_FLASHC_FCMD_KEY_KEY;
    AVR32_FLASHC.fcmd = u_avr32_flashc_fcmd.fcmd;
    flashc_error_status = flashc_get_error_status();
    flashc_wait_until_ready();
}


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
void flashc_clear_page_buffer(void)
{
    flashc_issue_command(AVR32_FLASHC_FCMD_CMD_CPB, -1);
}


/**
 * @brief Tells whether the page to which the last Quick Page Read or Quick
 *         Page Read User Page command was applied was erased.
 *
 * @return Whether the page to which the last Quick Page Read or Quick Page Read
 *         User Page command was applied was erased.
 */
bool flashc_is_page_erased(void)
{
    return ((AVR32_FLASHC.fsr & AVR32_FLASHC_FSR_QPRR_MASK) != 0);
}


/**
 * @brief Applies the Quick Page Read command to a page.
 *
 * @param page_number The page number
 * @return Whether the specified page is erased.
 *
 * @note The FLASHC error status returned by \ref flashc_is_lock_error and
 *       \ref flashc_is_programming_error is updated.
 */
bool flashc_quick_page_read(int page_number)
{
    flashc_issue_command(AVR32_FLASHC_FCMD_CMD_QPR, page_number);
    return flashc_is_page_erased();
}

/**
 * @brief Erases a page.
 *
 * @param page_number The page number:
 * @param check Whether to check erase: \c true or \c false.
 *
 * @return Whether the erase succeeded or always \c true if erase check was not
 *         requested.
 */
bool flashc_erase_page(int page_number, bool check)
{
    bool page_erased = true;
    flashc_issue_command(AVR32_FLASHC_FCMD_CMD_EP, page_number);
    if (check)
    {
        uint32_t error_status = flashc_error_status;
        page_erased = flashc_quick_page_read(-1);
        flashc_error_status |= error_status;
    }
    return page_erased;
}


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
void flashc_write_page(int page_number)
{
    flashc_issue_command(AVR32_FLASHC_FCMD_CMD_WP, page_number);
}


/**
 * @brief Issues a Quick Page Read User Page command to the FLASHC.
 *
 * @return Whether the User page is erased.
 *
 * @note The FLASHC error status returned by \ref flashc_is_lock_error and
 *       \ref flashc_is_programming_error is updated.
 */
bool flashc_quick_user_page_read(void)
{
    flashc_issue_command(AVR32_FLASHC_FCMD_CMD_QPRUP, -1);
    return flashc_is_page_erased();
}


/**
 * @brief Erases the User page.
 *
 * @param check Whether to check erase: \c true or \c false.
 *
 * @return Whether the erase succeeded or always \c true if erase check was not
 *         requested.
 *
 * @note The FLASHC error status returned by \ref flashc_is_lock_error and
 *       \ref flashc_is_programming_error is updated.
 *
 * @note An erase operation can only set bits.
 */
bool flashc_erase_user_page(bool check)
{
    flashc_issue_command(AVR32_FLASHC_FCMD_CMD_EUP, -1);
    return (check) ? flashc_quick_user_page_read() : true;
}


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
void flashc_write_user_page(void)
{
    flashc_issue_command(AVR32_FLASHC_FCMD_CMD_WUP, -1);
}

/**
 * @brief Copies \a nbytes bytes to the flash destination pointed to by \a dst
 *         from the source pointed to by \a src.
 *
 * @param dst Pointer to flash destination.
 * @param src Pointer to source data.
 * @param nbytes Number of bytes to copy.
 * @param erase Whether to erase before writing: \c true or \c false.
 *
 * @return The value of \a dst.
 *
 * @warning If copying takes place between areas that overlap, the behavior is
 *          undefined.
 *
 * @warning This function may be called with \a erase set to \c false only if
 *          the destination consists only of erased words
 *
 *
 * @note The FLASHC error status returned by \ref flashc_is_lock_error and
 *       \ref flashc_is_programming_error is updated.
 */
volatile void *flash_memcpy(volatile void *dst, const void *src, size_t nbytes, bool erase)
{
    /* Use aggregated pointers to have several alignments available for a same address. */
    UnionCVPtr flash_array_end;
    UnionVPtr dest;
    UnionCPtr source;
    StructCVPtr dest_end;
    UnionCVPtr flash_page_source_end;
    bool incomplete_flash_page_end;
    Union64 flash_dword;
    bool flash_dword_pending = false;
    UnionVPtr tmp;
    uint32_t error_status = 0;
    uint32_t i, j;

    /* Reformat arguments. */
    flash_array_end.u8ptr = AVR32_FLASH + flashc_get_flash_size();
    dest.u8ptr = dst;
    source.u8ptr = src;
    dest_end.u8ptr = dest.u8ptr + nbytes;

    /* Align each end of destination pointer with its natural boundary*/
    dest_end.u16ptr = (uint16_t *)Align_down((uint32_t)dest_end.u8ptr, sizeof(uint16_t));
    dest_end.u32ptr = (uint32_t *)Align_down((uint32_t)dest_end.u16ptr, sizeof(uint32_t));
    dest_end.u64ptr = (uint64_t *)Align_down((uint32_t)dest_end.u32ptr, sizeof(uint64_t));

    /* While end of destination is not reached... */
    while (dest.u8ptr < dest_end.u8ptr)
    {
        /* Clear the page buffer in order to prepare data for a flash page write.*/
        flashc_clear_page_buffer();
        error_status |= flashc_error_status;

        /* Determine where the source data will end in the current flash page. */
        flash_page_source_end.u64ptr =
        (uint64_t *)min((uint32_t)dest_end.u64ptr,
                 Align_down((uint32_t)dest.u8ptr, AVR32_FLASHC_PAGE_SIZE) + AVR32_FLASHC_PAGE_SIZE);

        /* Determine if the current destination page has an incomplete end.*/
        incomplete_flash_page_end = (Align_down((uint32_t)dest.u8ptr, AVR32_FLASHC_PAGE_SIZE) >=
                                 Align_down((uint32_t)dest_end.u8ptr, AVR32_FLASHC_PAGE_SIZE));

        /* If destination does not point to the beginning of the current flash page... */
        if (!Test_align((uint32_t)dest.u8ptr, AVR32_FLASHC_PAGE_SIZE))
        {
            /* Fill the beginning of the page buffer with the current flash page data.
             * This is required by the hardware, even if page erase is not requested,
             * in order to be able to write successfully to erased parts of flash
             * pages that have already been written to.
             */
            for (tmp.u8ptr = (uint8_t *)Align_down((uint32_t)dest.u8ptr, AVR32_FLASHC_PAGE_SIZE);
                tmp.u64ptr < (uint64_t *)Align_down((uint32_t)dest.u8ptr, sizeof(uint64_t));
                tmp.u64ptr++)
            *tmp.u64ptr = *tmp.u64ptr;

            /* If destination is not 64-bit aligned... */
            if (!Test_align((uint32_t)dest.u8ptr, sizeof(uint64_t)))
            {
                for (i = 0; i < Get_align((uint32_t)dest.u8ptr, sizeof(uint64_t)); i++)
                flash_dword.u8[i] = *tmp.u8ptr++;

                /* Fill the end of the flash double-word buffer with the source data.*/
                for (; i < sizeof(uint64_t); i++)
                flash_dword.u8[i] = *source.u8ptr++;

                /* Align the destination pointer with its 64-bit boundary. */
                dest.u64ptr = (uint64_t *)Align_down((uint32_t)dest.u8ptr, sizeof(uint64_t));

                /* If the current destination double-word is not the last one...*/
                if (dest.u64ptr < dest_end.u64ptr)
                {
                    /* Write the flash double-word buffer to the page buffer. */
                    *dest.u64ptr++ = flash_dword.u64;
                }

                /* If the current destination double-word is the last one, the flash
                 * double-word buffer must be kept for later.
                 */
                else flash_dword_pending = true;
            }
        }

        /* Read the source data with the maximal possible alignment and write it to
         * the page buffer with 64-bit alignment.
         */
        switch (Get_align((uint32_t)source.u8ptr, sizeof(uint32_t)))
        {
            case 0:
                for (i = flash_page_source_end.u64ptr - dest.u64ptr; i; i--)
                *dest.u64ptr++ = *source.u64ptr++;
                break;

            case sizeof(uint16_t):
                for (i = flash_page_source_end.u64ptr - dest.u64ptr; i; i--)
                {
                    for (j = 0; j < sizeof(uint64_t) / sizeof(uint16_t); j++) flash_dword.u16[j] = *source.u16ptr++;
                    *dest.u64ptr++ = flash_dword.u64;
                }
                break;

            default:
                for (i = flash_page_source_end.u64ptr - dest.u64ptr; i; i--)
                {
                    for (j = 0; j < sizeof(uint64_t); j++) flash_dword.u8[j] = *source.u8ptr++;
                    *dest.u64ptr++ = flash_dword.u64;
                }
        }

        /* If the current destination page has an incomplete end... */
        if (incomplete_flash_page_end)
        {
            /* If the flash double-word buffer is in use, do not initialize it. */
            if (flash_dword_pending) i = Get_align((uint32_t)dest_end.u8ptr, sizeof(uint64_t));
            /* If the flash double-word buffer is free...*/
            else
            {
                /* Fill the beginning of the flash double-word buffer with the source data. */
                for (i = 0; i < Get_align((uint32_t)dest_end.u8ptr, sizeof(uint64_t)); i++)
                    flash_dword.u8[i] = *source.u8ptr++;
            }

            /* This is required by the hardware, even if page erase is not requested,
             * in order to be able to write successfully to erased parts of flash
             * pages that have already been written to.
             */
            {
                tmp.u8ptr = (volatile uint8_t *)dest_end.u8ptr;

                /* If end of destination is not 64-bit aligned... */
                if (!Test_align((uint32_t)dest_end.u8ptr, sizeof(uint64_t)))
                {
                    /* Fill the end of the flash double-word buffer with the current flash page data. */
                    for (; i < sizeof(uint64_t); i++)
                    flash_dword.u8[i] = *tmp.u8ptr++;

                    /* Write the flash double-word buffer to the page buffer.*/
                    *dest.u64ptr++ = flash_dword.u64;
                }

                /* Fill the end of the page buffer with the current flash page data.*/
                for (; !Test_align((uint32_t)tmp.u64ptr, AVR32_FLASHC_PAGE_SIZE); tmp.u64ptr++)
                *tmp.u64ptr = *tmp.u64ptr;
            }
        }

        /* If the current flash page is in the flash array... */
        if (dest.u8ptr <= AVR32_FLASHC_USER_PAGE)
        {
            /* Erase the current page if requested and write it from the page buffer.*/
            if (erase)
            {
                flashc_erase_page(-1, false);
                error_status |= flashc_error_status;
            }
            flashc_write_page(-1);
            error_status |= flashc_error_status;

            /* If the end of the flash array is reached, go to the User page. */
            if (dest.u8ptr >= flash_array_end.u8ptr)
            {
                source.u8ptr += AVR32_FLASHC_USER_PAGE - dest.u8ptr;
                dest.u8ptr = AVR32_FLASHC_USER_PAGE;
            }
        }

        /* If the current flash page is the User page...*/
        else
        {
            /* Erase the User page if requested and write it from the page buffer.*/
            if (erase)
            {
                flashc_erase_user_page(false);
                error_status |= flashc_error_status;
            }
            flashc_write_user_page();
            error_status |= flashc_error_status;
        }
    }

    /* Update the FLASHC error status. */
    flashc_error_status = error_status;

    /* Return the initial destination pointer as the standard memcpy function does. */
    return dst;
}
/* EOF */
