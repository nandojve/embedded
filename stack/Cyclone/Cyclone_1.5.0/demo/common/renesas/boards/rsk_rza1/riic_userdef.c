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
* File Name     : riic_userdef.c
* Device(s)     : RZ/A1H RSK+RZA1H
* H/W Platform  : RSK+RZA1H CPU Board
* Description   : RIIC driver (User define function)
*******************************************************************************/
/*******************************************************************************
* History       : DD.MM.YYYY Version Description
*               : 18.06.2013 1.00
*******************************************************************************/

/******************************************************************************
Includes   <System Includes> , "Project Includes"
******************************************************************************/
#include "r_typedefs.h"                   /* Default  type definition header */
#include "iodefine.h"                            /* I/O Register root header */
#include "dev_drv.h"                                 /* Device driver header */
#include "devdrv_riic.h"                               /* RIIC Driver header */
#include "devdrv_intc.h"                               /* INTC Driver Header */
#include "sample_riic_rza1h_rsk_i2c.h"   /* I2C RSK+RZA1H Eval-board header */
#include "compiler_settings.h"   /* Interchangeable compiler specific header */
#include "rza_io_regrw.h"            /* Low level register read/write header */

/******************************************************************************
Typedef definitions
******************************************************************************/
#define ICC0_MAX_TIMEOUT (20000)
#define ICC1_MAX_TIMEOUT (20000)
#define ICC2_MAX_TIMEOUT (20000)
#define ICC3_MAX_TIMEOUT (20000)

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
static volatile uint8_t riic0_receive_full_flg   = DEVDRV_FLAG_OFF;
static volatile uint8_t riic0_transmit_empty_flg = DEVDRV_FLAG_OFF;
static volatile uint8_t riic0_transmit_end_flg   = DEVDRV_FLAG_OFF;

/* Channel 1 Not in use
static volatile uint8_t riic1_receive_full_flg   = DEVDRV_FLAG_OFF;
static volatile uint8_t riic1_transmit_empty_flg = DEVDRV_FLAG_OFF;
static volatile uint8_t riic1_transmit_end_flg   = DEVDRV_FLAG_OFF;
*/

/* Channel 2 Not in use
static volatile uint8_t riic2_receive_full_flg   = DEVDRV_FLAG_OFF;
static volatile uint8_t riic2_transmit_empty_flg = DEVDRV_FLAG_OFF;
static volatile uint8_t riic2_transmit_end_flg   = DEVDRV_FLAG_OFF;
*/
static volatile uint8_t riic3_receive_full_flg   = DEVDRV_FLAG_OFF;
static volatile uint8_t riic3_transmit_empty_flg = DEVDRV_FLAG_OFF;
static volatile uint8_t riic3_transmit_end_flg   = DEVDRV_FLAG_OFF;

