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
* File Name     : ostm_iobitmask.h
* Device(s)     : RZ/A1H RSK2+RZA1H
* H/W Platform  : RSK2+RZA1H CPU Board
* Description   : OSTM register define header
*******************************************************************************/
/*******************************************************************************
* History       : DD.MM.YYYY Version Description
*               : 18.06.2013 1.00
*******************************************************************************/

#ifndef OSTM_IOBITMASK_H
#define OSTM_IOBITMASK_H


/* ==== Mask values for IO registers ==== */
/* ---- OSTM0 ---- */
#define OSTM0_OSTMnCMP_OSTMnCMP                 (0xFFFFFFFFuL)

#define OSTM0_OSTMnCNT_OSTMnCNT                 (0xFFFFFFFFuL)

#define OSTM0_OSTMnTE_OSTMnTE                   (0x01u)

#define OSTM0_OSTMnTS_OSTMnTS                   (0x01u)

#define OSTM0_OSTMnTT_OSTMnTT                   (0x01u)

#define OSTM0_OSTMnCTL_MD0                      (0x00000001uL)
#define OSTM0_OSTMnCTL_MD1                      (0x00000002uL)

/* ---- OSTM1 ---- */
#define OSTM1_OSTMnCMP_OSTMnCMP                 (0xFFFFFFFFuL)

#define OSTM1_OSTMnCNT_OSTMnCNT                 (0xFFFFFFFFuL)

#define OSTM1_OSTMnTE_OSTMnTE                   (0x01u)

#define OSTM1_OSTMnTS_OSTMnTS                   (0x01u)

#define OSTM1_OSTMnTT_OSTMnTT                   (0x01u)

#define OSTM1_OSTMnCTL_MD0                      (0x00000001uL)
#define OSTM1_OSTMnCTL_MD1                      (0x00000002uL)


/* ==== Shift values for IO registers ==== */
/* ---- OSTM0 ---- */
#define OSTM0_OSTMnCMP_OSTMnCMP_SHIFT           (0u)

#define OSTM0_OSTMnCNT_OSTMnCNT_SHIFT           (0u)

#define OSTM0_OSTMnTE_OSTMnTE_SHIFT             (0u)

#define OSTM0_OSTMnTS_OSTMnTS_SHIFT             (0u)

#define OSTM0_OSTMnTT_OSTMnTT_SHIFT             (0u)

#define OSTM0_OSTMnCTL_MD0_SHIFT                (0u)
#define OSTM0_OSTMnCTL_MD1_SHIFT                (1u)

/* ---- OSTM1 ---- */
#define OSTM1_OSTMnCMP_OSTMnCMP_SHIFT           (0u)

#define OSTM1_OSTMnCNT_OSTMnCNT_SHIFT           (0u)

#define OSTM1_OSTMnTE_OSTMnTE_SHIFT             (0u)

#define OSTM1_OSTMnTS_OSTMnTS_SHIFT             (0u)

#define OSTM1_OSTMnTT_OSTMnTT_SHIFT             (0u)

#define OSTM1_OSTMnCTL_MD0_SHIFT                (0u)
#define OSTM1_OSTMnCTL_MD1_SHIFT                (1u)


#endif /* OSTM_IOBITMASK_H */

/* End of File */
