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
* Copyright (C) 2012 Renesas Electronics Corporation. All rights reserved.
*******************************************************************************/
/*******************************************************************************
* File Name   : intc_handler.c
* $Rev: 19220 $
* $Date:: 2013-08-30 08:59:20 +0100#$
* Description : Aragon Sample Program - Handler process
*******************************************************************************/

/******************************************************************************
Includes   <System Includes> , "Project Includes"
******************************************************************************/
#include <stdio.h>
#include "r_typedefs.h"
#include "devdrv_intc.h"        /* INTC Driver Header */
#include "iodefine.h"
#include "intc_handler.h"
#include "compiler_settings.h"

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
/* ==== Prototype declaration ==== */
void INTC_Handler_Interrupt(uint32_t icciar);
// __irq void FiqHandler_Interrupt(void);
// TODO rcoze __irq void FiqHandler_Interrupt(void);

/******************************************************************************
Private global variables and functions
******************************************************************************/

/******************************************************************************
* Function Name: INTC_Handler_Interrupt
* Description  :
* Arguments    : uint32_t icciar
* Return Value : none
******************************************************************************/
void INTC_Handler_Interrupt(uint32_t icciar)
{
    uint32_t int_id;
    uint32_t mask;
    volatile uint32_t dummy_buf;

    int_id = icciar & 0x000003FF;

    __enable_irq();

    Userdef_INTC_HandlerExe(int_id);

    __disable_irq();

    mask = 1 << (((int_id % 16) * 2) + 1);
    if (0 == (INTC.ICDICFR.LONG[int_id / 16] & mask))
    {
        mask = 0x00000000;
        mask = ~(1 << (int_id % 32));
        INTC.ICDABR.LONG[int_id / 32] &= mask;

        dummy_buf = INTC.ICDABR.LONG[int_id / 32];

        if ((INTC_ID_USBI0 == int_id) || (INTC_ID_USBI1 == int_id))
        {
            dummy_buf = INTC.ICDABR.LONG[int_id / 32];
            dummy_buf = INTC.ICDABR.LONG[int_id / 32];
        }
    }
    else
    {
        /* Do Nothing */
    }
}

/*******************************************************************************
* Function Name: FiqHandler_Interrupt
* Description  : 
* Arguments    : none
* Return Value : none
*******************************************************************************/
void FiqHandler_Interrupt(void)
{
    Userdef_FIQ_HandlerExe();
}

/* END of File */