/******************************************************************************
* Function Name: Userdef_RIIC0_Init
* Description  : User-defined function.
*              : Called to initialise the RIIC channel 0 peripheral
*              : Uses the following interrupts:
*              : Receive data full interrupt
*              : Transmit data empty interrupt
*              : Transmission complete interrupt
* Arguments    : uint32_t cks     : I2c Clock Selection
*              :                  : (Settings in the CKS bit of RIICnMR1 register)
*              :                  :   RIIC_CKS_DIVISION_1   : P0φ/1 clock
*              :                  :   RIIC_CKS_DIVISION_2   : P0φ/2 clock
*              :                  :   RIIC_CKS_DIVISION_4   : P0φ/4 clock
*              :                  :   RIIC_CKS_DIVISION_8   : P0φ/8 clock
*              :                  :   RIIC_CKS_DIVISION_16  : P0φ/16 clock
*              :                  :   RIIC_CKS_DIVISION_32  : P0φ/32 clock
*              :                  :   RIIC_CKS_DIVISION_64  : P0φ/64 clock
*              :                  :   RIIC_CKS_DIVISION_128 : P0φ/128 clock
*              : uint32_t brl     : Lower byte value of the width of the SCL clock
*              : uint32_t brh     : High byte value of the width of the SCL clock
* Return Value : none
******************************************************************************/
void Userdef_RIIC0_Init(uint32_t cks, uint32_t brl, uint32_t brh)
{
    volatile uint8_t dummy_buf = 0u;

    UNUSED_VARIABLE(dummy_buf);

    /* ==== Module standby clear ==== */
    /* ---- Supply clock to the RIIC(channel 0) ---- */
    RZA_IO_RegWrite_8((uint8_t *)&CPG.STBCR9,
    		          0,
    		          CPG_STBCR9_MSTP97_SHIFT,
    		          CPG_STBCR9_MSTP97);

    dummy_buf = RZA_IO_RegRead_8((uint8_t *)&CPG.STBCR9,
    		                     CPG_STBCR9_MSTP97_SHIFT,
    		                     CPG_STBCR9_MSTP97);

    /* ==== RIIC initial setting ==== */
    /* Clear the RIICn reset bit */
    RZA_IO_RegWrite_8(&(RIIC0.RIICnCR1.UINT8[0]),
    		            0,
    		            RIICn_RIICnCR1_ICE_SHIFT,
    		            RIICn_RIICnCR1_ICE);

    /* Set I2C Bus Interface Internal Reset */
    RZA_IO_RegWrite_8(&(RIIC0.RIICnCR1.UINT8[0]),
    		            1,
    		            RIICn_RIICnCR1_IICRST_SHIFT,
    		            RIICn_RIICnCR1_IICRST);

    /* Set the RIICn reset bit */
    RZA_IO_RegWrite_8(&(RIIC0.RIICnCR1.UINT8[0]),
    		            1,
    		            RIICn_RIICnCR1_ICE_SHIFT,
    		            RIICn_RIICnCR1_ICE);

    /* Set Internal Reference Clock (IICφ) Selection */
    RZA_IO_RegWrite_8(&(RIIC0.RIICnMR1.UINT8[0]),
    		            (uint8_t)cks,
    		            RIICn_RIICnMR1_CKS_SHIFT,
    		            RIICn_RIICnMR1_CKS);

    /* Set Bit Rate Low-Level Period */
    RZA_IO_RegWrite_8(&(RIIC0.RIICnBRL.UINT8[0]),
    		            (uint8_t)brl,
    		            RIICn_RIICnBRL_BRL_SHIFT,
    		            RIICn_RIICnBRL_BRL);

    /* Set Bit Rate High-Level Period */
    RZA_IO_RegWrite_8(&(RIIC0.RIICnBRH.UINT8[0]),
                        (uint8_t)brh,
    		            RIICn_RIICnBRH_BRH_SHIFT,
    		            RIICn_RIICnBRH_BRH);

    /* Set NACK Reception Transfer Suspension Enable */
    RZA_IO_RegWrite_8(&(RIIC0.RIICnFER.UINT8[0]),
    		            1,
    		            RIICn_RIICnFER_NACKE_SHIFT,
    		            RIICn_RIICnFER_NACKE);

    /* Set Receive Data Full Interrupt Enable */
    RZA_IO_RegWrite_8(&(RIIC0.RIICnIER.UINT8[0]),
    		            1,
    		            RIICn_RIICnIER_RIE_SHIFT,
    		            RIICn_RIICnIER_RIE);

    /* Set Transmit Data Empty Interrupt Enable */
    RZA_IO_RegWrite_8(&(RIIC0.RIICnIER.UINT8[0]),
    		            1,
    		            RIICn_RIICnIER_TIE_SHIFT,
    		            RIICn_RIICnIER_TIE);

    /*Set Transmit End Interrupt Enable */
    RZA_IO_RegWrite_8(&(RIIC0.RIICnIER.UINT8[0]),
    		            1,
    		            RIICn_RIICnIER_TEIE_SHIFT,
    		            RIICn_RIICnIER_TEIE);

    /* Clear I2C Bus Interface Internal Reset */
    RZA_IO_RegWrite_8(&(RIIC0.RIICnCR1.UINT8[0]),
    		            0,
    		            RIICn_RIICnCR1_IICRST_SHIFT,
    		            RIICn_RIICnCR1_IICRST);

    /* Initialise flags */
    riic0_receive_full_flg   = DEVDRV_FLAG_OFF;
    riic0_transmit_empty_flg = DEVDRV_FLAG_OFF;
    riic0_transmit_end_flg   = DEVDRV_FLAG_OFF;

    /* Register active interrupts */
    R_INTC_RegistIntFunc(INTC_ID_RI0,  Sample_RIIC_Ri0_Interrupt);
    R_INTC_RegistIntFunc(INTC_ID_TI0,  Sample_RIIC_Ti0_Interrupt);
    R_INTC_RegistIntFunc(INTC_ID_TEI0, Sample_RIIC_Tei0_Interrupt);

    /* Set active interrupt priorities */
    R_INTC_SetPriority(INTC_ID_RI0,  9);
    R_INTC_SetPriority(INTC_ID_TI0,  9);
    R_INTC_SetPriority(INTC_ID_TEI0, 9);

    /* Enable active interrupts */
    R_INTC_Enable(INTC_ID_RI0);
    R_INTC_Enable(INTC_ID_TI0);
    R_INTC_Enable(INTC_ID_TEI0);
}

