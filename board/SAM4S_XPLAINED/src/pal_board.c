/**
 * @file pal_board.c
 *
 * @brief PAL board specific functionality
 *
 * This file implements PAL board specific functionality.
 *
 * $Id: pal_board.c 30926 2012-02-20 13:29:28Z mahendran.p $
 *
 *  @author
 *      Atmel Corporation: http://www.atmel.com
 *      Support email: avr@atmel.com
 */
/*
 * Copyright (c) 2010, Atmel Corporation All rights reserved.
 *
 * Licensed under Atmel's Limited License Agreement --> EULA.txt
 */
#ifdef MAC802_15_4

/* === Includes ============================================================ */

#include <stdbool.h>
#include <stdlib.h>
#include "pal.h"
#include "pal_boardtypes.h"
#include "pal_config.h"
/*
 * 'sys_time' is a entity of timer module which is given to other modules
 * through the interface file below. 'sys_time' is required to obtain the
 * frame timestamp
 */
#include "pal_timer.h"

#if (BOARD_TYPE == SAM4S_XPLAINED)

/* === Macros =============================================================== */

/* Shift bits definitions of CKGR_PLLR */
#define CKGR_MUL_SHIFT					(16)
#define CKGR_PLLCOUNT_SHIFT				(8)
#define CKGR_DIV_SHIFT					(0)

/* PLLA mutilplier and divider settings */
#if (F_CPU == 32000000)
/* For 32MHz*/
#define PLLA_MULTIPLIER					(0xF)
#define PLLA_DIVIDER					(0x3)

#elif (F_CPU == 48000000)
/* For 48MHz*/
#define PLLA_MULTIPLIER					(0x7)
#define PLLA_DIVIDER					(0x1)
#else
#error "No settings for current F_CPU."
#endif

/* PLLB mutilplier and divider settings - Used for USB */
#define PLLB_MULTIPLIER					(0x7)
#define PLLB_DIVIDER					(0x1)

/* Main Oscillator Register settings*/
#define BOARD_OSCOUNT					(CKGR_MOR_MOSCXTST_Msk & (0x8 << 8))

#define BOARD_PLLAR						((1 << 29) | ( PLLA_MULTIPLIER << CKGR_MUL_SHIFT)	\
										| (0x1 << CKGR_PLLCOUNT_SHIFT)						\
										| (PLLA_DIVIDER << CKGR_DIV_SHIFT))

#define BOARD_PLLBR						((1 << 29) | ( PLLB_MULTIPLIER << CKGR_MUL_SHIFT)	\
										| (0x1 << CKGR_PLLCOUNT_SHIFT)						\
										| (PLLB_DIVIDER << CKGR_DIV_SHIFT))

#define BOARD_MCKR						(PMC_MCKR_PRES_CLK_2 | PMC_MCKR_CSS_PLLA_CLK)

/* Timer Channel Macros*/
#define TIMER_CHANNEL_0					(0)
#define TIMER_CHANNEL_1					(1)
#define TIMER_CHANNEL_2					(2)

/* Serial Clock Baud Rate position in CSR register*/
#define SCBR_FIELD_POS_IN_CSR_REG		(8)

/* Define clock timeout */
#define CLOCK_TIMEOUT					(0xFFFFFFFF)

/* === Globals ============================================================== */

/* Indicates that the ADC conversion is finished. */
static volatile uint8_t conversionDone;
static const pin_t spi_pins[]			= PINS_SPI;
/* === Prototypes =========================================================== */

static void initialize_timer_channel(uint32_t mode, TcChannel *channel);
static uint8_t adc_is_channel_irq_status_set(uint32_t adc_sr, uint8_t channel);
static void adc_initialize (Adc *pAdc);
void trx_interface_init(void);

/* === Implementation ======================================================= */

/**
 * @brief Provides timestamp of the last received frame
 *
 * This function provides the timestamp (in microseconds)
 * of the last received frame.
 *
 * @param[out] Timestamp in microseconds
 */
