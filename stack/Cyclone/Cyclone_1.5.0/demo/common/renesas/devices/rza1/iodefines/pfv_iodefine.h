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
* File Name : pfv_iodefine.h
* Description : Definition of I/O Register (V0.50j)
******************************************************************************/
#ifndef PFV_IODEFINE_H
#define PFV_IODEFINE_H

struct st_pfv
{                                                          /* PFV              */
    uint32_t       PFVCR;                                  /*  PFVCR           */
    uint32_t       PFVICR;                                 /*  PFVICR          */
    uint32_t       PFVISR;                                 /*  PFVISR          */
    uint8_t        dummy1[20];                             /*                  */
#define PFVID_COUNT 8
    uint32_t       PFVID0;                                 /*  PFVID0          */
    uint32_t       PFVID1;                                 /*  PFVID1          */
    uint32_t       PFVID2;                                 /*  PFVID2          */
    uint32_t       PFVID3;                                 /*  PFVID3          */
    uint32_t       PFVID4;                                 /*  PFVID4          */
    uint32_t       PFVID5;                                 /*  PFVID5          */
    uint32_t       PFVID6;                                 /*  PFVID6          */
    uint32_t       PFVID7;                                 /*  PFVID7          */
#define PFVOD_COUNT 8
    uint32_t       PFVOD0;                                 /*  PFVOD0          */
    uint32_t       PFVOD1;                                 /*  PFVOD1          */
    uint32_t       PFVOD2;                                 /*  PFVOD2          */
    uint32_t       PFVOD3;                                 /*  PFVOD3          */
    uint32_t       PFVOD4;                                 /*  PFVOD4          */
    uint32_t       PFVOD5;                                 /*  PFVOD5          */
    uint32_t       PFVOD6;                                 /*  PFVOD6          */
    uint32_t       PFVOD7;                                 /*  PFVOD7          */
    uint8_t        dummy2[4];                              /*                  */
    uint32_t       PFVIFSR;                                /*  PFVIFSR         */
    uint32_t       PFVOFSR;                                /*  PFVOFSR         */
    uint32_t       PFVACR;                                 /*  PFVACR          */
    uint32_t       PFV_MTX_MODE;                           /*  PFV_MTX_MODE    */
    uint32_t       PFV_MTX_YG_ADJ0;                        /*  PFV_MTX_YG_ADJ0 */
    uint32_t       PFV_MTX_YG_ADJ1;                        /*  PFV_MTX_YG_ADJ1 */
    uint32_t       PFV_MTX_CBB_ADJ0;                       /*  PFV_MTX_CBB_ADJ0 */
    uint32_t       PFV_MTX_CBB_ADJ1;                       /*  PFV_MTX_CBB_ADJ1 */
    uint32_t       PFV_MTX_CRR_ADJ0;                       /*  PFV_MTX_CRR_ADJ0 */
    uint32_t       PFV_MTX_CRR_ADJ1;                       /*  PFV_MTX_CRR_ADJ1 */
    uint32_t       PFVSZR;                                 /*  PFVSZR          */
};


#define PFV0    (*(volatile struct st_pfv     *)0xE8205000uL) /* PFV0 */
#define PFV1    (*(volatile struct st_pfv     *)0xE8205800uL) /* PFV1 */


/* Start of channnel array defines of PFV */

/* Channnel array defines of PFV */
/*(Sample) value = PFV[ channel ]->PFVCR; */
#define PFV_COUNT  2
#define PFV_ADDRESS_LIST \
    &PFV0, &PFV1

/* End of channnel array defines of PFV */


