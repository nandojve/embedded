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
* Copyright (C) 2013 Renesas Electronics Corporation. All rights reserved.
*******************************************************************************/
/******************************************************************************
* File Name     : led.c
* Device(s)     : RZ/A1H RSK+RZA1H
* H/W Platform  : RSK+RZA1H CPU Board
* Description   : Sample Program - LED setting
*               : Note
*               : LED0 is connected directly to processor
*               : LED1, LED2, LED3 are connected via port expander 1
******************************************************************************/
/******************************************************************************
* History       : DD.MM.YYYY Version Description
*               : 18.06.2013 1.00
******************************************************************************/

/******************************************************************************
Includes   <System Includes> , "Project Includes"
******************************************************************************/
#include "r_typedefs.h"                   /* Default  type definition header */
#include "iodefine.h"                            /* I/O Register root header */
#include "dev_drv.h"                          /* Device Driver common header */
#include "sample_led.h"       /* Interface for directly connected LED header */
#include "sample_riic_rza1h_rsk_i2c.h"   /* I2C RSK+RZA1H Eval-board header */
#include "rza_io_regrw.h"            /* Low level register read/write header */
#include "compiler_settings.h"   /* Interchangeable compiler specific header */

/******************************************************************************
Typedef definitions
******************************************************************************/

/******************************************************************************
Macro definitions
******************************************************************************/

/******************************************************************************
Imported global variables and functions (from other files)
******************************************************************************/

/******************************************************************************
Exported global variables and functions (to be accessed by other files)
******************************************************************************/

/******************************************************************************
Private global variables and functions
******************************************************************************/

/******************************************************************************
* Function Name: R_LED_Init
* Description  : Initialize port for LED.
* Arguments    : none
* Return Value : none
******************************************************************************/
void R_LED_Init(void)
{
    /* ---- P7_1 : LED0 direct connection to IP */
    RZA_IO_RegWrite_16(&GPIO.PMC7,  0, GPIO_PMC7_PMC71_SHIFT,  GPIO_PMC7_PMC71);
    RZA_IO_RegWrite_16(&GPIO.P7,  1, GPIO_P7_P71_SHIFT,  GPIO_P7_P71);
    RZA_IO_RegWrite_16(&GPIO.PM7,  0, GPIO_PM7_PM71_SHIFT,  GPIO_PM7_PM71);
    RZA_IO_RegWrite_16(&GPIO.PIPC7, 0, GPIO_PIPC7_PIPC71_SHIFT, GPIO_PIPC7_PIPC71);
}

/******************************************************************************
* End of Function R_LED_Init
******************************************************************************/

/*******************************************************************************
* Function Name: R_LED_On
* Description  : LED specified for "num" is turned on. 
* Arguments    : num desired LED to modify
* Return Value : none
*******************************************************************************/
void R_LED_On(uint8_t num)
{
	uint8_t PE_LEDs = LED1 | LED2 | LED3;

	if(LED0 == (num & LED0))
	{
        RZA_IO_RegWrite_16(&GPIO.P7,  0, GPIO_P7_P71_SHIFT,  GPIO_P7_P71);
    }

	if((num & PE_LEDs) > 0)
	{
		Sample_R_RIIC_LED_PE_On(num & PE_LEDs);
    }
}

/*******************************************************************************
* Function Name: R_LED_Off
* Description  : LED specified for "num" is turned off. 
* Arguments    : num desired LED to modify
* Return Value : none
*******************************************************************************/
void R_LED_Off(uint8_t num)
{
	uint8_t PE_LEDs = LED1 | LED2 | LED3;

	if(LED0 == (num & LED0))
	{
        RZA_IO_RegWrite_16(&GPIO.P7,  1, GPIO_P7_P71_SHIFT,  GPIO_P7_P71);
    }

	if((num & PE_LEDs) > 0)
	{
		Sample_R_RIIC_LED_PE_Off(num & PE_LEDs);
    }
}

/*******************************************************************************
* Function Name: R_LED_Toggle
* Description  : LED specified for "num" is toggled.
* Arguments    : num desired LED to modify
* Return Value : none
*******************************************************************************/
void R_LED_Toggle(uint8_t num)
{
	uint8_t PE_LEDs = LED1 | LED2 | LED3;

	if(LED0 == (num & LED0))
	{
		uint16_t tmp = RZA_IO_RegRead_16(&GPIO.P7,
				                          GPIO_P7_P71_SHIFT,
				                          GPIO_P7_P71);

		if(tmp)
		{
			RZA_IO_RegWrite_16(&GPIO.P7,  0, GPIO_P7_P71_SHIFT,  GPIO_P7_P71);
		}
		else
		{
			RZA_IO_RegWrite_16(&GPIO.P7,  1, GPIO_P7_P71_SHIFT,  GPIO_P7_P71);
		}
    }

	if((num & PE_LEDs) > 0)
	{
		Sample_R_RIIC_LED_PE_Toggle(num & PE_LEDs);
    }
}
/******************************************************************************
* End of Function R_LED_Off
******************************************************************************/


/* End of File */