void pal_trx_read_timestamp(uint32_t *timestamp)
{
    ENTER_CRITICAL_REGION();
    /*
     * 'TIME_STAMP_HIGH_REGISTER'    'TIME_STAMP_LOW_REGISTER register'
     *  |-----------------|-----------------| => 32 bit timestamp
     *        16 bits           16 bits
     */
    *timestamp							= (uint32_t)TIME_STAMP_HIGH_REGISTER << (uint32_t)16;
    *timestamp							|= TIME_STAMP_LOW_REGISTER;
    
	/* The timeout is pre scaled accroding to the clock period. */
    *timestamp							= (uint32_t)(*timestamp * CLOCK_PERIOD);

    LEAVE_CRITICAL_REGION();
}


#ifdef SNIFFER
/**
 * @brief Write the timestamp value
 *
 * This function writes the timestamp in the register
 *
 * @param[In] Timestamp in microseconds
 */
void pal_trx_write_timestamp(uint32_t *timestamp)
{
    /*
     * 'TIME_STAMP_HIGH_REGISTER'   'TIME_STAMP_LOW_REGISTER'
     *  ---------|--------- => 32 bit timestamp
     *   16 bits   16 bits
     */
    TIME_STAMP_LOW_REGISTER				= (uint16_t)(*timestamp);
    TIME_STAMP_HIGH_REGISTER			= (uint16_t)((*timestamp) >> (uint32_t)16);
}
#endif


/**
 * @brief Calibrates the internal RC oscillator
 *
 * This function calibrates the internal RC oscillator.
 *
 * @return True
 */
bool pal_calibrate_rc_osc(void)
{
    return (true);
}


/**
 * @brief Initializes the GPIO pins
 *
 * This function is used to initialize the port pins used to connect
 * the microcontroller to transceiver.
 */
void gpio_init(void)
{
    /*
     * The clock to PIO A and PIO B are enabled. This is necessary, as only
     * when the clock is provided the PIO starts functioning.
     */
    PERIPHERAL_CLOCK_ENABLE(ID_PIOA);
    PERIPHERAL_CLOCK_ENABLE(ID_PIOB);
	PERIPHERAL_CLOCK_ENABLE(ID_PIOC);

    /* The following pins are output pins.  */
    PIN_SET_AS_PIO_OUTPUT(AT86RFX_RST_MASK, AT86RFX_RST_PIO);
    PIN_SET_AS_PIO_OUTPUT(AT86RFX_SLP_MASK, AT86RFX_SLP_PIO);
}

/*
 * This function is called by timer_init() to perform the non-generic portion
 * of the initialization of the timer module.
 */
