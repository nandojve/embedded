/*******************************************************************************
* DISCLAIMER
* This software is supplied by Renesas Electronics Corporation and is only
* intended for use with Renesas products. No other uses are authorized. This
* software is owned by Renesas Electronics Corporation and is protected under
* all applicable laws, including copyright laws.
* THIS SOFTWARE IS PROVIDED "AS IS" AND RENESAS MAKES NO WARRANTIES REGARDING
* THIS SOFTWARE, WHETHER EXPRESS, IMPLIED OR STATUTORY, INCLUDING BUT NOT
* LIMITED TO WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE
* AND NON-INFRINGEMENT. ALL SUCH WARRANTIES ARE EXPRESSLY DISCLAIMED.
* TO THE MAXIMUM EXTENT PERMITTED NOT PROHIBITED BY LAW, NEITHER RENESAS
* ELECTRONICS CORPORATION NOR ANY OF ITS AFFILIATED COMPANIES SHALL BE LIABLE
* FOR ANY DIRECT, INDIRECT, SPECIAL, INCIDENTAL OR CONSEQUENTIAL DAMAGES FOR
* ANY REASON RELATED TO THIS SOFTWARE, EVEN IF RENESAS OR ITS AFFILIATES HAVE
* BEEN ADVISED OF THE POSSIBILITY OF SUCH DAMAGES.
* Renesas reserves the right, without notice, to make changes to this software
* and to discontinue the availability of this software. By using this software,
* you agree to the additional terms and conditions found by accessing the
* following link:
* http://www.renesas.com/disclaimer
*
*******************************************************************************/
/*******************************************************************************
* Copyright (C) 2013 Renesas Electronics Corporation. All rights reserved.
*******************************************************************************/
/*******************************************************************************
* File Name     : sample_adc.c
* Device(s)     : RZ/A1H RSK+RZA1H
* H/W Platform  : RSK+RZA1H CPU Board
* Description   : Sample Program - ADC channel A7 user code
******************************************************************************/
/*******************************************************************************
* History       : DD.MM.YYYY Version Description
*               : 18.06.2013 1.00
*******************************************************************************/

/******************************************************************************
Includes   <System Includes> , "Project Includes"
******************************************************************************/
#include <stdio.h>
#include "r_typedefs.h"                   /* Default  type definition header */
#include "iodefine.h"                            /* I/O Register root header */
#include "dev_drv.h"                                 /* Device driver header */
#include "devdrv_intc.h"                               /* INTC Driver Header */
//#include "main.h"                               /* Main sample module header */
#include "rza_io_regrw.h"            /* Low level register read/write header */
#include "compiler_settings.h"   /* Interchangeable compiler specific header */


/******************************************************************************
Typedef definitions
******************************************************************************/

/******************************************************************************
Macro definitions
******************************************************************************/
#define ADC_DELAY       (10000)
#define ADC_PRI_ADI     (5uL)      /* ADI interrupt priority                 */

#define ADCSR_SET       (0x4000u)  /* ADCSR setting value                    */
#define STBCR6_7_SET    (0x80uL)   /* STBCR6  setting value                  */
#define PMC1_8_SET      (0x0100uL) /* P1 PMC  setting value                  */
#define PFCAE1_8_SET    (0xFEFFuL) /* P1 PFCAE  setting value                */
#define PFCE1_8_SET     (0xFEFFuL) /* P1 PFCE  setting value                 */
#define PFC1_8_SET      (0xFEFFuL) /* P1 PFC  setting value                  */
#define PM1_8_SET       (0x0100uL) /* P1 PM  setting value                   */
#define ADST_ON         (0x2000uL) /* ADC start                              */
#define ADST_OFF        (0xDFFFuL) /* ADC stop                               */
#define ADC_10BIT_SET   (0x03FFuL) /* 15-10bit 0 write ADCDRA                */
#define ADC_BIT_SHIFT_6 (0x0006uL) /* 6bit right shift ADCDRA                */
#define ADIE_ON         (0x4000uL) /* A/D conversion end interrupt enble     */
#define ADIE_OFF        (0xBFFFuL) /* A/D conversion end interrupt disabled  */
#define ADF_CLEAR       (0x7FFFuL) /* A/D conversion end interrupt ADF clear */
#define ADC_SET_FLG     (1u)       /* A/D conversion end interrupt ADF set   */
#define ADC_READ_ERR    ((uint32_t)-1)  /* Timeout error                    */

#define DELAY_TIME      (2000000uL)/* Sampling loop                          */
#define TIME_OUT        (2000000uL)/* Timeout loop                           */


/******************************************************************************
Imported global variables and functions (from other files)
******************************************************************************/

/******************************************************************************
Exported global variables and functions (to be accessed by other files)
******************************************************************************/

/******************************************************************************
Private global variables and functions
******************************************************************************/