/******************************************************************************
* Function Name: Userdef_RIIC1_Init
* Description  : User-defined function.
*              : Called to initialise the RIIC channel 1 peripheral
*              : Uses the following interrupts:
*              : Receive data full interrupt
*              : Transmit data empty interrupt
*              : Transmission complete interrupt
* Arguments    : uint32_t cks     : I2c Clock Selection
*              :                  : (Settings in the CKS bit of RIICnMR1 register)
*              :                  :   RIIC_CKS_DIVISION_1   : P0φ/1 clock
*              :                  :   RIIC_CKS_DIVISION_2   : P0φ/2 clock
*              :                  :   RIIC_CKS_DIVISION_4   : P0φ/4 clock
*              :                  :   RIIC_CKS_DIVISION_8   : P0φ/8 clock
*              :                  :   RIIC_CKS_DIVISION_16  : P0φ/16 clock
*              :                  :   RIIC_CKS_DIVISION_32  : P0φ/32 clock
*              :                  :   RIIC_CKS_DIVISION_64  : P0φ/64 clock
*              :                  :   RIIC_CKS_DIVISION_128 : P0φ/128 clock
*              : uint32_t brl     : Lower byte value of the width of the SCL clock
*              : uint32_t brh     : High byte value of the width of the SCL clock
* Return Value : none
******************************************************************************/
void Userdef_RIIC1_Init(uint32_t cks, uint32_t brl, uint32_t brh)
{
}

/******************************************************************************
* Function Name: Userdef_RIIC2_Init
* Description  : User-defined function.
*              : Called to initialise the RIIC channel 2 peripheral
*              : Uses the following interrupts:
*              : Receive data full interrupt
*              : Transmit data empty interrupt
*              : Transmission complete interrupt
* Arguments    : uint32_t cks     : I2c Clock Selection
*              :                  : (Settings in the CKS bit of RIICnMR1 register)
*              :                  :   RIIC_CKS_DIVISION_1   : P0φ/1 clock
*              :                  :   RIIC_CKS_DIVISION_2   : P0φ/2 clock
*              :                  :   RIIC_CKS_DIVISION_4   : P0φ/4 clock
*              :                  :   RIIC_CKS_DIVISION_8   : P0φ/8 clock
*              :                  :   RIIC_CKS_DIVISION_16  : P0φ/16 clock
*              :                  :   RIIC_CKS_DIVISION_32  : P0φ/32 clock
*              :                  :   RIIC_CKS_DIVISION_64  : P0φ/64 clock
*              :                  :   RIIC_CKS_DIVISION_128 : P0φ/128 clock
*              : uint32_t brl     : Lower byte value of the width of the SCL clock
*              : uint32_t brh     : High byte value of the width of the SCL clock
* Return Value : none
******************************************************************************/
void Userdef_RIIC2_Init(uint32_t cks, uint32_t brl, uint32_t brh)
{
}