void timer_init_non_generic(void)
{
#ifdef TIMESTAMP_UPPER_16BIT_SW

    initialize_timer_channel(TC_CLKS_MCK32, &PAL_TIMER_CH);

    /* The clock for pal timer is enabled here. */
    PERIPHERAL_CLOCK_ENABLE(PAL_TIMER_PERIPH_ID);

    /* The clock is enabled at the timer level. */
    PAL_TIMER_CCR						= PAL_TIMER_CCR_EN;

    /* timer overflow interrupts for the pal timer is enabled */
    PAL_TIMER_IER						= PAL_TIMER_SR_OVERFLOW_FLAG;

    /* The pal timer channel interrupt in NVIC is enabled. */
    NVIC_EnableIRQ(PAL_TIMER_IRQ_ID);

    /* pal timer channel is triggered. Reset counter and start clock */
    PAL_TIMER_CCR						= TC_CCR_SWTRG;
#else
    initialize_timer_channel(TC_CLKS_MCK32, &PAL_TIMER_CH);

    /* The clock for pal timer is enabled here. */
    PERIPHERAL_CLOCK_ENABLE(PAL_TIMER_PERIPH_ID);

    /* The clock is enabled at the timer level. */
    PAL_TIMER_CCR						= PAL_TIMER_CCR_EN;

    /* timer overflow interrupts for the pal timer is enabled */
    PAL_TIMER_IER						= PAL_TIMER_SR_OVERFLOW_FLAG;

    /* The pal timer channel interrupt in NVIC is enabled. */
    NVIC_EnableIRQ(PAL_TIMER_IRQ_ID);

    /* Initialize timer for Time stamp -32bit time */
    initialize_timer_channel((TC_CLKS_MCK32 | TC_CMR0_WAVE | TC_CMR0_ACPA_SET | (0x1 << 13) | TC_CMR0_ACPC_CLEAR), &TC0->TC_CHANNEL[TIMER_CHANNEL_0]);
    initialize_timer_channel((TC_CMR0_TCCLKS_XC1), &TC0->TC_CHANNEL[TIMER_CHANNEL_1]);

    /* RC is loaded with 0xFFFF so that whenever CV reached FFFF(RC) TIOA line is
     made as high */
    TC0->TC_CHANNEL[TIMER_CHANNEL_0].TC_RC = 0xFFFF;

    TC0->TC_BMR							= TC_BMR_TC1XC1S_TCLK1;
    TC0->TC_BCR							= TC_BCR_SYNC;

    /* The peripheral clock for pal timer is enabled */
    PERIPHERAL_CLOCK_ENABLE(ID_TC0);
    PERIPHERAL_CLOCK_ENABLE(ID_TC1);

    /* The clock is enabled at the timer level. */
    TC0->TC_CHANNEL[TIMER_CHANNEL_0].TC_CCR = TC_CCR0_CLKEN;
    TC0->TC_CHANNEL[TIMER_CHANNEL_1].TC_CCR = TC_CCR1_CLKEN;

    /* Reset counter and start the timer */
    PAL_TIMER_CCR = TC_CCR0_SWTRG;
    TC0->TC_CHANNEL[TIMER_CHANNEL_0].TC_CCR = TC_CCR0_SWTRG;
    TC0->TC_CHANNEL[TIMER_CHANNEL_1].TC_CCR = TC_CCR1_SWTRG;
#endif


#ifdef ENABLE_HIGH_PRIO_TMR
    initialize_timer_channel(TC_CLKS_MCK32, &PAL_HIGHPRI_TMR_CH);
    /*
     * The clock for high priority timer will be enabled only when a high
     * priority timer is requested to be started, as until that time, the timer
     * is not used at all. Also the overall power consumption
     * can be reduced by clocking a perpheral only when required.
     */
#endif  /* ENABLE_HIGH_PRIO_TMR */
}


/**
 * @brief Configures the timer channel
 *
 * This function configures the timer channel. It disables the clock to the
 * timer channel at the timer level, disables all the timer interrupts and
 * programs the prescaler for timer clock.
 *
 * @param mode Value to be written in the TCCLKS field
 *                                of the TC_CMR register, to select the
 *                                prescaler for the main clock which is
 *                                the timer clock source
 * @param channel - timer channel
 */
static void initialize_timer_channel(uint32_t mode,
                                     TcChannel *channel)
{
    uint32_t tc_status;
    TcChannel *pTcCh;

    pTcCh								= channel;

    /* The clock and the interrupts of the timer channel are disabled. */
    /*  Disable TC clock */
    pTcCh->TC_CCR						= TC_CCR_CLKDIS;

    /*  Disable interrupts */
    pTcCh->TC_IDR						= ALL_TIMER_INTERRUPTS_MASK;

    /* The status register is read to clear any pending interrupt. */
    tc_status							= pTcCh->TC_SR;

    /*  Set mode */
    /* The prescaler for the timer clock source (main clock) is selected. */
    pTcCh->TC_CMR						= mode;

    /*
     * Done to avoid compiler warning about variable being not used after
     * setting.
     */
    tc_status							= tc_status;
}


/**
 * @brief Initialize LEDs
 */
void pal_led_init(void)
{
    PIN_SET((PIN_LED_0_MASK | PIN_LED_1_MASK), PIN_LED_0_PIO);
    PIN_SET_AS_PIO_OUTPUT((PIN_LED_0_MASK | PIN_LED_1_MASK), PIN_LED_0_PIO);
}


