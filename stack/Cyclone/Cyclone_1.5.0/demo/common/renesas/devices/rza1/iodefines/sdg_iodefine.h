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
* http://www.renesas.com/disclaimer*
* Copyright (C) 2013 Renesas Electronics Corporation. All rights reserved.
*******************************************************************************/
/*******************************************************************************
* File Name : sdg_iodefine.h
* Description : Definition of I/O Register (V0.50j)
******************************************************************************/
#ifndef SDG_IODEFINE_H
#define SDG_IODEFINE_H

struct st_sdg
{                                                          /* SDG              */
    uint8_t        SGCR1;                                  /*  SGCR1           */
    uint8_t        SGCSR;                                  /*  SGCSR           */
    uint8_t        SGCR2;                                  /*  SGCR2           */
    uint8_t        SGLR;                                   /*  SGLR            */
    uint8_t        SGTFR;                                  /*  SGTFR           */
    uint8_t        SGSFR;                                  /*  SGSFR           */
};


#define SDG0    (*(volatile struct st_sdg     *)0xFCFF4800uL) /* SDG0 */
#define SDG1    (*(volatile struct st_sdg     *)0xFCFF4A00uL) /* SDG1 */
#define SDG2    (*(volatile struct st_sdg     *)0xFCFF4C00uL) /* SDG2 */
#define SDG3    (*(volatile struct st_sdg     *)0xFCFF4E00uL) /* SDG3 */


/* Start of channnel array defines of SDG */

/* Channnel array defines of SDG */
/*(Sample) value = SDG[ channel ]->SGCR1; */
#define SDG_COUNT  4
#define SDG_ADDRESS_LIST \
    &SDG0, &SDG1, &SDG2, &SDG3

/* End of channnel array defines of SDG */


#define SGCR1_0 SDG0.SGCR1
#define SGCSR_0 SDG0.SGCSR
#define SGCR2_0 SDG0.SGCR2
#define SGLR_0 SDG0.SGLR
#define SGTFR_0 SDG0.SGTFR
#define SGSFR_0 SDG0.SGSFR
#define SGCR1_1 SDG1.SGCR1
#define SGCSR_1 SDG1.SGCSR
#define SGCR2_1 SDG1.SGCR2
#define SGLR_1 SDG1.SGLR
#define SGTFR_1 SDG1.SGTFR
#define SGSFR_1 SDG1.SGSFR
#define SGCR1_2 SDG2.SGCR1
#define SGCSR_2 SDG2.SGCSR
#define SGCR2_2 SDG2.SGCR2
#define SGLR_2 SDG2.SGLR
#define SGTFR_2 SDG2.SGTFR
#define SGSFR_2 SDG2.SGSFR
#define SGCR1_3 SDG3.SGCR1
#define SGCSR_3 SDG3.SGCSR
#define SGCR2_3 SDG3.SGCR2
#define SGLR_3 SDG3.SGLR
#define SGTFR_3 SDG3.SGTFR
#define SGSFR_3 SDG3.SGSFR
#endif