/******************************************************************************
* Function Name: Userdef_RIIC3_Init
* Description  : User-defined function.
*              : Called to initialise the RIIC channel 3 peripheral
*              : Uses the following interrupts:
*              : Receive data full interrupt
*              : Transmit data empty interrupt
*              : Transmission complete interrupt
* Arguments    : uint32_t cks     : I2c Clock Selection
*              :                  : (Settings in the CKS bit of RIICnMR1 register)
*              :                  :   RIIC_CKS_DIVISION_1   : P0φ/1 clock
*              :                  :   RIIC_CKS_DIVISION_2   : P0φ/2 clock
*              :                  :   RIIC_CKS_DIVISION_4   : P0φ/4 clock
*              :                  :   RIIC_CKS_DIVISION_8   : P0φ/8 clock
*              :                  :   RIIC_CKS_DIVISION_16  : P0φ/16 clock
*              :                  :   RIIC_CKS_DIVISION_32  : P0φ/32 clock
*              :                  :   RIIC_CKS_DIVISION_64  : P0φ/64 clock
*              :                  :   RIIC_CKS_DIVISION_128 : P0φ/128 clock
*              : uint32_t brl     : Lower byte value of the width of the SCL clock
*              : uint32_t brh     : High byte value of the width of the SCL clock
* Return Value : none
******************************************************************************/
void Userdef_RIIC3_Init(uint32_t cks, uint32_t brl, uint32_t brh)
{
    volatile uint8_t dummy_buf = 0u;

    UNUSED_VARIABLE(dummy_buf);

    /* ==== Module standby clear ==== */
    /* ---- Supply clock to the RIIC(channel 3) ---- */
    RZA_IO_RegWrite_8((uint8_t *)&CPG.STBCR9,
    		          0,
    		          CPG_STBCR9_MSTP94_SHIFT,
    		          CPG_STBCR9_MSTP94);

    dummy_buf = RZA_IO_RegRead_8((uint8_t *)&CPG.STBCR9,
    		                     CPG_STBCR9_MSTP94_SHIFT,
    		                     CPG_STBCR9_MSTP94);

    /* Clear the RIICn reset bit */
    RZA_IO_RegWrite_8(&(RIIC3.RIICnCR1.UINT8[0]),
    		            0,
    		            RIICn_RIICnCR1_ICE_SHIFT,
    		            RIICn_RIICnCR1_ICE);

    /* Set I2C Bus Interface Internal Reset */
    RZA_IO_RegWrite_8(&(RIIC3.RIICnCR1.UINT8[0]),
    		            1,
    		            RIICn_RIICnCR1_IICRST_SHIFT,
    		            RIICn_RIICnCR1_IICRST);

    /* Set the RIICn reset bit */
    RZA_IO_RegWrite_8(&(RIIC3.RIICnCR1.UINT8[0]),
    		            1,
    		            RIICn_RIICnCR1_ICE_SHIFT,
    		            RIICn_RIICnCR1_ICE);

    /* Set Internal Reference Clock (IICφ) Selection */
    RZA_IO_RegWrite_8(&(RIIC3.RIICnMR1.UINT8[0]),
                        (uint8_t)cks,
    		            RIICn_RIICnMR1_CKS_SHIFT,
    		            RIICn_RIICnMR1_CKS);

    /* Set Bit Rate Low-Level Period */
    RZA_IO_RegWrite_8(&(RIIC3.RIICnBRL.UINT8[0]),
    		            (uint8_t)brl,
    		            RIICn_RIICnBRL_BRL_SHIFT,
    		            RIICn_RIICnBRL_BRL);

    /* Set Bit Rate High-Level Period */
    RZA_IO_RegWrite_8(&(RIIC3.RIICnBRH.UINT8[0]),
                        (uint8_t)brh,
    		            RIICn_RIICnBRH_BRH_SHIFT,
    		            RIICn_RIICnBRH_BRH);

    /* Set NACK Reception Transfer Suspension Enable */
    RZA_IO_RegWrite_8(&(RIIC3.RIICnFER.UINT8[0]),
    		            1,
    		            RIICn_RIICnFER_NACKE_SHIFT,
    		            RIICn_RIICnFER_NACKE);

    /* Set Receive Data Full Interrupt Enable */
    RZA_IO_RegWrite_8(&(RIIC3.RIICnIER.UINT8[0]),
    		            1,
    		            RIICn_RIICnIER_RIE_SHIFT,
    		            RIICn_RIICnIER_RIE);

    /* Set Transmit Data Empty Interrupt Enable */
    RZA_IO_RegWrite_8(&(RIIC3.RIICnIER.UINT8[0]),
    		            1,
    		            RIICn_RIICnIER_TIE_SHIFT,
    		            RIICn_RIICnIER_TIE);

    /*Set Transmit End Interrupt Enable */
    RZA_IO_RegWrite_8(&(RIIC3.RIICnIER.UINT8[0]),
    		            1,
    		            RIICn_RIICnIER_TEIE_SHIFT,
    		            RIICn_RIICnIER_TEIE);

    /* Clear I2C Bus Interface Internal Reset */
    RZA_IO_RegWrite_8(&(RIIC3.RIICnCR1.UINT8[0]),
    		            0,
    		            RIICn_RIICnCR1_IICRST_SHIFT,
    		            RIICn_RIICnCR1_IICRST);

    /* Initialise flags */
    riic3_receive_full_flg   = DEVDRV_FLAG_OFF;
    riic3_transmit_empty_flg = DEVDRV_FLAG_OFF;
    riic3_transmit_end_flg   = DEVDRV_FLAG_OFF;

    /* Register active interrupts */
    R_INTC_RegistIntFunc(INTC_ID_RI3,  Sample_RIIC_Ri3_Interrupt);
    R_INTC_RegistIntFunc(INTC_ID_TI3,  Sample_RIIC_Ti3_Interrupt);
    R_INTC_RegistIntFunc(INTC_ID_TEI3, Sample_RIIC_Tei3_Interrupt);

    /* Set active interrupt priorities */
    R_INTC_SetPriority(INTC_ID_RI3,  9);
    R_INTC_SetPriority(INTC_ID_TI3,  9);
    R_INTC_SetPriority(INTC_ID_TEI3, 9);

    /* Enable active interrupts */
    R_INTC_Enable(INTC_ID_RI3);
    R_INTC_Enable(INTC_ID_TI3);
    R_INTC_Enable(INTC_ID_TEI3);
}

/******************************************************************************
* Function Name: Userdef_RIIC0_InitReceiveFull
* Description  : User-defined function.
*              : Called to process INTC_ID_INTIICRI for channel 0
* Arguments    : none
* Return Value : none
******************************************************************************/
void Userdef_RIIC0_InitReceiveFull(void)
{
    riic0_receive_full_flg = DEVDRV_FLAG_OFF;
}

/******************************************************************************
* Function Name: Userdef_RIIC1_InitReceiveFull
* Description  : User-defined function.
*              : Called to process INTC_ID_INTIICRI for channel 1
* Arguments    : none
* Return Value : none
******************************************************************************/
void Userdef_RIIC1_InitReceiveFull(void)
{
}

/******************************************************************************
* Function Name: Userdef_RIIC2_InitReceiveFull
* Description  : User-defined function.
*              : Called to process INTC_ID_INTIICRI for channel 2
* Arguments    : none
* Return Value : none
******************************************************************************/
void Userdef_RIIC2_InitReceiveFull(void)
{
}

