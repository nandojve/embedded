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
* File Name : romdec_iodefine.h
* Description : Definition of I/O Register (V0.50j)
******************************************************************************/
#ifndef ROMDEC_IODEFINE_H
#define ROMDEC_IODEFINE_H

struct st_romdec
{                                                          /* ROMDEC           */
    uint8_t        CROMEN;                                 /*  CROMEN          */
    uint8_t        CROMSY0;                                /*  CROMSY0         */
#define ROMDEC_CROMCTL0_COUNT 2
    uint8_t        CROMCTL0;                               /*  CROMCTL0        */
    uint8_t        CROMCTL1;                               /*  CROMCTL1        */
    uint8_t        dummy23[1];                             /*                  */
    uint8_t        CROMCTL3;                               /*  CROMCTL3        */
    uint8_t        CROMCTL4;                               /*  CROMCTL4        */
    uint8_t        CROMCTL5;                               /*  CROMCTL5        */
#define ROMDEC_CROMST0_COUNT 2
    uint8_t        CROMST0;                                /*  CROMST0         */
    uint8_t        CROMST1;                                /*  CROMST1         */
    uint8_t        dummy24[1];                             /*                  */
    uint8_t        CROMST3;                                /*  CROMST3         */
    uint8_t        CROMST4;                                /*  CROMST4         */
    uint8_t        CROMST5;                                /*  CROMST5         */
    uint8_t        CROMST6;                                /*  CROMST6         */
    uint8_t        dummy25[5];                             /*                  */
#define ROMDEC_CBUFST0_COUNT 3
    uint8_t        CBUFST0;                                /*  CBUFST0         */
    uint8_t        CBUFST1;                                /*  CBUFST1         */
    uint8_t        CBUFST2;                                /*  CBUFST2         */
    uint8_t        dummy26[1];                             /*                  */
#define ROMDEC_HEAD00_COUNT 4
    uint8_t        HEAD00;                                 /*  HEAD00          */
    uint8_t        HEAD01;                                 /*  HEAD01          */
    uint8_t        HEAD02;                                 /*  HEAD02          */
    uint8_t        HEAD03;                                 /*  HEAD03          */
#define ROMDEC_SHEAD00_COUNT 8
    uint8_t        SHEAD00;                                /*  SHEAD00         */
    uint8_t        SHEAD01;                                /*  SHEAD01         */
    uint8_t        SHEAD02;                                /*  SHEAD02         */
    uint8_t        SHEAD03;                                /*  SHEAD03         */
    uint8_t        SHEAD04;                                /*  SHEAD04         */
    uint8_t        SHEAD05;                                /*  SHEAD05         */
    uint8_t        SHEAD06;                                /*  SHEAD06         */
    uint8_t        SHEAD07;                                /*  SHEAD07         */
#define ROMDEC_HEAD20_COUNT 4
    uint8_t        HEAD20;                                 /*  HEAD20          */
    uint8_t        HEAD21;                                 /*  HEAD21          */
    uint8_t        HEAD22;                                 /*  HEAD22          */
    uint8_t        HEAD23;                                 /*  HEAD23          */
#define ROMDEC_SHEAD20_COUNT 8
    uint8_t        SHEAD20;                                /*  SHEAD20         */
    uint8_t        SHEAD21;                                /*  SHEAD21         */
    uint8_t        SHEAD22;                                /*  SHEAD22         */
    uint8_t        SHEAD23;                                /*  SHEAD23         */
    uint8_t        SHEAD24;                                /*  SHEAD24         */
    uint8_t        SHEAD25;                                /*  SHEAD25         */
    uint8_t        SHEAD26;                                /*  SHEAD26         */
    uint8_t        SHEAD27;                                /*  SHEAD27         */
    uint8_t        dummy27[16];                            /*                  */
#define ROMDEC_CBUFCTL0_COUNT 4
    uint8_t        CBUFCTL0;                               /*  CBUFCTL0        */
    uint8_t        CBUFCTL1;                               /*  CBUFCTL1        */
    uint8_t        CBUFCTL2;                               /*  CBUFCTL2        */
    uint8_t        CBUFCTL3;                               /*  CBUFCTL3        */
    uint8_t        dummy28[1];                             /*                  */
    uint8_t        CROMST0M;                               /*  CROMST0M        */
    uint8_t        dummy29[186];                           /*                  */
    uint8_t        RST;                                    /*  RST             */
    uint8_t        RSTSTAT;                                /*  RSTSTAT         */
    uint8_t        SSI;                                    /*  SSI             */
    uint8_t        dummy30[5];                             /*                  */
    uint8_t        INTHOLD;                                /*  INTHOLD         */
    uint8_t        INHINT;                                 /*  INHINT          */
    uint8_t        dummy31[246];                           /*                  */
#define ROMDEC_STRMDIN0_COUNT 2
    uint16_t       STRMDIN0;                               /*  STRMDIN0        */
    uint16_t       STRMDIN2;                               /*  STRMDIN2        */
    uint16_t       STRMDOUT0;                              /*  STRMDOUT0       */
};