/**
 * @brief Control LED status
 *
 * param led_no LED ID
 * param led_setting LED_ON, LED_OFF, LED_TOGGLE
 */
void pal_led(led_id_t led_no, led_action_t led_setting)
{
    uint32_t led_pin;
    uint32_t pin_status;
    uint32_t pio_select;

    switch (led_no)
    {
        case LED_0:
        {
            led_pin						= PIN_LED_0_MASK;
            pio_select					= PIN_LED_0_PIO;
        }
        break;
        case LED_1:
        {
            led_pin						= PIN_LED_1_MASK;
            pio_select					= PIN_LED_0_PIO;
        }
        break;
        //case LED_2:
        //{
            //led_pin = PIO_PA20;
            //pio_select = PIO_A;
        //}
        //break;
        default:
            return;
    }

    if (led_setting == LED_ON)
    {
        PIN_CLEAR(led_pin, pio_select);
    }
    else if (led_setting == LED_OFF)
    {
        PIN_SET(led_pin, pio_select);
    }
    else if (led_setting == LED_TOGGLE)
    {
        PIN_GET_STATUS(pin_status, led_pin, pio_select);

        if (pin_status)
        {
            PIN_CLEAR(led_pin, pio_select);
        }
        else
        {
            PIN_SET(led_pin, pio_select);
        }
    }
    else
    {
        return;
    }
}


/**
 * @brief Initialize the button
 */
void pal_button_init(void)
{
    const pin_t pinsPushButtons[]		= {PINS_PUSHBUTTONS};
    
	/* Enable peripheral clock of PIOB(button1) and PIOC(button2) before configuring the pins */
    PERIPHERAL_CLOCK_ENABLE(PIN_SW0_PIO);
    
	/* Configure the pins for buttons */
    pal_pio_configure(pinsPushButtons, PIO_LISTSIZE(pinsPushButtons));
}


/**
 * @brief Read button
 *
 * @param button_no Button ID
 */
button_state_t pal_button_read(button_id_t button_no)
{
    Pio *pio_ptr;
    uint32_t button;
    if (button_no == BUTTON_0)
    {
		button							= PIN_SW0_MASK;
        pio_ptr							= PIN_SW0_PIO;
    }
    //else
    //{
        //pio_ptr = PIOC;
        //button = RIGHT_BUTTON;
    //}
    /* Read the status of the pin(button) */
    if (~pio_ptr->PIO_PDSR & button)
    {
        return BUTTON_PRESSED;
    }
    else
    {
        return BUTTON_OFF;
    }
}


/**
 * @brief Initializes the ADC controller
 *
 * @param pAdc Pointer to an Adc instance
 */
void adc_initialize(Adc *pAdc)
{
    /* Enable the peripheral clock. */
    PERIPHERAL_CLOCK_ENABLE(ID_ADC);

    /* Reset the controller. */
    pAdc->ADC_CR						= ADC_CR_SWRST;

    /* Clear the MR register. */
    pAdc->ADC_MR						= 0;

    /*
     * Write the required mode:
     * TRGEN: Hardware triggers are disabled
     * TRGSEL: 0
     * LOWRES: 12-bit resolution
     * SLEEP: Normal Mode
     */
    pAdc->ADC_MR						= ADC_MR_PRESCAL(ADC_PRESCAL)
										| ADC_MR_STARTUP_SUT512
										| ADC_MR_TRACKTIM(ADC_SHTIM);
}


/**
 * @brief Generation of random seed for function srand() in case this
 *        is not supported by the transceiver (e.g. AT86RF230)
 *
 * @return uint16_t Random number to be used as seed for function srand()
 * @ingroup apiPalApi
 */
