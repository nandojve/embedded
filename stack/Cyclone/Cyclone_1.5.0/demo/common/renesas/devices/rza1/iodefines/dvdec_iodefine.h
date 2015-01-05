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
* File Name : dvdec_iodefine.h
* Description : Definition of I/O Register (V0.50j)
******************************************************************************/
#ifndef DVDEC_IODEFINE_H
#define DVDEC_IODEFINE_H

struct st_dvdec
{                                                          /* DVDEC            */
    uint16_t       ADCCR1;                                 /*  ADCCR1          */
    uint8_t        dummy1[4];                              /*                  */
#define DVDEC_TGCRn_COUNT 3
    uint16_t       TGCR1;                                  /*  TGCR1           */
    uint16_t       TGCR2;                                  /*  TGCR2           */
    uint16_t       TGCR3;                                  /*  TGCR3           */
    uint8_t        dummy2[6];                              /*                  */
#define DVDEC_SYNSCRn_COUNT 5
    uint16_t       SYNSCR1;                                /*  SYNSCR1         */
    uint16_t       SYNSCR2;                                /*  SYNSCR2         */
    uint16_t       SYNSCR3;                                /*  SYNSCR3         */
    uint16_t       SYNSCR4;                                /*  SYNSCR4         */
    uint16_t       SYNSCR5;                                /*  SYNSCR5         */
#define DVDEC_HAFCCRn_COUNT 3
    uint16_t       HAFCCR1;                                /*  HAFCCR1         */
    uint16_t       HAFCCR2;                                /*  HAFCCR2         */
    uint16_t       HAFCCR3;                                /*  HAFCCR3         */
    uint16_t       VCDWCR1;                                /*  VCDWCR1         */
    uint8_t        dummy3[4];                              /*                  */
#define DVDEC_DCPCRn_COUNT 8
    uint16_t       DCPCR1;                                 /*  DCPCR1          */
    uint16_t       DCPCR2;                                 /*  DCPCR2          */
    uint16_t       DCPCR3;                                 /*  DCPCR3          */
    uint16_t       DCPCR4;                                 /*  DCPCR4          */
    uint16_t       DCPCR5;                                 /*  DCPCR5          */
    uint16_t       DCPCR6;                                 /*  DCPCR6          */
    uint16_t       DCPCR7;                                 /*  DCPCR7          */
    uint16_t       DCPCR8;                                 /*  DCPCR8          */
    uint16_t       NSDCR;                                  /*  NSDCR           */
    uint16_t       BTLCR;                                  /*  BTLCR           */
    uint16_t       BTGPCR;                                 /*  BTGPCR          */
#define DVDEC_ACCCRn_COUNT 3
    uint16_t       ACCCR1;                                 /*  ACCCR1          */
    uint16_t       ACCCR2;                                 /*  ACCCR2          */
    uint16_t       ACCCR3;                                 /*  ACCCR3          */
    uint16_t       TINTCR;                                 /*  TINTCR          */
    uint16_t       YCDCR;                                  /*  YCDCR           */
#define DVDEC_AGCCRn_COUNT 2
    uint16_t       AGCCR1;                                 /*  AGCCR1          */
    uint16_t       AGCCR2;                                 /*  AGCCR2          */
    uint16_t       PKLIMITCR;                              /*  PKLIMITCR       */
#define DVDEC_RGORCRn_COUNT 7
    uint16_t       RGORCR1;                                /*  RGORCR1         */
    uint16_t       RGORCR2;                                /*  RGORCR2         */
    uint16_t       RGORCR3;                                /*  RGORCR3         */
    uint16_t       RGORCR4;                                /*  RGORCR4         */
    uint16_t       RGORCR5;                                /*  RGORCR5         */
    uint16_t       RGORCR6;                                /*  RGORCR6         */
    uint16_t       RGORCR7;                                /*  RGORCR7         */
    uint8_t        dummy4[24];                             /*                  */
    uint16_t       AFCPFCR;                                /*  AFCPFCR         */
    uint16_t       RUPDCR;                                 /*  RUPDCR          */
    uint16_t       VSYNCSR;                                /*  VSYNCSR         */
    uint16_t       HSYNCSR;                                /*  HSYNCSR         */
#define DVDEC_DCPSRn_COUNT 2
    uint16_t       DCPSR1;                                 /*  DCPSR1          */
    uint16_t       DCPSR2;                                 /*  DCPSR2          */
    uint8_t        dummy5[4];                              /*                  */
    uint16_t       NSDSR;                                  /*  NSDSR           */
#define DVDEC_CROMASRn_COUNT 2
    uint16_t       CROMASR1;                               /*  CROMASR1        */
    uint16_t       CROMASR2;                               /*  CROMASR2        */
    uint16_t       SYNCSSR;                                /*  SYNCSSR         */
#define DVDEC_AGCCSRn_COUNT 2
    uint16_t       AGCCSR1;                                /*  AGCCSR1         */
    uint16_t       AGCCSR2;                                /*  AGCCSR2         */
    uint8_t        dummy6[108];                            /*                  */
#define DVDEC_YCSCRn_COUNT 7
    uint16_t       YCSCR3;                                 /*  YCSCR3          */
    uint16_t       YCSCR4;                                 /*  YCSCR4          */
    uint16_t       YCSCR5;                                 /*  YCSCR5          */
    uint16_t       YCSCR6;                                 /*  YCSCR6          */
    uint16_t       YCSCR7;                                 /*  YCSCR7          */
    uint16_t       YCSCR8;                                 /*  YCSCR8          */
    uint16_t       YCSCR9;                                 /*  YCSCR9          */
    uint8_t        dummy7[2];                              /*                  */
    uint16_t       YCSCR11;                                /*  YCSCR11         */
    uint16_t       YCSCR12;                                /*  YCSCR12         */
    uint8_t        dummy8[104];                            /*                  */
    uint16_t       DCPCR9;                                 /*  DCPCR9          */
    uint8_t        dummy9[16];                             /*                  */
#define DVDEC_YCTWA_Fn_COUNT 9
    uint16_t       YCTWA_F0;                               /*  YCTWA_F0        */
    uint16_t       YCTWA_F1;                               /*  YCTWA_F1        */
    uint16_t       YCTWA_F2;                               /*  YCTWA_F2        */
    uint16_t       YCTWA_F3;                               /*  YCTWA_F3        */
    uint16_t       YCTWA_F4;                               /*  YCTWA_F4        */
    uint16_t       YCTWA_F5;                               /*  YCTWA_F5        */
    uint16_t       YCTWA_F6;                               /*  YCTWA_F6        */
    uint16_t       YCTWA_F7;                               /*  YCTWA_F7        */
    uint16_t       YCTWA_F8;                               /*  YCTWA_F8        */
#define DVDEC_YCTWB_Fn_COUNT 9
    uint16_t       YCTWB_F0;                               /*  YCTWB_F0        */
    uint16_t       YCTWB_F1;                               /*  YCTWB_F1        */
    uint16_t       YCTWB_F2;                               /*  YCTWB_F2        */
    uint16_t       YCTWB_F3;                               /*  YCTWB_F3        */
    uint16_t       YCTWB_F4;                               /*  YCTWB_F4        */
    uint16_t       YCTWB_F5;                               /*  YCTWB_F5        */
    uint16_t       YCTWB_F6;                               /*  YCTWB_F6        */
    uint16_t       YCTWB_F7;                               /*  YCTWB_F7        */
    uint16_t       YCTWB_F8;                               /*  YCTWB_F8        */
#define DVDEC_YCTNA_Fn_COUNT 9
    uint16_t       YCTNA_F0;                               /*  YCTNA_F0        */
    uint16_t       YCTNA_F1;                               /*  YCTNA_F1        */
    uint16_t       YCTNA_F2;                               /*  YCTNA_F2        */
    uint16_t       YCTNA_F3;                               /*  YCTNA_F3        */
    uint16_t       YCTNA_F4;                               /*  YCTNA_F4        */
    uint16_t       YCTNA_F5;                               /*  YCTNA_F5        */
    uint16_t       YCTNA_F6;                               /*  YCTNA_F6        */
    uint16_t       YCTNA_F7;                               /*  YCTNA_F7        */
    uint16_t       YCTNA_F8;                               /*  YCTNA_F8        */
#define DVDEC_YCTNB_Fn_COUNT 9
    uint16_t       YCTNB_F0;                               /*  YCTNB_F0        */
    uint16_t       YCTNB_F1;                               /*  YCTNB_F1        */
    uint16_t       YCTNB_F2;                               /*  YCTNB_F2        */
    uint16_t       YCTNB_F3;                               /*  YCTNB_F3        */
    uint16_t       YCTNB_F4;                               /*  YCTNB_F4        */
    uint16_t       YCTNB_F5;                               /*  YCTNB_F5        */
    uint16_t       YCTNB_F6;                               /*  YCTNB_F6        */
    uint16_t       YCTNB_F7;                               /*  YCTNB_F7        */
    uint16_t       YCTNB_F8;                               /*  YCTNB_F8        */
    uint8_t        dummy10[38];                            /*                  */
    uint16_t       YGAINCR;                                /*  YGAINCR         */
    uint16_t       CBGAINCR;                               /*  CBGAINCR        */
    uint16_t       CRGAINCR;                               /*  CRGAINCR        */
    uint8_t        dummy11[122];                           /*                  */
    uint16_t       PGA_UPDATE;                             /*  PGA_UPDATE      */
    uint16_t       PGACR;                                  /*  PGACR           */
    uint16_t       ADCCR2;                                 /*  ADCCR2          */
};