uint8_t g_adc_complete_flag = 0u;
uint32_t                g_adc_result  = 0u;
volatile static uint8_t g_adc_end_flg = 0u;


static void     AdcStart(void);
static void     AdcStop(void);
static uint32_t     AdcReadSingle(void);
static void     AdcInitialise(void);
static void     AdcUninitialise(void);
static void     IntAdcEnd(uint32_t sense);

/******************************************************************************
* Function Name: AdcInitialise
* Description  : ADC Initialise (Single mode, Analog input AN7)
* Arguments    : none
* Return Value : none
******************************************************************************/
static void AdcInitialise(void)
{
    uint8_t  stbcr6_tmp;

    /* Configure ADC
    Channel AN0, Single mode, AD conversion end interrupt enable,
    Conversion time =  256tcyc(10bit)
    ADCSR   - A/D  control/status register
    b15     - A/D end flag              - Write 0 to clear flag
    b14     - A/D interrupt enable      - AD conversion end interrupt enable
    b13     - A/D start                 - Stop
    b12-9   - Timer Trigger set         - Disables start A/D conversion by external trigger
    b8-6    - Clock Select              - Conversion time = 256tcyc(10bit)
    b5-3    - Multi-scan mode           - Single Mode
    b2-0    - Channel Select            - AN0 */
    ADCSR = ADCSR_SET;

    /* IRQ interrupt disabled */
    __disable_fiq();

    /* FIQ interrupt disabled */
    __disable_irq();

    /* Configure Low-power mode register
    ADC clock supply,Other settings are saved
    STBCR6  - Standby control register 6
    b7      - Module Stop 67            - ADC clock supply
    b6-0    - Module Stop 66-63         - No setting */
    stbcr6_tmp = CPG.STBCR6.BYTE;
    CPG.STBCR6.BYTE = (uint8_t)((uint32_t)stbcr6_tmp & STBCR6_7_SET);

    /* Configure P1_8 as AN0 */
    RZA_IO_RegWrite_16(&GPIO.PM1,   1, GPIO_PM1_PM18_SHIFT,    GPIO_PM1_PM18);
    RZA_IO_RegWrite_16(&GPIO.PMC1,  1, GPIO_PMC1_PMC18_SHIFT,  GPIO_PMC1_PMC18);
    RZA_IO_RegWrite_16(&GPIO.PFCAE1,0, GPIO_PFCAE1_PFCAE18_SHIFT,GPIO_PFCAE1_PFCAE18);
    RZA_IO_RegWrite_16(&GPIO.PFCE1, 0, GPIO_PFCE1_PFCE18_SHIFT, GPIO_PFCE1_PFCE18);
    RZA_IO_RegWrite_16(&GPIO.PFC1,  0, GPIO_PFC1_PFC18_SHIFT,  GPIO_PFC1_PFC18);
    RZA_IO_RegWrite_16(&GPIO.PIPC1, 1, GPIO_PIPC1_PIPC18_SHIFT, GPIO_PIPC1_PIPC18);

    /* static variable initialization */
    g_adc_end_flg = 0u;

    /* IRQ interrupt Enable */
    __enable_irq();

    /* FIQ interrupt Enable */
    __enable_fiq();

}

/******************************************************************************
* Function Name: AdcStart
* Description  : ADC start
* Arguments    : none
* Return Value : none
******************************************************************************/
static void AdcStart(void)
{
    uint16_t adcsr_tmp;

    /* Configure ADC
    ADC Start, Other settings are saved
    ADCSR   - A/D  control/status register
    b15     - A/D end flag              - No setting
    b14     - A/D interrupt enable      - No setting
    b13     - A/D start                 - Start
    b12-9   - Timer Trigger set         - No setting
    b8-6    - Clock Select              - No setting
    b5-3    - Multi-scan mode           - No setting
    b2-0    - Channel Select            - 111 */
    adcsr_tmp = ADCSR;
    ADCSR= (uint16_t)((uint32_t)adcsr_tmp | ADST_ON);
}


/******************************************************************************
* Function Name: AdcStop
* Description  : ADC stop
* Arguments    : none
* Return Value : none
******************************************************************************/
static void AdcStop(void)
{
    uint16_t adcsr_tmp;

    /* Configure ADC
    ADC Stop, Other settings are saved
    ADCSR     - A/D  control/status register
    b15       - A/D end flag            - No setting
    b14       - A/D interrupt enable    - No setting
    b13       - A/D start               - Stop
    b12-9     - Timer Trigger set       - No setting
    b8-6      - Clock Select            - No setting
    b5-3      - Multi-scan mode         - No setting
    b2-0      - Channel Select          - No setting */
    adcsr_tmp = ADCSR;
    ADCSR = (uint16_t)((uint32_t)adcsr_tmp & ADST_OFF);
}