/******************************************************************************
* Function Name: Userdef_RIIC3_InitReceiveFull
* Description  : User-defined function.
*              : Called to process INTC_ID_INTIICRI for channel 3
* Arguments    : none
* Return Value : none
******************************************************************************/
void Userdef_RIIC3_InitReceiveFull(void)
{
	riic3_receive_full_flg = DEVDRV_FLAG_OFF;
}

/******************************************************************************
* Function Name: Userdef_RIIC0_InitTransmitEmpty
* Description  : User-defined function.
*              : Called to process INTC_ID_INTIICTI for channel 0
* Arguments    : none
* Return Value : none
******************************************************************************/
void Userdef_RIIC0_InitTransmitEmpty(void)
{
    riic0_transmit_empty_flg = DEVDRV_FLAG_OFF;
}

/******************************************************************************
* Function Name: Userdef_RIIC1_InitTransmitEmpty
* Description  : User-defined function.
*              : Called to process INTC_ID_INTIICTI for channel 1
* Arguments    : none
* Return Value : none
******************************************************************************/
void Userdef_RIIC1_InitTransmitEmpty(void)
{
}

/******************************************************************************
* Function Name: Userdef_RIIC2_InitTransmitEmpty
* Description  : User-defined function.
*              : Called to process INTC_ID_INTIICTI for channel 2
* Arguments    : none
* Return Value : none
******************************************************************************/
void Userdef_RIIC2_InitTransmitEmpty(void)
{
}

/******************************************************************************
* Function Name: Userdef_RIIC3_InitTransmitEmpty
* Description  : User-defined function.
*              : Called to process INTC_ID_INTIICTI for channel 3
* Arguments    : none
* Return Value : none
******************************************************************************/
void Userdef_RIIC3_InitTransmitEmpty(void)
{
	riic3_transmit_empty_flg = DEVDRV_FLAG_OFF;
}

/******************************************************************************
* Function Name: Userdef_RIIC0_InitTransmitEnd
* Description  : User-defined function.
*              : Called to process INTC_ID_INTIICTEI for channel 0
* Arguments    : none
* Return Value : none
******************************************************************************/
void Userdef_RIIC0_InitTransmitEnd(void)
{
    riic0_transmit_end_flg = DEVDRV_FLAG_OFF;
}

/******************************************************************************
* Function Name: Userdef_RIIC1_InitTransmitEnd
* Description  : User-defined function.
*              : Called to process INTC_ID_INTIICTEI for channel 1
* Arguments    : none
* Return Value : none
******************************************************************************/
void Userdef_RIIC1_InitTransmitEnd(void)
{
}

/******************************************************************************
* Function Name: Userdef_RIIC2_InitTransmitEnd
* Description  : User-defined function.
*              : Called to process INTC_ID_INTIICTEI for channel 2
* Arguments    : none
* Return Value : none
******************************************************************************/
void Userdef_RIIC2_InitTransmitEnd(void)
{
}

/******************************************************************************
* Function Name: Userdef_RIIC3_InitTransmitEnd
* Description  : User-defined function.
*              : Called to process INTC_ID_INTIICTEI for channel 3
* Arguments    : none
* Return Value : none
******************************************************************************/
void Userdef_RIIC3_InitTransmitEnd(void)
{
	riic3_transmit_end_flg = DEVDRV_FLAG_OFF;
}

/******************************************************************************
* Function Name: Userdef_RIIC0_SetReceiveFull
* Description  : User-defined function.
*              : Set receive full flag for channel 0
* Arguments    : none
* Return Value : none
******************************************************************************/
void Userdef_RIIC0_SetReceiveFull(void)
{
    riic0_receive_full_flg = DEVDRV_FLAG_ON;
}

/******************************************************************************
* Function Name: Userdef_RIIC1_SetReceiveFull
* Description  : User-defined function.
*              : Set receive full flag for channel 1
* Arguments    : none
* Return Value : none
******************************************************************************/
void Userdef_RIIC1_SetReceiveFull(void)
{
}

/******************************************************************************
* Function Name: Userdef_RIIC2_SetReceiveFull
* Description  : User-defined function.
*              : Set receive full flag for channel 2
* Arguments    : none
* Return Value : none
******************************************************************************/
void Userdef_RIIC2_SetReceiveFull(void)
{
}

/******************************************************************************
* Function Name: Userdef_RIIC3_SetReceiveFull
* Description  : User-defined function.
*              : Set receive full flag for channel 3
* Arguments    : none
* Return Value : none
******************************************************************************/
void Userdef_RIIC3_SetReceiveFull(void)
{
	riic0_receive_full_flg = DEVDRV_FLAG_ON;
}

/******************************************************************************
* Function Name: Userdef_RIIC0_SetTransmitEmpty
* Description  : User-defined function.
*              : Set transmit empty flag for channel 0
* Arguments    : none
* Return Value : none
******************************************************************************/
void Userdef_RIIC0_SetTransmitEmpty(void)
{
    riic0_transmit_empty_flg = DEVDRV_FLAG_ON;
}

