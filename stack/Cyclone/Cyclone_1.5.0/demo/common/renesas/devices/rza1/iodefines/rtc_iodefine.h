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
* File Name : rtc_iodefine.h
* Description : Definition of I/O Register (V0.50j)
******************************************************************************/
#ifndef RTC_IODEFINE_H
#define RTC_IODEFINE_H

struct st_rtc
{                                                          /* RTC              */
    uint8_t        R64CNT;                                 /*  R64CNT          */
    uint8_t        dummy550[1];                            /*                  */
    uint8_t        RSECCNT;                                /*  RSECCNT         */
    uint8_t        dummy551[1];                            /*                  */
    uint8_t        RMINCNT;                                /*  RMINCNT         */
    uint8_t        dummy552[1];                            /*                  */
    uint8_t        RHRCNT;                                 /*  RHRCNT          */
    uint8_t        dummy553[1];                            /*                  */
    uint8_t        RWKCNT;                                 /*  RWKCNT          */
    uint8_t        dummy554[1];                            /*                  */
    uint8_t        RDAYCNT;                                /*  RDAYCNT         */
    uint8_t        dummy555[1];                            /*                  */
    uint8_t        RMONCNT;                                /*  RMONCNT         */
    uint8_t        dummy556[1];                            /*                  */
    uint16_t       RYRCNT;                                 /*  RYRCNT          */
    uint8_t        RSECAR;                                 /*  RSECAR          */
    uint8_t        dummy557[1];                            /*                  */
    uint8_t        RMINAR;                                 /*  RMINAR          */
    uint8_t        dummy558[1];                            /*                  */
    uint8_t        RHRAR;                                  /*  RHRAR           */
    uint8_t        dummy559[1];                            /*                  */
    uint8_t        RWKAR;                                  /*  RWKAR           */
    uint8_t        dummy560[1];                            /*                  */
    uint8_t        RDAYAR;                                 /*  RDAYAR          */
    uint8_t        dummy561[1];                            /*                  */
    uint8_t        RMONAR;                                 /*  RMONAR          */
    uint8_t        dummy562[1];                            /*                  */
    uint8_t        RCR1;                                   /*  RCR1            */
    uint8_t        dummy563[1];                            /*                  */
    uint8_t        RCR2;                                   /*  RCR2            */
    uint8_t        dummy564[1];                            /*                  */
    uint16_t       RYRAR;                                  /*  RYRAR           */
    uint8_t        dummy565[2];                            /*                  */
    uint8_t        RCR3;                                   /*  RCR3            */
    uint8_t        dummy566[1];                            /*                  */
    uint8_t        RCR5;                                   /*  RCR5            */
    uint8_t        dummy567[3];                            /*                  */
    uint16_t       RFRH;                                   /*  RFRH            */
    uint16_t       RFRL;                                   /*  RFRL            */
};


#define RTC     (*(volatile struct st_rtc     *)0xFCFF1000uL) /* RTC */


#define RTCR64CNT RTC.R64CNT
#define RTCRSECCNT RTC.RSECCNT
#define RTCRMINCNT RTC.RMINCNT
#define RTCRHRCNT RTC.RHRCNT
#define RTCRWKCNT RTC.RWKCNT
#define RTCRDAYCNT RTC.RDAYCNT
#define RTCRMONCNT RTC.RMONCNT
#define RTCRYRCNT RTC.RYRCNT
#define RTCRSECAR RTC.RSECAR
#define RTCRMINAR RTC.RMINAR
#define RTCRHRAR RTC.RHRAR
#define RTCRWKAR RTC.RWKAR
#define RTCRDAYAR RTC.RDAYAR
#define RTCRMONAR RTC.RMONAR
#define RTCRCR1 RTC.RCR1
#define RTCRCR2 RTC.RCR2
#define RTCRYRAR RTC.RYRAR
#define RTCRCR3 RTC.RCR3
#define RTCRCR5 RTC.RCR5
#define RTCRFRH RTC.RFRH
#define RTCRFRL RTC.RFRL
#endif
