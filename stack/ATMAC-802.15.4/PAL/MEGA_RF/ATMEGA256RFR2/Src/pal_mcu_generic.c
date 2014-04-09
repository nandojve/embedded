/**
 * @file pal_mcu_generic.c
 *
 * @brief MCU related functions generic for ATmega256RFR2.
 *
 * This file implements functions generic for the ATmega256RFR2 MCU.
 *
 * $Id: pal_mcu_generic.c 27659 2011-07-08 14:42:45Z sschneid $
 *
 * @author    Atmel Corporation: http://www.atmel.com
 * @author    Support email: avr@atmel.com
 */
/*
 * Copyright (c) 2009, Atmel Corporation All rights reserved.
 *
 * Licensed under Atmel's Limited License Agreement --> EULA.txt
 */

/* === Includes ============================================================ */

#include "pal.h"

#if (PAL_TYPE == ATMEGA256RFR2)

/* === Macros =============================================================== */


/* === Globals ============================================================= */


/* === Prototypes =========================================================== */

static void mcu_clock_init(void);

/* === Implementation ======================================================= */

/**
 * @brief Initializer MCU during start-up
 */
void mcu_init(void)
{
    mcu_clock_init();

    /* Enable SRAM Data Retention */
    DRTRAM0 = _BV(ENDRT);
    DRTRAM1 = _BV(ENDRT);
    DRTRAM2 = _BV(ENDRT);
    DRTRAM3 = _BV(ENDRT);
}

/**
 * @brief Initialize the system clock to 16 MHz
 *
 * Ensure that CKDIV8 fuse does not affect the system clock prescaler
 *
 */
void mcu_clock_init(void)
{
    uint8_t low_fuse, prescaler;

    /*
     * Determine which oscillator the CPU is configured for, and
     * adjust the clock prescaler appropriately.  For the internal RC
     * oscillator, the special prescaler value 0x0F must be used to
     * adjust the RC oscillator for full 16 MHz operation.  In all
     * other cases, value 0x0 is used to use a prescaler of 1.  This
     * makes the operation independent of the CKDIV8 fuse setting.
     */
    ENTER_CRITICAL_REGION();
    low_fuse = read_avr_fuse(0);
    LEAVE_CRITICAL_REGION();

    if ((low_fuse & 0x0F) == 0x02)
    {
        /* Internal RC oscillator used. */
#if (F_CPU == 16000000)
        prescaler = 0x0F;
#endif
#if (F_CPU == 8000000)
        prescaler = clock_div_1;
#endif
#if (F_CPU == 4000000)
        prescaler = clock_div_2;
#endif
    }
    else
    {
        /* Any other clock source, usually the 16 MHz crystal oscillator. */
        prescaler = clock_div_1;
    }

    clock_prescale_set(prescaler);
}

#endif /* (PAL_TYPE == ATMEGA256RFR2) */

/* EOF */