/******************************************************************************
* Function Name: Userdef_RIIC1_SetTransmitEmpty
* Description  : User-defined function.
*              : Set transmit empty flag for channel 1
* Arguments    : none
* Return Value : none
******************************************************************************/
void Userdef_RIIC1_SetTransmitEmpty(void)
{
}

/******************************************************************************
* Function Name: Userdef_RIIC2_SetTransmitEmpty
* Description  : User-defined function.
*              : Set transmit empty flag for channel 2
* Arguments    : none
* Return Value : none
******************************************************************************/
void Userdef_RIIC2_SetTransmitEmpty(void)
{
}

/******************************************************************************
* Function Name: Userdef_RIIC3_SetTransmitEmpty
* Description  : User-defined function.
*              : Set transmit empty flag for channel 3
* Arguments    : none
* Return Value : none
******************************************************************************/
void Userdef_RIIC3_SetTransmitEmpty(void)
{
	riic3_transmit_empty_flg = DEVDRV_FLAG_ON;
}

/******************************************************************************
* Function Name: Userdef_RIIC0_SetTransmitEnd
* Description  : User-defined function.
*              : Set transmit end flag for channel 0
* Arguments    : none
* Return Value : none
******************************************************************************/
void Userdef_RIIC0_SetTransmitEnd(void)
{
    riic0_transmit_end_flg = DEVDRV_FLAG_ON;
}

/******************************************************************************
* Function Name: Userdef_RIIC1_SetTransmitEnd
* Description  : User-defined function.
*              : Set transmit end flag for channel 1
* Arguments    : none
* Return Value : none
******************************************************************************/
void Userdef_RIIC1_SetTransmitEnd(void)
{
}

/******************************************************************************
* Function Name: Userdef_RIIC2_SetTransmitEnd
* Description  : User-defined function.
*              : Set transmit end flag for channel 2
* Arguments    : none
* Return Value : none
******************************************************************************/
void Userdef_RIIC2_SetTransmitEnd(void)
{
}

/******************************************************************************
* Function Name: Userdef_RIIC3_SetTransmitEnd
* Description  : User-defined function.
*              : Set transmit end flag for channel 3
* Arguments    : none
* Return Value : none
******************************************************************************/
void Userdef_RIIC3_SetTransmitEnd(void)
{
	riic3_transmit_end_flg = DEVDRV_FLAG_ON;
}

/******************************************************************************
* Function Name: Userdef_RIIC0_WaitReceiveFull
* Description  : User-defined function.
*              : Wait in function till receive full flag set for channel 0
* Arguments    : none
* Return Value : none
******************************************************************************/
void Userdef_RIIC0_WaitReceiveFull(void)
{
	int32_t timeout = ICC0_MAX_TIMEOUT;

    while ((timeout != 0) & (DEVDRV_FLAG_OFF == riic0_receive_full_flg))
    {
    	timeout--;
        /* Wait */
    }
}

/******************************************************************************
* Function Name: Userdef_RIIC1_WaitReceiveFull
* Description  : User-defined function.
*              : Wait in function till receive full flag set for channel 1
* Arguments    : none
* Return Value : none
******************************************************************************/
void Userdef_RIIC1_WaitReceiveFull(void)
{
}

/******************************************************************************
* Function Name: Userdef_RIIC2_WaitReceiveFull
* Description  : User-defined function.
*              : Wait in function till receive full flag set for channel 2
* Arguments    : none
* Return Value : none
******************************************************************************/
void Userdef_RIIC2_WaitReceiveFull(void)
{
}

/******************************************************************************
* Function Name: Userdef_RIIC3_WaitReceiveFull
* Description  : User-defined function.
*              : Wait in function till receive full flag set for channel 3
* Arguments    : none
* Return Value : none
******************************************************************************/
void Userdef_RIIC3_WaitReceiveFull(void)
{
	int32_t timeout = ICC3_MAX_TIMEOUT;

    while ((timeout != 0) & (DEVDRV_FLAG_OFF == riic3_receive_full_flg))
    {
    	timeout--;
        /* Wait */
    }
}

/******************************************************************************
* Function Name: Userdef_RIIC0_WaitTransmitEmpty
* Description  : User-defined function.
*              : Wait in function till transmit empty flag set for channel 0
* Arguments    : none
* Return Value : none
******************************************************************************/
void Userdef_RIIC0_WaitTransmitEmpty(void)
{
	int32_t timeout = ICC0_MAX_TIMEOUT;

    while ((timeout != 0) & (DEVDRV_FLAG_OFF == riic0_transmit_empty_flg))
    {
    	timeout--;
        /* Wait */
    }
}

/******************************************************************************
* Function Name: Userdef_RIIC1_WaitTransmitEmpty
* Description  : User-defined function.
*              : Wait in function till transmit empty flag set for channel 1
* Arguments    : none
* Return Value : none
******************************************************************************/
void Userdef_RIIC1_WaitTransmitEmpty(void)
{
}