#define DVDEC1  (*(volatile struct st_dvdec   *)0xFCFFA008uL) /* DVDEC1 */
#define DVDEC0  (*(volatile struct st_dvdec   *)0xFCFFB808uL) /* DVDEC0 */


/* Start of channnel array defines of DVDEC */

/* Channnel array defines of DVDEC */
/*(Sample) value = DVDEC[ channel ]->ADCCR1; */
#define DVDEC_COUNT  2
#define DVDEC_ADDRESS_LIST \
    &DVDEC0, &DVDEC1

/* End of channnel array defines of DVDEC */


#define DVDEC1ADCCR1 DVDEC1.ADCCR1
#define DVDEC1TGCR1 DVDEC1.TGCR1
#define DVDEC1TGCR2 DVDEC1.TGCR2
#define DVDEC1TGCR3 DVDEC1.TGCR3
#define DVDEC1SYNSCR1 DVDEC1.SYNSCR1
#define DVDEC1SYNSCR2 DVDEC1.SYNSCR2
#define DVDEC1SYNSCR3 DVDEC1.SYNSCR3
#define DVDEC1SYNSCR4 DVDEC1.SYNSCR4
#define DVDEC1SYNSCR5 DVDEC1.SYNSCR5
#define DVDEC1HAFCCR1 DVDEC1.HAFCCR1
#define DVDEC1HAFCCR2 DVDEC1.HAFCCR2
#define DVDEC1HAFCCR3 DVDEC1.HAFCCR3
#define DVDEC1VCDWCR1 DVDEC1.VCDWCR1
#define DVDEC1DCPCR1 DVDEC1.DCPCR1
#define DVDEC1DCPCR2 DVDEC1.DCPCR2
#define DVDEC1DCPCR3 DVDEC1.DCPCR3
#define DVDEC1DCPCR4 DVDEC1.DCPCR4
#define DVDEC1DCPCR5 DVDEC1.DCPCR5
#define DVDEC1DCPCR6 DVDEC1.DCPCR6
#define DVDEC1DCPCR7 DVDEC1.DCPCR7
#define DVDEC1DCPCR8 DVDEC1.DCPCR8
#define DVDEC1NSDCR DVDEC1.NSDCR
#define DVDEC1BTLCR DVDEC1.BTLCR
#define DVDEC1BTGPCR DVDEC1.BTGPCR
#define DVDEC1ACCCR1 DVDEC1.ACCCR1
#define DVDEC1ACCCR2 DVDEC1.ACCCR2
#define DVDEC1ACCCR3 DVDEC1.ACCCR3
#define DVDEC1TINTCR DVDEC1.TINTCR
#define DVDEC1YCDCR DVDEC1.YCDCR
#define DVDEC1AGCCR1 DVDEC1.AGCCR1
#define DVDEC1AGCCR2 DVDEC1.AGCCR2
#define DVDEC1PKLIMITCR DVDEC1.PKLIMITCR
#define DVDEC1RGORCR1 DVDEC1.RGORCR1
#define DVDEC1RGORCR2 DVDEC1.RGORCR2
#define DVDEC1RGORCR3 DVDEC1.RGORCR3
#define DVDEC1RGORCR4 DVDEC1.RGORCR4
#define DVDEC1RGORCR5 DVDEC1.RGORCR5
#define DVDEC1RGORCR6 DVDEC1.RGORCR6
#define DVDEC1RGORCR7 DVDEC1.RGORCR7
#define DVDEC1AFCPFCR DVDEC1.AFCPFCR
#define DVDEC1RUPDCR DVDEC1.RUPDCR
#define DVDEC1VSYNCSR DVDEC1.VSYNCSR
#define DVDEC1HSYNCSR DVDEC1.HSYNCSR
#define DVDEC1DCPSR1 DVDEC1.DCPSR1
#define DVDEC1DCPSR2 DVDEC1.DCPSR2
#define DVDEC1NSDSR DVDEC1.NSDSR
#define DVDEC1CROMASR1 DVDEC1.CROMASR1
#define DVDEC1CROMASR2 DVDEC1.CROMASR2
#define DVDEC1SYNCSSR DVDEC1.SYNCSSR
#define DVDEC1AGCCSR1 DVDEC1.AGCCSR1
#define DVDEC1AGCCSR2 DVDEC1.AGCCSR2
#define DVDEC1YCSCR3 DVDEC1.YCSCR3
#define DVDEC1YCSCR4 DVDEC1.YCSCR4
#define DVDEC1YCSCR5 DVDEC1.YCSCR5
#define DVDEC1YCSCR6 DVDEC1.YCSCR6
#define DVDEC1YCSCR7 DVDEC1.YCSCR7
#define DVDEC1YCSCR8 DVDEC1.YCSCR8
#define DVDEC1YCSCR9 DVDEC1.YCSCR9
#define DVDEC1YCSCR11 DVDEC1.YCSCR11
#define DVDEC1YCSCR12 DVDEC1.YCSCR12
#define DVDEC1DCPCR9 DVDEC1.DCPCR9
#define DVDEC1YCTWA_F0 DVDEC1.YCTWA_F0
#define DVDEC1YCTWA_F1 DVDEC1.YCTWA_F1
#define DVDEC1YCTWA_F2 DVDEC1.YCTWA_F2
#define DVDEC1YCTWA_F3 DVDEC1.YCTWA_F3
#define DVDEC1YCTWA_F4 DVDEC1.YCTWA_F4
#define DVDEC1YCTWA_F5 DVDEC1.YCTWA_F5
#define DVDEC1YCTWA_F6 DVDEC1.YCTWA_F6
#define DVDEC1YCTWA_F7 DVDEC1.YCTWA_F7
#define DVDEC1YCTWA_F8 DVDEC1.YCTWA_F8
#define DVDEC1YCTWB_F0 DVDEC1.YCTWB_F0
#define DVDEC1YCTWB_F1 DVDEC1.YCTWB_F1
#define DVDEC1YCTWB_F2 DVDEC1.YCTWB_F2
#define DVDEC1YCTWB_F3 DVDEC1.YCTWB_F3
#define DVDEC1YCTWB_F4 DVDEC1.YCTWB_F4
#define DVDEC1YCTWB_F5 DVDEC1.YCTWB_F5
#define DVDEC1YCTWB_F6 DVDEC1.YCTWB_F6
#define DVDEC1YCTWB_F7 DVDEC1.YCTWB_F7
#define DVDEC1YCTWB_F8 DVDEC1.YCTWB_F8
#define DVDEC1YCTNA_F0 DVDEC1.YCTNA_F0
#define DVDEC1YCTNA_F1 DVDEC1.YCTNA_F1
#define DVDEC1YCTNA_F2 DVDEC1.YCTNA_F2
#define DVDEC1YCTNA_F3 DVDEC1.YCTNA_F3
#define DVDEC1YCTNA_F4 DVDEC1.YCTNA_F4
#define DVDEC1YCTNA_F5 DVDEC1.YCTNA_F5
#define DVDEC1YCTNA_F6 DVDEC1.YCTNA_F6
#define DVDEC1YCTNA_F7 DVDEC1.YCTNA_F7
#define DVDEC1YCTNA_F8 DVDEC1.YCTNA_F8
#define DVDEC1YCTNB_F0 DVDEC1.YCTNB_F0
#define DVDEC1YCTNB_F1 DVDEC1.YCTNB_F1
#define DVDEC1YCTNB_F2 DVDEC1.YCTNB_F2
#define DVDEC1YCTNB_F3 DVDEC1.YCTNB_F3
#define DVDEC1YCTNB_F4 DVDEC1.YCTNB_F4
#define DVDEC1YCTNB_F5 DVDEC1.YCTNB_F5
#define DVDEC1YCTNB_F6 DVDEC1.YCTNB_F6
#define DVDEC1YCTNB_F7 DVDEC1.YCTNB_F7
#define DVDEC1YCTNB_F8 DVDEC1.YCTNB_F8
#define DVDEC1YGAINCR DVDEC1.YGAINCR
#define DVDEC1CBGAINCR DVDEC1.CBGAINCR
#define DVDEC1CRGAINCR DVDEC1.CRGAINCR
#define DVDEC1PGA_UPDATE DVDEC1.PGA_UPDATE
#define DVDEC1PGACR DVDEC1.PGACR
#define DVDEC1ADCCR2 DVDEC1.ADCCR2
#define DVDEC0ADCCR1 DVDEC0.ADCCR1
#define DVDEC0TGCR1 DVDEC0.TGCR1
#define DVDEC0TGCR2 DVDEC0.TGCR2
#define DVDEC0TGCR3 DVDEC0.TGCR3
#define DVDEC0SYNSCR1 DVDEC0.SYNSCR1
#define DVDEC0SYNSCR2 DVDEC0.SYNSCR2
#define DVDEC0SYNSCR3 DVDEC0.SYNSCR3
#define DVDEC0SYNSCR4 DVDEC0.SYNSCR4
#define DVDEC0SYNSCR5 DVDEC0.SYNSCR5
#define DVDEC0HAFCCR1 DVDEC0.HAFCCR1
#define DVDEC0HAFCCR2 DVDEC0.HAFCCR2
#define DVDEC0HAFCCR3 DVDEC0.HAFCCR3
#define DVDEC0VCDWCR1 DVDEC0.VCDWCR1
#define DVDEC0DCPCR1 DVDEC0.DCPCR1
#define DVDEC0DCPCR2 DVDEC0.DCPCR2
#define DVDEC0DCPCR3 DVDEC0.DCPCR3
#define DVDEC0DCPCR4 DVDEC0.DCPCR4
#define DVDEC0DCPCR5 DVDEC0.DCPCR5
#define DVDEC0DCPCR6 DVDEC0.DCPCR6
#define DVDEC0DCPCR7 DVDEC0.DCPCR7
#define DVDEC0DCPCR8 DVDEC0.DCPCR8
#define DVDEC0NSDCR DVDEC0.NSDCR
#define DVDEC0BTLCR DVDEC0.BTLCR
#define DVDEC0BTGPCR DVDEC0.BTGPCR
#define DVDEC0ACCCR1 DVDEC0.ACCCR1
#define DVDEC0ACCCR2 DVDEC0.ACCCR2
#define DVDEC0ACCCR3 DVDEC0.ACCCR3
#define DVDEC0TINTCR DVDEC0.TINTCR
#define DVDEC0YCDCR DVDEC0.YCDCR
#define DVDEC0AGCCR1 DVDEC0.AGCCR1
#define DVDEC0AGCCR2 DVDEC0.AGCCR2
#define DVDEC0PKLIMITCR DVDEC0.PKLIMITCR
#define DVDEC0RGORCR1 DVDEC0.RGORCR1
#define DVDEC0RGORCR2 DVDEC0.RGORCR2
#define DVDEC0RGORCR3 DVDEC0.RGORCR3
#define DVDEC0RGORCR4 DVDEC0.RGORCR4
#define DVDEC0RGORCR5 DVDEC0.RGORCR5
#define DVDEC0RGORCR6 DVDEC0.RGORCR6
#define DVDEC0RGORCR7 DVDEC0.RGORCR7
#define DVDEC0AFCPFCR DVDEC0.AFCPFCR
#define DVDEC0RUPDCR DVDEC0.RUPDCR
#define DVDEC0VSYNCSR DVDEC0.VSYNCSR
#define DVDEC0HSYNCSR DVDEC0.HSYNCSR
#define DVDEC0DCPSR1 DVDEC0.DCPSR1
#define DVDEC0DCPSR2 DVDEC0.DCPSR2
#define DVDEC0NSDSR DVDEC0.NSDSR
#define DVDEC0CROMASR1 DVDEC0.CROMASR1
#define DVDEC0CROMASR2 DVDEC0.CROMASR2
#define DVDEC0SYNCSSR DVDEC0.SYNCSSR
#define DVDEC0AGCCSR1 DVDEC0.AGCCSR1
#define DVDEC0AGCCSR2 DVDEC0.AGCCSR2
#define DVDEC0YCSCR3 DVDEC0.YCSCR3
#define DVDEC0YCSCR4 DVDEC0.YCSCR4
#define DVDEC0YCSCR5 DVDEC0.YCSCR5
#define DVDEC0YCSCR6 DVDEC0.YCSCR6
#define DVDEC0YCSCR7 DVDEC0.YCSCR7
#define DVDEC0YCSCR8 DVDEC0.YCSCR8
#define DVDEC0YCSCR9 DVDEC0.YCSCR9
#define DVDEC0YCSCR11 DVDEC0.YCSCR11
#define DVDEC0YCSCR12 DVDEC0.YCSCR12
#define DVDEC0DCPCR9 DVDEC0.DCPCR9
#define DVDEC0YCTWA_F0 DVDEC0.YCTWA_F0
#define DVDEC0YCTWA_F1 DVDEC0.YCTWA_F1
#define DVDEC0YCTWA_F2 DVDEC0.YCTWA_F2
#define DVDEC0YCTWA_F3 DVDEC0.YCTWA_F3
#define DVDEC0YCTWA_F4 DVDEC0.YCTWA_F4
#define DVDEC0YCTWA_F5 DVDEC0.YCTWA_F5
#define DVDEC0YCTWA_F6 DVDEC0.YCTWA_F6
#define DVDEC0YCTWA_F7 DVDEC0.YCTWA_F7
#define DVDEC0YCTWA_F8 DVDEC0.YCTWA_F8
#define DVDEC0YCTWB_F0 DVDEC0.YCTWB_F0
#define DVDEC0YCTWB_F1 DVDEC0.YCTWB_F1
#define DVDEC0YCTWB_F2 DVDEC0.YCTWB_F2
#define DVDEC0YCTWB_F3 DVDEC0.YCTWB_F3
#define DVDEC0YCTWB_F4 DVDEC0.YCTWB_F4
#define DVDEC0YCTWB_F5 DVDEC0.YCTWB_F5
#define DVDEC0YCTWB_F6 DVDEC0.YCTWB_F6
#define DVDEC0YCTWB_F7 DVDEC0.YCTWB_F7
#define DVDEC0YCTWB_F8 DVDEC0.YCTWB_F8
#define DVDEC0YCTNA_F0 DVDEC0.YCTNA_F0
#define DVDEC0YCTNA_F1 DVDEC0.YCTNA_F1
#define DVDEC0YCTNA_F2 DVDEC0.YCTNA_F2
#define DVDEC0YCTNA_F3 DVDEC0.YCTNA_F3
#define DVDEC0YCTNA_F4 DVDEC0.YCTNA_F4
#define DVDEC0YCTNA_F5 DVDEC0.YCTNA_F5
#define DVDEC0YCTNA_F6 DVDEC0.YCTNA_F6
#define DVDEC0YCTNA_F7 DVDEC0.YCTNA_F7
#define DVDEC0YCTNA_F8 DVDEC0.YCTNA_F8
#define DVDEC0YCTNB_F0 DVDEC0.YCTNB_F0
#define DVDEC0YCTNB_F1 DVDEC0.YCTNB_F1
#define DVDEC0YCTNB_F2 DVDEC0.YCTNB_F2
#define DVDEC0YCTNB_F3 DVDEC0.YCTNB_F3
#define DVDEC0YCTNB_F4 DVDEC0.YCTNB_F4
#define DVDEC0YCTNB_F5 DVDEC0.YCTNB_F5
#define DVDEC0YCTNB_F6 DVDEC0.YCTNB_F6
#define DVDEC0YCTNB_F7 DVDEC0.YCTNB_F7
#define DVDEC0YCTNB_F8 DVDEC0.YCTNB_F8
#define DVDEC0YGAINCR DVDEC0.YGAINCR
#define DVDEC0CBGAINCR DVDEC0.CBGAINCR
#define DVDEC0CRGAINCR DVDEC0.CRGAINCR
#define DVDEC0PGA_UPDATE DVDEC0.PGA_UPDATE
#define DVDEC0PGACR DVDEC0.PGACR
#define DVDEC0ADCCR2 DVDEC0.ADCCR2
#endif
