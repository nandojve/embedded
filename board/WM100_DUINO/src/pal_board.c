/**
 * @file pal_board.c
 *
 * @brief PAL board specific functionality
 *
 * This file implements PAL board specific functionality.
 *
 * $Id: pal_board.c 19641 2009-12-21 15:15:39Z sschneid $
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
#ifdef MAC802_15_4
#include <stdbool.h>
#include <stdlib.h>
#include "drivers.h"
#include "pal.h"
#include "pal_internal.h"
#include "pal_boardtypes.h"
#include "pal_config.h"

#if (BOARD_TYPE == WM100_DUINO)

/* === Macros ============================================================== */


/* === Types =============================================================== */


/* === Globals ============================================================= */


/* === Prototypes ========================================================== */


/* === Implementation ====================================================== */

/**
 * @brief Calibrates the internal RC oscillator
 *
 * This function calibrates the internal RC oscillator, based
 * on the external 32.768 Hz crystal clock source.
 *
 * It will also set the seed for the random generator based as
 * a side effect of the calibration.
 *
 * @return True if calibration is successful, false otherwise.
 */
bool pal_calibrate_rc_osc(void)
{
    /* Nothing to be done. */
    return true;
}



/**
 * @brief Initialize LEDs
 */
void pal_led_init(void)
{
    LED_PORT		|= (1 << LED_PIN_0);
    LED_PORT_DIR	|= (1 << LED_PIN_0);
    LED_PORT		|= (1 << LED_PIN_1);
    LED_PORT_DIR	|= (1 << LED_PIN_1);
    LED_PORT		|= (1 << LED_PIN_2);
    LED_PORT_DIR	|= (1 << LED_PIN_2);
}



/**
 * @brief Control LED status
 *
 * @param[in]  led_no LED ID
 * @param[in]  led_setting LED_ON, LED_OFF, LED_TOGGLE
 */
void pal_led(led_id_t led_no, led_action_t led_setting)
{
    switch (led_no)
    {
    default:
    case 0:
        switch (led_setting)
        {
            case LED_ON:    LED_PORT &= ~(1 << LED_PIN_0); break;
            case LED_OFF:   LED_PORT |= (1 << LED_PIN_0); break;
            case LED_TOGGLE: LED_PORT ^= (1 << LED_PIN_0); break;
        }
        break;

    case 1:
        switch (led_setting)
        {
            case LED_ON:    LED_PORT &= ~(1 << LED_PIN_1); break;
            case LED_OFF:   LED_PORT |= (1 << LED_PIN_1); break;
            case LED_TOGGLE: LED_PORT ^= (1 << LED_PIN_1); break;
        }
        break;

    case 2:
        switch (led_setting)
        {
            case LED_ON:    LED_PORT &= ~(1 << LED_PIN_2); break;
            case LED_OFF:   LED_PORT |= (1 << LED_PIN_2); break;
            case LED_TOGGLE: LED_PORT ^= (1 << LED_PIN_2); break;
        }
        break;

    }
}


/**
 * @brief Initialize the button
 */
void pal_button_init(void)
{
    BUTTON_PORT |= (1 << BUTTON_PIN_0);
    BUTTON_PORT_DIR &= ~(1 << BUTTON_PIN_0);
}



/**
 * @brief Read button
 *
 * @param button_no Button ID
 */
button_state_t pal_button_read(button_id_t button_no)
{
    /* Keep compiler happy. */
    button_no = button_no;

    if ((BUTTON_INPUT_PINS & (1 << BUTTON_PIN_0)) == 0x00)
    {
        return BUTTON_PRESSED;
    }
    else
    {
        return BUTTON_OFF;
    }
}

/**
 * @brief Initializes the GPIO pins
 */
void gpio_init(void)
{
}