/******************************************************************************
* Function Name: Userdef_RIIC2_WaitTransmitEmpty
* Description  : User-defined function.
*              : Wait in function till transmit empty flag set for channel 2
* Arguments    : none
* Return Value : none
******************************************************************************/
void Userdef_RIIC2_WaitTransmitEmpty(void)
{
}

/******************************************************************************
* Function Name: Userdef_RIIC3_WaitTransmitEmpty
* Description  : User-defined function.
*              : Wait in function till transmit empty flag set for channel 3
* Arguments    : none
* Return Value : none
******************************************************************************/
void Userdef_RIIC3_WaitTransmitEmpty(void)
{
	int32_t timeout = ICC3_MAX_TIMEOUT;

    while ((timeout != 0) & (DEVDRV_FLAG_OFF == riic3_transmit_empty_flg))
    {
    	timeout--;
        /* Wait */
    }
}

/******************************************************************************
* Function Name: Userdef_RIIC0_WaitTransmitEnd
* Description  : User-defined function.
*              : Wait in function till transmit end flag set for channel 0
* Arguments    : uint32_t mode : RIIC_TEND_WAIT_TRANSMIT : Transmission Mode
*              :               : RIIC_TEND_WAIT_RECEIVE  : Receive Mode
* Return Value : none
******************************************************************************/
void Userdef_RIIC0_WaitTransmitEnd(uint32_t mode)
{
    if (RIIC_TEND_WAIT_TRANSMIT == mode)
    {
    	int32_t timeout = ICC0_MAX_TIMEOUT;

        while ((timeout != 0) & (DEVDRV_FLAG_OFF == riic0_transmit_end_flg))
        {
        	timeout--;
            /* Wait */
        }
    }
    else
    {
        /* Reception Mode */
    	int32_t timeout = ICC0_MAX_TIMEOUT;

        while ((timeout != 0) & (DEVDRV_FLAG_OFF == riic0_receive_full_flg))
        {
        	timeout--;
            /* Wait */
        }
    }
}

/******************************************************************************
* Description  : User-defined function.
*              : Wait in function till transmit end flag set for channel 1
* Arguments    : uint32_t mode : RIIC_TEND_WAIT_TRANSMIT : Transmission Mode
*              :               : RIIC_TEND_WAIT_RECEIVE  : Receive Mode
* Return Value : none
******************************************************************************/
void Userdef_RIIC1_WaitTransmitEnd(uint32_t mode)
{
}

/******************************************************************************
* Function Name: Userdef_RIIC2_WaitTransmitEnd
* Description  : User-defined function.
*              : Wait in function till transmit end flag set for channel 1
* Arguments    : uint32_t mode : RIIC_TEND_WAIT_TRANSMIT : Transmission Mode
*              :               : RIIC_TEND_WAIT_RECEIVE  : Receive Mode
* Return Value : none
******************************************************************************/
void Userdef_RIIC2_WaitTransmitEnd(uint32_t mode)
{
}

/******************************************************************************
* Function Name: Userdef_RIIC3_WaitTransmitEnd
* Description  : User-defined function.
*              : Wait in function till transmit end flag set for channel 3
* Arguments    : uint32_t mode : RIIC_TEND_WAIT_TRANSMIT : Transmission Mode
*              :               : RIIC_TEND_WAIT_RECEIVE  : Receive Mode
* Return Value : none
******************************************************************************/
void Userdef_RIIC3_WaitTransmitEnd(uint32_t mode)
{
    if (RIIC_TEND_WAIT_TRANSMIT == mode)
    {
    	int32_t timeout = ICC3_MAX_TIMEOUT;

        while ((timeout != 0) & (DEVDRV_FLAG_OFF == riic3_transmit_end_flg))
        {
        	timeout--;
            /* Wait */
        }
    }
    else
    {
        /* Reception Mode */
    	int32_t timeout = ICC3_MAX_TIMEOUT;

        while ((timeout != 0) & (DEVDRV_FLAG_OFF == riic3_receive_full_flg))
        {
        	timeout--;
            /* Wait */
        }
    }
}

/******************************************************************************
* Function Name: Userdef_RIIC0_WaitBusMastership
* Description  : User-defined function.
*              : Wait for bus busy or bus free state for channel 0
* Arguments    : uint32_t mode
*              :        : RIIC_BUS_MASTERSHIP_WAIT_FREE : wait till bus free
*              :        : RIIC_BUS_MASTERSHIP_WAIT_BUSY : wait till bus busy
* Return Value : none
******************************************************************************/
void Userdef_RIIC0_WaitBusMastership(uint32_t mode)
{
	int32_t timeout = ICC0_MAX_TIMEOUT;
    if (RIIC_BUS_MASTERSHIP_WAIT_FREE == mode)
    {
        while ((timeout != 0) &
        	   (1 == RZA_IO_RegRead_8(&(RIIC0.RIICnCR2.UINT8[0]),
                                        RIICn_RIICnCR2_BBSY_SHIFT,
                                        RIICn_RIICnCR2_BBSY)))
        {
            /* Wait */
        	timeout--;
        }
    }
    else
    {
        while ((timeout != 0) &
               (0 == RZA_IO_RegRead_8(&(RIIC0.RIICnCR2.UINT8[0]),
            		                    RIICn_RIICnCR2_BBSY_SHIFT,
            		                    RIICn_RIICnCR2_BBSY)))
        {
            /* Wait */
        	timeout--;
        }
    }
}

