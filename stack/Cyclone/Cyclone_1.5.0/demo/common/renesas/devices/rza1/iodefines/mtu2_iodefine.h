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
* File Name : mtu2_iodefine.h
* Description : Definition of I/O Register (V0.50j)
******************************************************************************/
#ifndef MTU2_IODEFINE_H
#define MTU2_IODEFINE_H

struct st_mtu2
{                                                          /* MTU2             */
    uint8_t        TCR_2;                                  /*  TCR_2           */
    uint8_t        TMDR_2;                                 /*  TMDR_2          */
    uint8_t        TIORH_2;                                /*  TIORH_2         */
    uint8_t        dummy533[1];                            /*                  */
    uint8_t        TIER_2;                                 /*  TIER_2          */
    uint8_t        TSR_2;                                  /*  TSR_2           */
    uint16_t       TCNT_2;                                 /*  TCNT_2          */
    uint16_t       TGRA_2;                                 /*  TGRA_2          */
    uint16_t       TGRB_2;                                 /*  TGRB_2          */
    uint8_t        dummy534[500];                          /*                  */
    uint8_t        TCR_3;                                  /*  TCR_3           */
    uint8_t        TCR_4;                                  /*  TCR_4           */
    uint8_t        TMDR_3;                                 /*  TMDR_3          */
    uint8_t        TMDR_4;                                 /*  TMDR_4          */
    uint8_t        TIORH_3;                                /*  TIORH_3         */
    uint8_t        TIORL_3;                                /*  TIORL_3         */
    uint8_t        TIORH_4;                                /*  TIORH_4         */
    uint8_t        TIORL_4;                                /*  TIORL_4         */
    uint8_t        TIER_3;                                 /*  TIER_3          */
    uint8_t        TIER_4;                                 /*  TIER_4          */
    uint8_t        TOER;                                   /*  TOER            */
    uint8_t        dummy535[2];                            /*                  */
    uint8_t        TGCR;                                   /*  TGCR            */
    uint8_t        TOCR1;                                  /*  TOCR1           */
    uint8_t        TOCR2;                                  /*  TOCR2           */
    uint16_t       TCNT_3;                                 /*  TCNT_3          */
    uint16_t       TCNT_4;                                 /*  TCNT_4          */
    uint16_t       TCDR;                                   /*  TCDR            */
    uint16_t       TDDR;                                   /*  TDDR            */
    uint16_t       TGRA_3;                                 /*  TGRA_3          */
    uint16_t       TGRB_3;                                 /*  TGRB_3          */
    uint16_t       TGRA_4;                                 /*  TGRA_4          */
    uint16_t       TGRB_4;                                 /*  TGRB_4          */
    uint16_t       TCNTS;                                  /*  TCNTS           */
    uint16_t       TCBR;                                   /*  TCBR            */
    uint16_t       TGRC_3;                                 /*  TGRC_3          */
    uint16_t       TGRD_3;                                 /*  TGRD_3          */
    uint16_t       TGRC_4;                                 /*  TGRC_4          */
    uint16_t       TGRD_4;                                 /*  TGRD_4          */
    uint8_t        TSR_3;                                  /*  TSR_3           */
    uint8_t        TSR_4;                                  /*  TSR_4           */
    uint8_t        dummy536[2];                            /*                  */
    uint8_t        TITCR;                                  /*  TITCR           */
    uint8_t        TITCNT;                                 /*  TITCNT          */
    uint8_t        TBTER;                                  /*  TBTER           */
    uint8_t        dummy537[1];                            /*                  */
    uint8_t        TDER;                                   /*  TDER            */
    uint8_t        dummy538[1];                            /*                  */
    uint8_t        TOLBR;                                  /*  TOLBR           */
    uint8_t        dummy539[1];                            /*                  */
    uint8_t        TBTM_3;                                 /*  TBTM_3          */
    uint8_t        TBTM_4;                                 /*  TBTM_4          */
    uint8_t        dummy540[6];                            /*                  */
    uint16_t       TADCR;                                  /*  TADCR           */
    uint8_t        dummy541[2];                            /*                  */
    uint16_t       TADCORA_4;                              /*  TADCORA_4       */
    uint16_t       TADCORB_4;                              /*  TADCORB_4       */
    uint16_t       TADCOBRA_4;                             /*  TADCOBRA_4      */
    uint16_t       TADCOBRB_4;                             /*  TADCOBRB_4      */
    uint8_t        dummy542[20];                           /*                  */
    uint8_t        TWCR;                                   /*  TWCR            */
    uint8_t        dummy543[31];                           /*                  */
    uint8_t        TSTR;                                   /*  TSTR            */
    uint8_t        TSYR;                                   /*  TSYR            */
    uint8_t        dummy544[2];                            /*                  */
    uint8_t        TRWER;                                  /*  TRWER           */
    uint8_t        dummy545[123];                          /*                  */
    uint8_t        TCR_0;                                  /*  TCR_0           */
    uint8_t        TMDR_0;                                 /*  TMDR_0          */
    uint8_t        TIORH_0;                                /*  TIORH_0         */
    uint8_t        TIORL_0;                                /*  TIORL_0         */
    uint8_t        TIER_0;                                 /*  TIER_0          */
    uint8_t        TSR_0;                                  /*  TSR_0           */
    uint16_t       TCNT_0;                                 /*  TCNT_0          */
    uint16_t       TGRA_0;                                 /*  TGRA_0          */
    uint16_t       TGRB_0;                                 /*  TGRB_0          */
    uint16_t       TGRC_0;                                 /*  TGRC_0          */
    uint16_t       TGRD_0;                                 /*  TGRD_0          */
    uint8_t        dummy546[16];                           /*                  */
    uint16_t       TGRE_0;                                 /*  TGRE_0          */
    uint16_t       TGRF_0;                                 /*  TGRF_0          */
    uint8_t        TIER2_0;                                /*  TIER2_0         */
    uint8_t        TSR2_0;                                 /*  TSR2_0          */
    uint8_t        TBTM_0;                                 /*  TBTM_0          */
    uint8_t        dummy547[89];                           /*                  */
    uint8_t        TCR_1;                                  /*  TCR_1           */
    uint8_t        TMDR_1;                                 /*  TMDR_1          */
    uint8_t        TIOR_1;                                 /*  TIOR_1          */
    uint8_t        dummy548[1];                            /*                  */
    uint8_t        TIER_1;                                 /*  TIER_1          */
    uint8_t        TSR_1;                                  /*  TSR_1           */
    uint16_t       TCNT_1;                                 /*  TCNT_1          */
    uint16_t       TGRA_1;                                 /*  TGRA_1          */
    uint16_t       TGRB_1;                                 /*  TGRB_1          */
    uint8_t        dummy549[4];                            /*                  */
    uint8_t        TICCR;                                  /*  TICCR           */
};


