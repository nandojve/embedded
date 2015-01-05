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
* File Name     : sample_sw.c
* Device(s)     : RZ/A1H RSK+RZA1H
* H/W Platform  : RSK+RZA1H CPU Board
* Description   : Sample Program - Switch handling function
*               : Note 
*               : LED0, LED1, LED2, LED3 are connected directly to processor
******************************************************************************/
/******************************************************************************
* History       : DD.MM.YYYY Version Description
*               : 18.06.2013 1.00
******************************************************************************/

/******************************************************************************
Includes   <System Includes> , "Project Includes"
******************************************************************************/
#include "r_typedefs.h"
#include "iodefine.h"
#include "dev_drv.h"                          /* Device Driver common header */
#include "devdrv_intc.h"
#include "sample_swi.h"                    /* Sample Switch interface header */
//#include "main.h"
#include "compiler_settings.h"
#include "devdrv_ostm.h"                               /* OSTM module header */

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
volatile uint8_t  g_switch_press_flg = 0u;

/******************************************************************************
Private global variables and functions
******************************************************************************/
void Sample_OSTM1_Interrupt(uint32_t int_sense);

/******************************************************************************
* Function Name: IntIRQ6
* Description  : IRQ4 interrupt
*              : Handler for IRQ6 interrupts
* Arguments    : uint32_t int_sense : Interrupt detection
*              :                    :   INTC_LEVEL_SENSITIVE : Level sense
*              :                    :   INTC_EDGE_TRIGGER    : Edge trigger
* Return Value : none
******************************************************************************/
void IntIRQ6 (uint32_t sense)
{
    // Write Data
    g_switch_press_flg |= SW1_SET_FLG;

    // Read Data
    g_switch_press_flg |= SW2_SET_FLG;

    // Inc Index
    g_switch_press_flg |= SW3_SET_FLG;

}
/******************************************************************************
* End of Function IntIRQ6
******************************************************************************/

/******************************************************************************
* Interface Functions
******************************************************************************/

/******************************************************************************
* Function Name: R_SWI_Open
* Description  : configures user switches (SW1,SW2,SW3) on target board
* Arguments    : none
* Return Value : none
******************************************************************************/
void R_SWI_Open(void)
{
    /* Use OSTM1 for polling of switch interrupts */
	/* Set SW1 (IRQ3) as input pin (P1_9) */
    PORT1.PBDCn.BIT.PBDCn9    = 1;
	PORT1.PMn.BIT.PMn9   	  = 1;
	PORT1.Pn.BIT.Pn9		  = 1;
	PORT1.PMCn.BIT.PMCn9      = 1;
	PORT1.PFCAEn.BIT.PFCAEn9  = 0;
	PORT1.PFCEn.BIT.PFCEn9    = 1;
	PORT1.PFCn.BIT.PFCn9      = 0;
	PORT1.PIPCn.BIT.PIPCn9    = 1;

	/* Set SW2 (IRQ2) as input pin (P1_8) */
    PORT1.PBDCn.BIT.PBDCn8    = 1;
	PORT1.PMn.BIT.PMn8   	  = 1;
	PORT1.Pn.BIT.Pn8		  = 1;
	PORT1.PMCn.BIT.PMCn8      = 1;
	PORT1.PFCAEn.BIT.PFCAEn8  = 0;
	PORT1.PFCEn.BIT.PFCEn8    = 1;
	PORT1.PFCn.BIT.PFCn8      = 0;
	PORT1.PIPCn.BIT.PIPCn8    = 1;

	/* Set SW3 (IRQ5) as input pin (P1_11) */
    PORT1.PBDCn.BIT.PBDCn11   = 1;
	PORT1.PMn.BIT.PMn11    	  = 1;
	PORT1.Pn.BIT.Pn11		  = 1;
	PORT1.PMCn.BIT.PMCn11     = 1;
	PORT1.PFCAEn.BIT.PFCAEn11 = 0;
	PORT1.PFCEn.BIT.PFCEn11   = 1;
	PORT1.PFCn.BIT.PFCn11     = 0;
	PORT1.PIPCn.BIT.PIPCn11   = 1;

    R_OSTM_Init(DEVDRV_CH_1, OSTM_MODE_INTERVAL, 100);

    R_OSTM_Open(DEVDRV_CH_1);
}
/******************************************************************************
* End of Function R_SWI_Open
******************************************************************************/

/******************************************************************************
* Function Name: R_SWI_Close
* Description  : Disables (SW1,SW2,SW3) on target board
* Arguments    : none
* Return Value : none
******************************************************************************/
void R_SWI_Close(void)
{
	uint32_t count;

    R_OSTM_Close(DEVDRV_CH_1, &count);
}
/******************************************************************************
* End of Function R_SWI_Open
******************************************************************************/

/******************************************************************************
* Function Name: Sample_OSTM1_Interrupt
* Description  : This function is executed when the OSTM0 interrupt is received.
*              : In this sample code, the processing to blink the LEDs on the
*              : CPU board every 500ms is executed.
* Arguments    : uint32_t int_sense : Interrupt detection
*              :                    :   INTC_LEVEL_SENSITIVE : Level sense
*              :                    :   INTC_EDGE_TRIGGER    : Edge trigger
* Return Value : none
******************************************************************************/
void Sample_OSTM1_Interrupt(uint32_t int_sense)
{
	g_switch_press_flg = 0;

	g_switch_press_flg = (uint8_t)(g_switch_press_flg + (PORT1.PPRn.BIT.PPRn9  ? 0 : SW1_SET_FLG));
	g_switch_press_flg = (uint8_t)(g_switch_press_flg + (PORT1.PPRn.BIT.PPRn8  ? 0 : SW2_SET_FLG));
	g_switch_press_flg = (uint8_t)(g_switch_press_flg + (PORT1.PPRn.BIT.PPRn11 ? 0 : SW3_SET_FLG));

}
/******************************************************************************
* End of Function Sample_OSTM1_Interrupt
******************************************************************************/

/* End of File */

