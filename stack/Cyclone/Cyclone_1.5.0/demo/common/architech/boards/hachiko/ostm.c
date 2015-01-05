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
/*******************************************************************************
* File Name   : ostm.c
* Description : OS timer driver
*******************************************************************************/


/******************************************************************************
Includes   <System Includes> , "Project Includes"
******************************************************************************/
#include "r_typedefs.h"
#include "dev_drv.h"                /* Device Driver common header */
#include "devdrv_ostm.h"            /* OSTM Driver header */
#include "iodefine.h"

/******************************************************************************
Typedef definitions
******************************************************************************/


/******************************************************************************
Macro definitions
******************************************************************************/
/* ==== OSTM H/W specification ==== */
#define OSTM_CH_TOTAL       (2)     /* The number of OSTM channels */

/******************************************************************************
Imported global variables and functions (from other files)
******************************************************************************/


/******************************************************************************
Exported global variables and functions (to be accessed by other files)
******************************************************************************/


/******************************************************************************
Private global variables and functions
******************************************************************************/
static void OSTM_Open(volatile struct st_ostm * ostm);
static void OSTM_Close(volatile struct st_ostm * ostm, uint32_t * count);


/******************************************************************************
* Function Name: R_OSTM_Init
* Description  : Executes initial setting for the OSTM specified by the argument
*              : channel. Sets the count value using the timer cycle specified 
*              : by the argument cycle. In this sample code, the initial setting
*              : is executed by the Userdef_OSTMn_Init of the user-defined
*              : function.
* Arguments    : uint32_t channel : OSTM channel (0 or 1)
*              : uint32_t mode    : OSTM operating mode
*              :                  :   OSTM_MODE_INTERVAL : Interval timer mode
*              :                  :   OSTM_MODE_COMPARE  : Free running compare mode
*              : uint32_t cycle   : Timer cycle
* Return Value : DEVDRV_SUCCESS   : Success of OSTM initialization
*              : DEVDRV_ERROR     : Failure of OSTM initialization
******************************************************************************/
int32_t R_OSTM_Init(uint32_t channel, uint32_t mode, uint32_t cycle)
{
    int32_t ret;

    /* ==== Argument check ==== */
    if ((channel >= OSTM_CH_TOTAL) || (mode > OSTM_MODE_COMPARE))
    {
        return DEVDRV_ERROR;        /* Argument error */
    }

    /* ==== OSTM initialization ==== */
    switch (channel)
    {
        case DEVDRV_CH_0:
#ifdef USE_CH_OSTM0
            ret = Userdef_OSTM0_Init(mode, cycle);
#endif /* USE_CH_OSTM0 */
        break;
        case DEVDRV_CH_1:
#ifdef USE_CH_OSTM1
            ret = Userdef_OSTM1_Init(mode, cycle);
#endif /* USE_CH_OSTM1 */
        break;
        default:
            /* Do not reach here based on the assumption */
            ret = DEVDRV_ERROR;
        break;
    }

    return ret;
}

/******************************************************************************
* Function Name: R_OSTM_Open
* Description  : Starts the OSTM timer counter specified by the argument channel.
* Arguments    : uint32_t channel : OSTM channel (0 or 1)
* Return Value : DEVDRV_SUCCESS   : Success to start counting OSTM
*              : DEVDRV_ERROR     : Failure to start counting OSTM
******************************************************************************/
int32_t R_OSTM_Open(uint32_t channel)
{
    /* ==== Argument check ==== */
    if (channel >= OSTM_CH_TOTAL)
    {
        return DEVDRV_ERROR;        /* Argument error */
    }

    /* ==== Start counting OSTM ==== */
    switch (channel)
    {
        case DEVDRV_CH_0:
            OSTM_Open(&OSTM0);
        break;
        case DEVDRV_CH_1:
            OSTM_Open(&OSTM1);
        break;
        default:
            /* Do not reach here based on the assumption */
        break;
    }

    return DEVDRV_SUCCESS;
}

/******************************************************************************
* Function Name: R_OSTM_Close
* Description  : Stops the OSTM timer counter specified by the argument channel.
*              : Returns the count value at the time the timer counter stopped 
*              : to the argument count.
* Arguments    : uint32_t channel : OSTM channel (0 or 1)
*              : uint32_t * count : Count value
* Return Value : DEVDRV_SUCCESS   : Success to stop counting OSTM
*              : DEVDRV_ERROR     : Failure to stop counting OSTM
******************************************************************************/
int32_t R_OSTM_Close(uint32_t channel, uint32_t * count)
{
    /* ==== Argument check ==== */
    if (channel >= OSTM_CH_TOTAL)
    {
        return DEVDRV_ERROR;        /* Argument error */
    }

    /* ==== Stop counting OSTM ==== */
    switch (channel)
    {
        case DEVDRV_CH_0:
            OSTM_Close(&OSTM0, count);
        break;
        case DEVDRV_CH_1:
            OSTM_Close(&OSTM1, count);
        break;
        default:
            /* Do not reach here based on the assumption */
        break;
    }

    return DEVDRV_SUCCESS;
}

/*******************************************************************************
* Function Name: OSTM_Open
* Description  : This function is a local function.
*              : Executes the register setting to start the timer counter.
* Arguments    : volatile struct st_ostm * ostm : Start address of OSTM register
*              :                                : by channel
* Return Value : none
*******************************************************************************/
static void OSTM_Open(volatile struct st_ostm * ostm)
{
    ostm->OSTMnTS = 0x01u;      /* Start counting */
}

/******************************************************************************
* Function Name: OSTM_Close
* Description  : This function is a local function.
*              : Executes the register setting to stop the timer counter.
* Arguments    : volatile struct st_ostm * ostm : Start address of OSTM register
*              :                                : by channel
*              : uint32_t * count : Count value
* Return Value : none
******************************************************************************/
static void OSTM_Close(volatile struct st_ostm * ostm, uint32_t * count)
{
    ostm->OSTMnTT = 0x01u;      /* Stop counting      */
    *count = ostm->OSTMnCNT;    /* Obtain count value */
}


/* End of File */