uint16_t pal_generate_rand_seed(void)
{
    uint16_t		cur_random_seed		= 0;
    uint8_t			cur_random_bit		= 0;
    uint16_t		cur_adc_sample		= 0;
    uint8_t			no_of_conversion	= NO_OF_CONVERSIONS;   /* 16 times 1 channel to get 16 bit */

    adc_initialize(ADC);

    /* Enable ADC channel. */
    ADC->ADC_CHER						= (1 << ADC_NUM_1);

    /*Enable ADC interrupt*/
    NVIC_EnableIRQ(ADC_IRQn);

    for (no_of_conversion = 0; no_of_conversion < NO_OF_CONVERSIONS; no_of_conversion++)
    {

        /* Enable Interrupt for the ADC channel. */
        ADC->ADC_IER				= 1 << ADC_NUM_1;

        conversionDone				= 0;

        /* Start the conversion. */
        ADC->ADC_CR					= ADC_CR_START;

        while ( conversionDone != ((1 << ADC_NUM_1)) );

        cur_adc_sample				= ADC->ADC_CDR[ADC_NUM_1];
        cur_random_bit				= cur_adc_sample & 0x01;
        cur_random_seed				= cur_random_seed << 1;
        cur_random_seed				|= cur_random_bit;
    }

    /* Disable interrupt source. */
    NVIC_DisableIRQ(ADC_IRQn);

    /* Disable ADC channel. */
    ADC->ADC_CHDR					= (1 << ADC_NUM_1);

    return (cur_random_seed);
}


/**
 * @brief Interrupt handler for the ADC.
 *
 * This function is the interrupt handler for the ADC.
 * Signals that the conversion is finished by setting a flag variable.
 */
void ADC_Handler(void)
{
    uint32_t status					= ADC->ADC_ISR;

    if (adc_is_channel_irq_status_set(status, ADC_NUM_1))
    {
        /* Disable Interrupt for the ADC channel. */
        ADC->ADC_IDR				= 1 << ADC_NUM_1;

        conversionDone				|= (1 << ADC_NUM_1);
    }
}


/**
 * @brief Checks if ADC channel interrupt status is set
 *
 * @param adc_sr Value of SR register
 * @param channel Channel to be checked
 *
 * @return 1 if interrupt status is set, otherwise 0
 */
uint8_t adc_is_channel_irq_status_set(uint32_t adc_sr, uint8_t channel)
{
    if ((adc_sr & (1 << channel)) == (1 << channel))
    {
        return (1);
    }
    else
    {
        return (0);
    }
}


/**
 * @brief initializes the trx interface
 */
void trx_interface_init(void)
{
    /*
     * Configure the SPI pins
     */
    pal_pio_configure(spi_pins, PIO_LISTSIZE(spi_pins));

    /*
     * Select line will be used as a GPIO. The controller recognizes 1 cycle
     * of SPI transaction as 8 bit, hence deactivates the chip select after 1
     * cycle. But the transceiver needs the chip select to be active for two
     * cycles (In one cycle the transceiver gets to know about the address of
     * register or memory location and the kind of operation to be performed
     * on that memory location. And in the second cycle its performs the
     * specified operation). To achieve this, the chip select line will be
     * manually pulled low and high (after the transaction). Hence the SEL line
     * is configured as PIO and the SPI control of SEL is disabled.
     */
    PIN_SET_AS_PIO_OUTPUT(AT86RFX_CS_MASK, AT86RFX_CS_PIO);

    /* The controller is configured to be master. */
    PERIPHERAL_CLOCK_ENABLE(ID_SPI);
    NVIC_EnableIRQ(SPI_IRQn);
    BOARD_TRX_SPI_BASE->SPI_CR			= SPI_CR_SPIDIS;

    /* Execute a software reset of the SPI twice */
    BOARD_TRX_SPI_BASE->SPI_CR			= SPI_CR_SWRST;
    BOARD_TRX_SPI_BASE->SPI_CR			= SPI_CR_SWRST;
    BOARD_TRX_SPI_BASE->SPI_MR			= (SPI_MR_MSTR | SPI_MR_MODFDIS | (SPI_MR_PCS_Msk & SS_ENABLE));
    BOARD_TRX_SPI_BASE->SPI_PTCR		= (SPI_PTCR_TXTDIS | SPI_PTCR_RXTDIS);

    /*
     * SPI mode 0 (clock polarity = 0 and clock phase = 1) is selected. The
     * transaction width is set to 8 bits. The SCBR register of the SPI module
     * is written with the divider required for generation of the baud rate. It
     * is calculated as follows. Baud rate = MCK / SPI_BR_DIVIDER.
     */
    BOARD_TRX_SPI_BASE->SPI_CSR[BOARD_TRX_NPCS] = (SPI_CSR_NCPHA |
                                                   (SPI_CSR_BITS_Msk & SPI_CSR_BITS_8_BIT) |
                                                   (SPI_CSR_SCBR_Msk & (SPI_BR_DIVIDER << SCBR_FIELD_POS_IN_CSR_REG)));

    /* The SPI is enabled. */
    BOARD_TRX_SPI_BASE->SPI_CR			= SPI_CR_SPIEN;
}