#define MTU2    (*(volatile struct st_mtu2    *)0xFCFF0000uL) /* MTU2 */


#define MTU2TCR_2 MTU2.TCR_2
#define MTU2TMDR_2 MTU2.TMDR_2
#define MTU2TIORH_2 MTU2.TIORH_2
#define MTU2TIER_2 MTU2.TIER_2
#define MTU2TSR_2 MTU2.TSR_2
#define MTU2TCNT_2 MTU2.TCNT_2
#define MTU2TGRA_2 MTU2.TGRA_2
#define MTU2TGRB_2 MTU2.TGRB_2
#define MTU2TCR_3 MTU2.TCR_3
#define MTU2TCR_4 MTU2.TCR_4
#define MTU2TMDR_3 MTU2.TMDR_3
#define MTU2TMDR_4 MTU2.TMDR_4
#define MTU2TIORH_3 MTU2.TIORH_3
#define MTU2TIORL_3 MTU2.TIORL_3
#define MTU2TIORH_4 MTU2.TIORH_4
#define MTU2TIORL_4 MTU2.TIORL_4
#define MTU2TIER_3 MTU2.TIER_3
#define MTU2TIER_4 MTU2.TIER_4
#define MTU2TOER MTU2.TOER
#define MTU2TGCR MTU2.TGCR
#define MTU2TOCR1 MTU2.TOCR1
#define MTU2TOCR2 MTU2.TOCR2
#define MTU2TCNT_3 MTU2.TCNT_3
#define MTU2TCNT_4 MTU2.TCNT_4
#define MTU2TCDR MTU2.TCDR
#define MTU2TDDR MTU2.TDDR
#define MTU2TGRA_3 MTU2.TGRA_3
#define MTU2TGRB_3 MTU2.TGRB_3
#define MTU2TGRA_4 MTU2.TGRA_4
#define MTU2TGRB_4 MTU2.TGRB_4
#define MTU2TCNTS MTU2.TCNTS
#define MTU2TCBR MTU2.TCBR
#define MTU2TGRC_3 MTU2.TGRC_3
#define MTU2TGRD_3 MTU2.TGRD_3
#define MTU2TGRC_4 MTU2.TGRC_4
#define MTU2TGRD_4 MTU2.TGRD_4
#define MTU2TSR_3 MTU2.TSR_3
#define MTU2TSR_4 MTU2.TSR_4
#define MTU2TITCR MTU2.TITCR
#define MTU2TITCNT MTU2.TITCNT
#define MTU2TBTER MTU2.TBTER
#define MTU2TDER MTU2.TDER
#define MTU2TOLBR MTU2.TOLBR
#define MTU2TBTM_3 MTU2.TBTM_3
#define MTU2TBTM_4 MTU2.TBTM_4
#define MTU2TADCR MTU2.TADCR
#define MTU2TADCORA_4 MTU2.TADCORA_4
#define MTU2TADCORB_4 MTU2.TADCORB_4
#define MTU2TADCOBRA_4 MTU2.TADCOBRA_4
#define MTU2TADCOBRB_4 MTU2.TADCOBRB_4
#define MTU2TWCR MTU2.TWCR
#define MTU2TSTR MTU2.TSTR
#define MTU2TSYR MTU2.TSYR
#define MTU2TRWER MTU2.TRWER
#define MTU2TCR_0 MTU2.TCR_0
#define MTU2TMDR_0 MTU2.TMDR_0
#define MTU2TIORH_0 MTU2.TIORH_0
#define MTU2TIORL_0 MTU2.TIORL_0
#define MTU2TIER_0 MTU2.TIER_0
#define MTU2TSR_0 MTU2.TSR_0
#define MTU2TCNT_0 MTU2.TCNT_0
#define MTU2TGRA_0 MTU2.TGRA_0
#define MTU2TGRB_0 MTU2.TGRB_0
#define MTU2TGRC_0 MTU2.TGRC_0
#define MTU2TGRD_0 MTU2.TGRD_0
#define MTU2TGRE_0 MTU2.TGRE_0
#define MTU2TGRF_0 MTU2.TGRF_0
#define MTU2TIER2_0 MTU2.TIER2_0
#define MTU2TSR2_0 MTU2.TSR2_0
#define MTU2TBTM_0 MTU2.TBTM_0
#define MTU2TCR_1 MTU2.TCR_1
#define MTU2TMDR_1 MTU2.TMDR_1
#define MTU2TIOR_1 MTU2.TIOR_1
#define MTU2TIER_1 MTU2.TIER_1
#define MTU2TSR_1 MTU2.TSR_1
#define MTU2TCNT_1 MTU2.TCNT_1
#define MTU2TGRA_1 MTU2.TGRA_1
#define MTU2TGRB_1 MTU2.TGRB_1
#define MTU2TICCR MTU2.TICCR
#endif