/******************************************************************************
* Function Name: Userdef_RIIC1_WaitBusMastership
* Description  : User-defined function.
*              : Wait for bus busy or bus free state for channel 1
* Arguments    : uint32_t mode
*              :        : RIIC_BUS_MASTERSHIP_WAIT_FREE : wait till bus free
*              :        : RIIC_BUS_MASTERSHIP_WAIT_BUSY : wait till bus busy
* Return Value : none
******************************************************************************/
void Userdef_RIIC1_WaitBusMastership(uint32_t mode)
{
}

/******************************************************************************
* Function Name: Userdef_RIIC2_WaitBusMastership
* Description  : User-defined function.
*              : Wait for bus busy or bus free state for channel 2
* Arguments    : uint32_t mode
*              :        : RIIC_BUS_MASTERSHIP_WAIT_FREE : wait till bus free
*              :        : RIIC_BUS_MASTERSHIP_WAIT_BUSY : wait till bus busy
* Return Value : none
******************************************************************************/
void Userdef_RIIC2_WaitBusMastership(uint32_t mode)
{
}

/******************************************************************************
* Function Name: Userdef_RIIC3_WaitBusMastership
* Description  : User-defined function.
*              : Wait for bus busy or bus free state for channel 3
* Arguments    : uint32_t mode
*              :        : RIIC_BUS_MASTERSHIP_WAIT_FREE : wait till bus free
*              :        : RIIC_BUS_MASTERSHIP_WAIT_BUSY : wait till bus busy
* Return Value : none
******************************************************************************/
void Userdef_RIIC3_WaitBusMastership(uint32_t mode)
{
	int32_t timeout = ICC0_MAX_TIMEOUT;

	if (RIIC_BUS_MASTERSHIP_WAIT_FREE == mode)
    {
    	/* Wait till bus free */
        while ((timeout != 0) &
               (1 == RZA_IO_RegRead_8(&(RIIC3.RIICnCR2.UINT8[0]),
            		                    RIICn_RIICnCR2_BBSY_SHIFT,
            		                    RIICn_RIICnCR2_BBSY)))
        {
            /* Wait */
        	timeout--;
        }
    }
    else
    {
    	/* Wait till bus busy */
        while ((timeout != 0) &
               (0 == RZA_IO_RegRead_8(&(RIIC3.RIICnCR2.UINT8[0]),
                                        RIICn_RIICnCR2_BBSY_SHIFT,
                                        RIICn_RIICnCR2_BBSY)))
        {
            /* Wait */
        	timeout--;
        }
    }
}

/******************************************************************************
* Function Name: Userdef_RIIC0_WaitStop
* Description  : User-defined function.
*              : Wait for stop bit state for channel 0
* Arguments    : none
* Return Value : none
******************************************************************************/
void Userdef_RIIC0_WaitStop(void)
{
	int32_t timeout = ICC0_MAX_TIMEOUT;

    while ((timeout != 0) &
           (0 == RZA_IO_RegRead_8(&(RIIC0.RIICnSR2.UINT8[0]),
                                    RIICn_RIICnSR2_STOP_SHIFT,
                                    RIICn_RIICnSR2_STOP)))
    {
        /* Wait */
    	timeout--;
    }
}

/******************************************************************************
* Function Name: Userdef_RIIC1_WaitStop
* Description  : User-defined function.
*              : Wait for stop bit state for channel 1
* Arguments    : none
* Return Value : none
******************************************************************************/
void Userdef_RIIC1_WaitStop(void)
{
}

/******************************************************************************
* Function Name: Userdef_RIIC2_WaitStop
* Description  : User-defined function.
*              : Wait for stop bit state for channel 2
* Arguments    : none
* Return Value : none
******************************************************************************/
void Userdef_RIIC2_WaitStop(void)
{
}

/******************************************************************************
* Function Name: Userdef_RIIC3_WaitStop
* Description  : User-defined function.
*              : Wait for stop bit state for channel 3
* Arguments    : none
* Return Value : none
******************************************************************************/
void Userdef_RIIC3_WaitStop(void)
{
	int32_t timeout = ICC0_MAX_TIMEOUT;

    while ((timeout != 0) &
           (0 == RZA_IO_RegRead_8(&(RIIC3.RIICnSR2.UINT8[0]),
        		                    RIICn_RIICnSR2_STOP_SHIFT,
        		                    RIICn_RIICnSR2_STOP)))
    {
        /* Wait */
    	timeout--;
    }
}


/* End of File */

