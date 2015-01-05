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
* File Name : adc_iodefine.h
* Description : Definition of I/O Register (V0.50j)
******************************************************************************/
#ifndef ADC_IODEFINE_H
#define ADC_IODEFINE_H

struct st_adc
{                                                          /* ADC              */
    uint16_t       ADDRA;                                  /*  ADDRA           */
    uint16_t       ADDRB;                                  /*  ADDRB           */
    uint16_t       ADDRC;                                  /*  ADDRC           */
    uint16_t       ADDRD;                                  /*  ADDRD           */
    uint16_t       ADDRE;                                  /*  ADDRE           */
    uint16_t       ADDRF;                                  /*  ADDRF           */
    uint16_t       ADDRG;                                  /*  ADDRG           */
    uint16_t       ADDRH;                                  /*  ADDRH           */
    uint8_t        dummy32[16];                            /*                  */
    uint16_t       MPHA;                                   /*  MPHA            */
    uint16_t       MPLA;                                   /*  MPLA            */
    uint16_t       MPHB;                                   /*  MPHB            */
    uint16_t       MPLB;                                   /*  MPLB            */
    uint16_t       MPHC;                                   /*  MPHC            */
    uint16_t       MPLC;                                   /*  MPLC            */
    uint16_t       MPHD;                                   /*  MPHD            */
    uint16_t       MPLD;                                   /*  MPLD            */
    uint16_t       MPHE;                                   /*  MPHE            */
    uint16_t       MPLE;                                   /*  MPLE            */
    uint16_t       MPHF;                                   /*  MPHF            */
    uint16_t       MPLF;                                   /*  MPLF            */
    uint16_t       MPHG;                                   /*  MPHG            */
    uint16_t       MPLG;                                   /*  MPLG            */
    uint16_t       MPHH;                                   /*  MPHH            */
    uint16_t       MPLH;                                   /*  MPLH            */
    uint8_t        dummy33[32];                            /*                  */
    uint16_t       SR;                                     /*  SR              */
    uint16_t       MPER;                                   /*  MPER            */
    uint16_t       MPSR;                                   /*  MPSR            */
};


#define ADC     (*(volatile struct st_adc     *)0xE8005800uL) /* ADC */


#define ADCADDRA ADC.ADDRA
#define ADCADDRB ADC.ADDRB
#define ADCADDRC ADC.ADDRC
#define ADCADDRD ADC.ADDRD
#define ADCADDRE ADC.ADDRE
#define ADCADDRF ADC.ADDRF
#define ADCADDRG ADC.ADDRG
#define ADCADDRH ADC.ADDRH
#define ADCMPHA ADC.MPHA
#define ADCMPLA ADC.MPLA
#define ADCMPHB ADC.MPHB
#define ADCMPLB ADC.MPLB
#define ADCMPHC ADC.MPHC
#define ADCMPLC ADC.MPLC
#define ADCMPHD ADC.MPHD
#define ADCMPLD ADC.MPLD
#define ADCMPHE ADC.MPHE
#define ADCMPLE ADC.MPLE
#define ADCMPHF ADC.MPHF
#define ADCMPLF ADC.MPLF
#define ADCMPHG ADC.MPHG
#define ADCMPLG ADC.MPLG
#define ADCMPHH ADC.MPHH
#define ADCMPLH ADC.MPLH
#define ADCSR ADC.SR
#define ADCMPER ADC.MPER
#define ADCMPSR ADC.MPSR
#endif
