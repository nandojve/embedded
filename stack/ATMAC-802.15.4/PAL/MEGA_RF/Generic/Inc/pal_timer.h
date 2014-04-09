/**
 * @file pal_timer.h
 *
 * @brief PAL timer internal functions prototypes for AVR 8-Bit Mega RF Single Chips
 *
 * This header has the timer specific stuctures, macros and
 * internal functions for AVR 8-Bit Mega RF Single Chips.
 *
 * $Id: pal_timer.h 28625 2011-09-20 12:51:03Z uwalter $
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
#ifndef PAL_TIMER_H
#define PAL_TIMER_H

/* === Includes ============================================================= */


/* === Types ================================================================ */

/*
 * This defines the structure of the time type.
 */
typedef struct timer_info_tag
{
    /* Timeout in microseconds */
    uint32_t abs_exp_timer;

    /* Callback function to be executed on expiry of the timer */
    FUNC_PTR timer_cb;

    /* Parameter to be passed to the callback function of the expired timer */
    void *param_cb;

    /* Next timer which was started or has expired */
    uint_fast8_t next_timer_in_queue;
} timer_info_t;

/*
 * Type definition for callbacks for timer functions
 */
typedef void (*timer_expiry_cb_t)(void *);

/* === Externals ============================================================ */

/* === Macros ================================================================ */

/*
 * Value to indicate end of timer in the array or queue
 */
#define NO_TIMER                (0xFF)

#if defined(DOXYGEN)
/**
 * @brief Reads the 32-bit timer register in the required order of bytes
 *
 * @param hh hh octet of 32-bit register
 * @param hl hl octet of 32-bit register
 * @param lh lh octet of 32-bit register
 * @param ll ll octet of 32-bit register
 *
 * @returns uint32_t Value of timer register
 */
static inline uint32_t sc_read32(volatile uint8_t *hh,
                                 volatile uint8_t *hl,
                                 volatile uint8_t *lh,
                                 volatile uint8_t *ll);
#else
FORCE_INLINE(uint32_t, sc_read32, volatile uint8_t *hh,
                                  volatile uint8_t *hl,
                                  volatile uint8_t *lh,
                                  volatile uint8_t *ll)
{
    union
    {
        uint8_t a[4];
        uint32_t rv;
    }
    x;

    x.a[0] = *ll;
    x.a[1] = *lh;
    x.a[2] = *hl;
    x.a[3] = *hh;

    return x.rv;
}
#endif /* DOXYGEN */


/** String concatenation by preprocessor used to create proper register names. */
#define CONCAT(a,b) a##b

/** Creates proper subregister names and reads the corresponding values. */
#define SC_READ32(reg)                  sc_read32(&CONCAT(reg,HH), \
                                                  &CONCAT(reg,HL), \
                                                  &CONCAT(reg,LH), \
                                                  &CONCAT(reg,LL))

/** Creates proper subregister names and writes the corresponding values. */
#define SC_WRITE32(reg,val) \
do { \
    union { uint8_t a[4]; uint32_t v; } x; \
    x.v = val; \
    CONCAT(reg,HH) = x.a[3]; \
    CONCAT(reg,HL) = x.a[2]; \
    CONCAT(reg,LH) = x.a[1]; \
    CONCAT(reg,LL) = x.a[0]; \
} while(0)

/*
 * Since all layers above PAL currently require a timer tick of 1us,
 * but the used hardware timer runs at symbol rate (1tick = 16us),
 * the upper 4 bits of the hardware timer are not used.
 *
 * In order to cope with this a variety of defines are required.
 */
/** Unmask upper 4 bits. */
#define MASK_LOW28                      (0x0FFFFFFFuL)

/** Unmask upper 5 bits. */
#define MASK_LOW27                      (0x07FFFFFFuL)

/** Conversion factor: us per symbol for 2,4 GHz. */
#define PAL_US_PER_SYMBOLS              (16)

/* === Prototypes =========================================================== */

#ifdef __cplusplus
extern "C" {
#endif

void timer_init(void);
void timer_init_non_generic(void);
void internal_timer_handler(void);
void timer_service(void);

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif  /* PAL_TIMER_H */
/* EOF */