#define ROMDEC  (*(volatile struct st_romdec  *)0xE8005000uL) /* ROMDEC */


#define ROMDECCROMEN ROMDEC.CROMEN
#define ROMDECCROMSY0 ROMDEC.CROMSY0
#define ROMDECCROMCTL0 ROMDEC.CROMCTL0
#define ROMDECCROMCTL1 ROMDEC.CROMCTL1
#define ROMDECCROMCTL3 ROMDEC.CROMCTL3
#define ROMDECCROMCTL4 ROMDEC.CROMCTL4
#define ROMDECCROMCTL5 ROMDEC.CROMCTL5
#define ROMDECCROMST0 ROMDEC.CROMST0
#define ROMDECCROMST1 ROMDEC.CROMST1
#define ROMDECCROMST3 ROMDEC.CROMST3
#define ROMDECCROMST4 ROMDEC.CROMST4
#define ROMDECCROMST5 ROMDEC.CROMST5
#define ROMDECCROMST6 ROMDEC.CROMST6
#define ROMDECCBUFST0 ROMDEC.CBUFST0
#define ROMDECCBUFST1 ROMDEC.CBUFST1
#define ROMDECCBUFST2 ROMDEC.CBUFST2
#define ROMDECHEAD00 ROMDEC.HEAD00
#define ROMDECHEAD01 ROMDEC.HEAD01
#define ROMDECHEAD02 ROMDEC.HEAD02
#define ROMDECHEAD03 ROMDEC.HEAD03
#define ROMDECSHEAD00 ROMDEC.SHEAD00
#define ROMDECSHEAD01 ROMDEC.SHEAD01
#define ROMDECSHEAD02 ROMDEC.SHEAD02
#define ROMDECSHEAD03 ROMDEC.SHEAD03
#define ROMDECSHEAD04 ROMDEC.SHEAD04
#define ROMDECSHEAD05 ROMDEC.SHEAD05
#define ROMDECSHEAD06 ROMDEC.SHEAD06
#define ROMDECSHEAD07 ROMDEC.SHEAD07
#define ROMDECHEAD20 ROMDEC.HEAD20
#define ROMDECHEAD21 ROMDEC.HEAD21
#define ROMDECHEAD22 ROMDEC.HEAD22
#define ROMDECHEAD23 ROMDEC.HEAD23
#define ROMDECSHEAD20 ROMDEC.SHEAD20
#define ROMDECSHEAD21 ROMDEC.SHEAD21
#define ROMDECSHEAD22 ROMDEC.SHEAD22
#define ROMDECSHEAD23 ROMDEC.SHEAD23
#define ROMDECSHEAD24 ROMDEC.SHEAD24
#define ROMDECSHEAD25 ROMDEC.SHEAD25
#define ROMDECSHEAD26 ROMDEC.SHEAD26
#define ROMDECSHEAD27 ROMDEC.SHEAD27
#define ROMDECCBUFCTL0 ROMDEC.CBUFCTL0
#define ROMDECCBUFCTL1 ROMDEC.CBUFCTL1
#define ROMDECCBUFCTL2 ROMDEC.CBUFCTL2
#define ROMDECCBUFCTL3 ROMDEC.CBUFCTL3
#define ROMDECCROMST0M ROMDEC.CROMST0M
#define ROMDECRST ROMDEC.RST
#define ROMDECRSTSTAT ROMDEC.RSTSTAT
#define ROMDECSSI ROMDEC.SSI
#define ROMDECINTHOLD ROMDEC.INTHOLD
#define ROMDECINHINT ROMDEC.INHINT
#define ROMDECSTRMDIN0 ROMDEC.STRMDIN0
#define ROMDECSTRMDIN2 ROMDEC.STRMDIN2
#define ROMDECSTRMDOUT0 ROMDEC.STRMDOUT0
#endif