/******************************************************************************
* Function Name: AdcReadSingle
* Description  : AN7 A/D conversion
* Arguments    : none
* Return Value : A/D conversion value
*              : Time out error
******************************************************************************/
static uint32_t AdcReadSingle(void)
{
    uint32_t adc_time_count;
    uint32_t  adc_data_A7;

    adc_time_count = 0u;

    /* g_adc_end_flg Wait */
    do
    {
        if (TIME_OUT <= adc_time_count)
        {
            /* Time out */
           break;
        }

        /* Time count up */
        adc_time_count++;
    }
    while (ADC_SET_FLG != (uint32_t)g_adc_end_flg);

    if(TIME_OUT <= adc_time_count)
    {
        /* ADC stop */
        AdcStop();
        /* Timeout error */
        adc_data_A7 = ADC_READ_ERR;
    }
    else
    {
        /* Normal processing 6bit right shift ADCDR  */
        adc_data_A7 = (int32_t)(((uint32_t)ADCADDRA >> ADC_BIT_SHIFT_6) & ADC_10BIT_SET);
    }

    /* ADI interrupt disabled */
    R_INTC_Disable(INTC_ID_ADI);

    /* g_adc_end_flg clear */
    g_adc_end_flg = 0u;

    /* ADI interrupt Enable */
    R_INTC_Enable(INTC_ID_ADI);

    return adc_data_A7;
}

/******************************************************************************
* Function Name: AdcUninitialize
* Description  : ADC Uninitialize
* Arguments    : none
* Return Value : none
******************************************************************************/
static void AdcUninitialise(void)
{
    /* Configure ADC
    ADC All clear
    ADCSR     - A/D  control/status register */
    ADCSR = 0u;
}

/******************************************************************************
* Function Name: IntAdcEnd
* Description  : A/D conversion end interrupt
*              : Set g_adc_end_flg of Inform conversion end
* Arguments    : Type of interrupt request (unused)
* Return Value : none
******************************************************************************/
static void IntAdcEnd(uint32_t int_sense)
{
    uint16_t adcsr_tmp;

    UNUSED_PARAM(int_sense);


    g_adc_end_flg = ADC_SET_FLG;

   	/* Clear ADC flag */
	if (0x8000u == (ADCSR & 0x8000u))
	{
		/* Clear ADC complete flag */
		ADCSR &= 0x7FFFu;
	}

	/* Read the result register for AN7 */
	g_adc_result = (uint32_t)ADCADDRA >> 4u;

	/* Check if the upper and lower limits have been read */
	if ((uint32_t)0u == g_adc_result)
	{
		g_adc_complete_flag |= 0x80u;
	}

	if ((uint32_t)4090u < g_adc_result)
	{
		g_adc_complete_flag |= 0x01u;
	}

    /* -> MISRA 10.6 */
    /* Configure ADC
    Clear of A/D end flag, Other settings are saved
    ADCSR   - A/D control/status register
    b15     - A/D end flag              - CLEAR
    b14     - A/D interrupt enable      - No setting
    b13     - A/D start                 - No setting
    b12-9   - Timer Trigger set         - No setting
    b8-6    - Clock Select              - No setting
    b5-3    - Multi-scan mode           - No setting
    b2-0    - Channel Select            - No setting */
    adcsr_tmp = ADCSR;
    ADCSR   = (uint16_t)((uint32_t)adcsr_tmp & ADF_CLEAR);
}


/******************************************************************************
* Interface Functions
******************************************************************************/

/******************************************************************************
* Function Name: R_ADC_Open
* Description  : Enables and configures AN0 ADC channel
* Arguments    : none
* Return Value : 0
******************************************************************************/
void R_ADC_Open(void)
{
    /* Registration interrupt function  */
    R_INTC_RegistIntFunc(INTC_ID_ADI, &IntAdcEnd);

    /* Set interrupt priority */
    R_INTC_SetPriority(INTC_ID_ADI, ADC_PRI_ADI);

    /* A/D end interrupt enable */
    R_INTC_Enable(INTC_ID_ADI);

    /* ADC initialise */
    AdcInitialise();

}
/******************************************************************************
* End of Function R_ADC_Open
******************************************************************************/

/******************************************************************************
* Function Name: R_ADC_Close
* Description  : Disables the ADC
* Arguments    : none
* Return Value : 0
******************************************************************************/
void R_ADC_Close(void)
{
    /* A/D end interrupt disable */
    R_INTC_Disable(INTC_ID_ADI);

    /* ADC Uninitialise */
    AdcUninitialise();

}

/******************************************************************************
* Function Name: R_ADC_Read
* Description  : Read value from AN0
* Arguments    : none
* Return Value : AN0
******************************************************************************/
uint32_t R_ADC_Read(void)
{
	/* ADC start */
	AdcStart();

	/* Wait for conversion to end */
	while ( g_adc_end_flg != ADC_SET_FLG)
	{
		__asm__("nop");
	}

    g_adc_result = AdcReadSingle();

	/* ADC start */
	AdcStop();

    return g_adc_result;
}

/* End of File */