#define PFV0PFVCR PFV0.PFVCR
#define PFV0PFVICR PFV0.PFVICR
#define PFV0PFVISR PFV0.PFVISR
#define PFV0PFVID0 PFV0.PFVID0
#define PFV0PFVID1 PFV0.PFVID1
#define PFV0PFVID2 PFV0.PFVID2
#define PFV0PFVID3 PFV0.PFVID3
#define PFV0PFVID4 PFV0.PFVID4
#define PFV0PFVID5 PFV0.PFVID5
#define PFV0PFVID6 PFV0.PFVID6
#define PFV0PFVID7 PFV0.PFVID7
#define PFV0PFVOD0 PFV0.PFVOD0
#define PFV0PFVOD1 PFV0.PFVOD1
#define PFV0PFVOD2 PFV0.PFVOD2
#define PFV0PFVOD3 PFV0.PFVOD3
#define PFV0PFVOD4 PFV0.PFVOD4
#define PFV0PFVOD5 PFV0.PFVOD5
#define PFV0PFVOD6 PFV0.PFVOD6
#define PFV0PFVOD7 PFV0.PFVOD7
#define PFV0PFVIFSR PFV0.PFVIFSR
#define PFV0PFVOFSR PFV0.PFVOFSR
#define PFV0PFVACR PFV0.PFVACR
#define PFV0PFV_MTX_MODE PFV0.PFV_MTX_MODE
#define PFV0PFV_MTX_YG_ADJ0 PFV0.PFV_MTX_YG_ADJ0
#define PFV0PFV_MTX_YG_ADJ1 PFV0.PFV_MTX_YG_ADJ1
#define PFV0PFV_MTX_CBB_ADJ0 PFV0.PFV_MTX_CBB_ADJ0
#define PFV0PFV_MTX_CBB_ADJ1 PFV0.PFV_MTX_CBB_ADJ1
#define PFV0PFV_MTX_CRR_ADJ0 PFV0.PFV_MTX_CRR_ADJ0
#define PFV0PFV_MTX_CRR_ADJ1 PFV0.PFV_MTX_CRR_ADJ1
#define PFV0PFVSZR PFV0.PFVSZR
#define PFV1PFVCR PFV1.PFVCR
#define PFV1PFVICR PFV1.PFVICR
#define PFV1PFVISR PFV1.PFVISR
#define PFV1PFVID0 PFV1.PFVID0
#define PFV1PFVID1 PFV1.PFVID1
#define PFV1PFVID2 PFV1.PFVID2
#define PFV1PFVID3 PFV1.PFVID3
#define PFV1PFVID4 PFV1.PFVID4
#define PFV1PFVID5 PFV1.PFVID5
#define PFV1PFVID6 PFV1.PFVID6
#define PFV1PFVID7 PFV1.PFVID7
#define PFV1PFVOD0 PFV1.PFVOD0
#define PFV1PFVOD1 PFV1.PFVOD1
#define PFV1PFVOD2 PFV1.PFVOD2
#define PFV1PFVOD3 PFV1.PFVOD3
#define PFV1PFVOD4 PFV1.PFVOD4
#define PFV1PFVOD5 PFV1.PFVOD5
#define PFV1PFVOD6 PFV1.PFVOD6
#define PFV1PFVOD7 PFV1.PFVOD7
#define PFV1PFVIFSR PFV1.PFVIFSR
#define PFV1PFVOFSR PFV1.PFVOFSR
#define PFV1PFVACR PFV1.PFVACR
#define PFV1PFV_MTX_MODE PFV1.PFV_MTX_MODE
#define PFV1PFV_MTX_YG_ADJ0 PFV1.PFV_MTX_YG_ADJ0
#define PFV1PFV_MTX_YG_ADJ1 PFV1.PFV_MTX_YG_ADJ1
#define PFV1PFV_MTX_CBB_ADJ0 PFV1.PFV_MTX_CBB_ADJ0
#define PFV1PFV_MTX_CBB_ADJ1 PFV1.PFV_MTX_CBB_ADJ1
#define PFV1PFV_MTX_CRR_ADJ0 PFV1.PFV_MTX_CRR_ADJ0
#define PFV1PFV_MTX_CRR_ADJ1 PFV1.PFV_MTX_CRR_ADJ1
#define PFV1PFVSZR PFV1.PFVSZR
#endif