#define				NUM_SAMPLES			255
double pal_read_temperature(void)
{
    float			temp_result;
    float			offset;
    double			result;
    uint8_t			i;
    uint16_t		adc_value;
    uint32_t		temp_result32		= 0;
    int32_t			offset32			= 0;

    ADCSRA								= (1 << ADEN);		// Enable ADC

    /**
      * Analog channel and gain selection
      * The MUX5 bit has to written first followed by a
      * write access to the MUX4:0 bits which triggers the update of the internal buffer.
      */
    ADCSRB								= (1 << MUX5);
    /**
      * Select internal 1.6V reference voltage
      * Select temperature sensor
      */
    ADMUX								= (1 << REFS1) | (1 << REFS0) | (1 << MUX3) | (1 << MUX0);

    /* Dummy conversion to clear PGA */
#if ((F_CPU == 16000000UL) || (F_CPU == 15384600UL))
    ADCSRA								= (1 << ADEN) | (1 << ADSC) | (1 << ADIF) | (1 << ADPS2) | (1 << ADPS0);
#elif (F_CPU == 8000000UL)
    ADCSRA								= (1 << ADEN) | (1 << ADSC) | (1 << ADIF) | (1 << ADPS2);
#elif (F_CPU == 4000000UL)
    ADCSRA								= (1 << ADEN) | (1 << ADSC) | (1 << ADIF) | (1 << ADPS1) | (1 << ADPS0);
#error "unsupported F_CPU"
#endif
    
	/* Wait for conversion to be completed */
    do
    {}
    while((ADCSRA & (1 << ADIF)) == 0x00);

    /* Sample */
    for(i = 0; i < NUM_SAMPLES; i++)
    {
        /**
          * ADC Control and Status Register A:
          * ADC Enable
          * ADC Start Conversion
          * Clear ADIF
          * Prescaler = 32 (500kHz) for 16 MHz main clock
          */

#if ((F_CPU == 16000000UL) || (F_CPU == 15384600UL))
        ADCSRA							= (1 << ADEN) | (1 << ADSC) | (1 << ADIF) | (1 << ADPS2) | (1 << ADPS0);
#elif (F_CPU == 8000000UL)
        ADCSRA							= (1 << ADEN) | (1 << ADSC) | (1 << ADIF) | (1 << ADPS2);
#elif (F_CPU == 4000000UL)
        ADCSRA							= (1 << ADEN) | (1 << ADSC) | (1 << ADIF) | (1 << ADPS1) | (1 << ADPS0);
#error "unsupported F_CPU"
#endif
        /* Wait for conversion to be completed */
        do
        {}
        while((ADCSRA & (1 << ADIF)) == 0x00);

        adc_value						= ADC;

        /* Averaging */
        temp_result32					+= adc_value;
    }
    temp_result							= (float)temp_result32 / NUM_SAMPLES;

    ADCSRA								&= ~(1 << ADEN); // Disable ADC for channel change

    /* Get offset value */
    ADCSRB								= 0x00;
    ADMUX								= (1 << REFS1) | (1 << REFS0) | (1 << MUX3);

    for (i = 0; i < NUM_SAMPLES; i++)
    {
#if ((F_CPU == 16000000UL) || (F_CPU == 15384600UL))
        ADCSRA							= (1 << ADEN) | (1 << ADSC) | (1 << ADIF) | (1 << ADPS2) | (1 << ADPS0);
#elif (F_CPU == 8000000UL)
        ADCSRA							= (1 << ADEN) | (1 << ADSC) | (1 << ADIF) | (1 << ADPS2);
#elif (F_CPU == 4000000UL)
        ADCSRA							= (1 << ADEN) | (1 << ADSC) | (1 << ADIF) | (1 << ADPS1) | (1 << ADPS0);
#error "unsupported F_CPU"
#endif
        /* Wait for conversion to be completed */
        do
        {}
        while((ADCSRA & (1 << ADIF)) == 0x00);

        adc_value						= ADC;

        /* Averaging */
        if(adc_value > 0x1FF)
        {
            offset32					-= 0x400 - adc_value;
        }
        else
        {
            offset32					+= adc_value;
        }
    }
    offset								= (float)offset32 / NUM_SAMPLES;

    ADCSRA								&= ~(1 << ADEN); // Disable ADC

    temp_result							-= offset;
    result								= ((double) temp_result * 1.13) - 272.8;

    return(result);
}


#endif /* WM100_DUINO */
#endif /* MAC802_15_4 */
/* EOF */