/**
 * @brief Performs the low-level initialization of the chip.
 * This includes EFC and master clock configuration.
 * It also enable a low level on the pin NRST triggers a user reset.
 */
void LowLevelInit (void)
{
    uint32_t timeout					= 0;

    /* Set 3 FWS for Embedded Flash Access */
    EFC0->EEFC_FMR						= (3 << 8);

    /* Initialize main oscillator */
    if (!(PMC->CKGR_MOR & CKGR_MOR_MOSCSEL))
    {
        PMC->CKGR_MOR					= (0x37 << 16) | BOARD_OSCOUNT | CKGR_MOR_MOSCRCEN | CKGR_MOR_MOSCXTEN;
        timeout							= 0;
        while (!(PMC->PMC_SR & PMC_SR_MOSCXTS) && (timeout++ < CLOCK_TIMEOUT));
    }

    /* Switch to 3-20MHz Xtal oscillator */
    PMC->CKGR_MOR						= (0x37 << 16) | BOARD_OSCOUNT | CKGR_MOR_MOSCRCEN | CKGR_MOR_MOSCXTEN | CKGR_MOR_MOSCSEL;
    timeout								= 0;
    while (!(PMC->PMC_SR & PMC_SR_MOSCSELS) && (timeout++ < CLOCK_TIMEOUT));
	
    PMC->PMC_MCKR						= (PMC->PMC_MCKR & ~(uint32_t)PMC_MCKR_CSS_Msk) | PMC_MCKR_CSS_MAIN_CLK;
    timeout								= 0;
    while (!(PMC->PMC_SR & PMC_SR_MCKRDY) && (timeout++ < CLOCK_TIMEOUT));

    /* Initialize PLLA */
    PMC->CKGR_PLLAR						= BOARD_PLLAR;
    timeout								= 0;
    while (!(PMC->PMC_SR & PMC_SR_LOCKA) && (timeout++ < CLOCK_TIMEOUT));

    /* Initialize PLLB */
    PMC->CKGR_PLLBR						= BOARD_PLLBR;
    timeout								= 0;
    while (!(PMC->PMC_SR & PMC_SR_LOCKB) && (timeout++ < CLOCK_TIMEOUT));

    /* Switch to fast clock */
    PMC->PMC_MCKR						= (BOARD_MCKR & ~PMC_MCKR_CSS_Msk) | PMC_MCKR_CSS_MAIN_CLK;
    timeout								= 0;
    while (!(PMC->PMC_SR & PMC_SR_MCKRDY) && (timeout++ < CLOCK_TIMEOUT));

    PMC->PMC_MCKR						= BOARD_MCKR;
    timeout								= 0;
    while (!(PMC->PMC_SR & PMC_SR_MCKRDY) && (timeout++ < CLOCK_TIMEOUT));
}

#endif /* #if (BOARD_TYPE == SAM4S_XPLAINED) */

#endif /* #ifdef MAC802_15_4 */

/* EOF */

