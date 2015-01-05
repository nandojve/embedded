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
* File Name : ceu_iodefine.h
* Description : Definition of I/O Register (V0.50j)
******************************************************************************/
#ifndef CEU_IODEFINE_H
#define CEU_IODEFINE_H

struct st_ceu
{                                                          /* CEU              */
/* start of struct st_ceu_n */
    uint32_t       CAPSR;                                  /*  CAPSR           */
    uint32_t       CAPCR;                                  /*  CAPCR           */
    uint32_t       CAMCR;                                  /*  CAMCR           */
    uint32_t       CMCYR;                                  /*  CMCYR           */
    uint32_t       CAMOR_A;                                /*  CAMOR_A         */
    uint32_t       CAPWR_A;                                /*  CAPWR_A         */
    uint32_t       CAIFR;                                  /*  CAIFR           */
    uint8_t        dummy322[12];                           /*                  */
    uint32_t       CRCNTR;                                 /*  CRCNTR          */
    uint32_t       CRCMPR;                                 /*  CRCMPR          */
    uint32_t       CFLCR_A;                                /*  CFLCR_A         */
    uint32_t       CFSZR_A;                                /*  CFSZR_A         */
    uint32_t       CDWDR_A;                                /*  CDWDR_A         */
    uint32_t       CDAYR_A;                                /*  CDAYR_A         */
    uint32_t       CDACR_A;                                /*  CDACR_A         */
    uint32_t       CDBYR_A;                                /*  CDBYR_A         */
    uint32_t       CDBCR_A;                                /*  CDBCR_A         */
    uint32_t       CBDSR_A;                                /*  CBDSR_A         */
    uint8_t        dummy323[12];                           /*                  */
    uint32_t       CFWCR;                                  /*  CFWCR           */
    uint32_t       CLFCR_A;                                /*  CLFCR_A         */
    uint32_t       CDOCR_A;                                /*  CDOCR_A         */
    uint8_t        dummy324[8];                            /*                  */
    uint32_t       CEIER;                                  /*  CEIER           */
    uint32_t       CETCR;                                  /*  CETCR           */
    uint8_t        dummy325[4];                            /*                  */
    uint32_t       CSTSR;                                  /*  CSTSR           */
    uint8_t        dummy326[4];                            /*                  */
    uint32_t       CDSSR;                                  /*  CDSSR           */
    uint8_t        dummy327[8];                            /*                  */
    uint32_t       CDAYR2_A;                               /*  CDAYR2_A        */
    uint32_t       CDACR2_A;                               /*  CDACR2_A        */
    uint32_t       CDBYR2_A;                               /*  CDBYR2_A        */
    uint32_t       CDBCR2_A;                               /*  CDBCR2_A        */
/* end of struct st_ceu_n */
    uint8_t        dummy3280[3936];                        /*                  */
/* start of struct st_ceu_n */
    uint8_t        dummy3281[4];                           /*                  */
    uint8_t        dummy3282[4];                           /*                  */
    uint8_t        dummy3283[4];                           /*                  */
    uint8_t        dummy3284[4];                           /*                  */
    uint32_t       CAMOR_B;                                /*  CAMOR_B         */
    uint32_t       CAPWR_B;                                /*  CAPWR_B         */
    uint8_t        dummy3290[4];                           /*                  */
    uint8_t        dummy3291[12];                          /*                  */
    uint8_t        dummy3292[4];                           /*                  */
    uint8_t        dummy3293[4];                           /*                  */
    uint32_t       CFLCR_B;                                /*  CFLCR_B         */
    uint32_t       CFSZR_B;                                /*  CFSZR_B         */
    uint32_t       CDWDR_B;                                /*  CDWDR_B         */
    uint32_t       CDAYR_B;                                /*  CDAYR_B         */
    uint32_t       CDACR_B;                                /*  CDACR_B         */
    uint32_t       CDBYR_B;                                /*  CDBYR_B         */
    uint32_t       CDBCR_B;                                /*  CDBCR_B         */
    uint32_t       CBDSR_B;                                /*  CBDSR_B         */
    uint8_t        dummy3300[12];                          /*                  */
    uint8_t        dummy3301[4];                           /*                  */
    uint32_t       CLFCR_B;                                /*  CLFCR_B         */
    uint32_t       CDOCR_B;                                /*  CDOCR_B         */
    uint8_t        dummy3310[8];                           /*                  */
    uint8_t        dummy3311[4];                           /*                  */
    uint8_t        dummy3312[4];                           /*                  */
    uint8_t        dummy3313[4];                           /*                  */
    uint8_t        dummy3314[4];                           /*                  */
    uint8_t        dummy3315[4];                           /*                  */
    uint8_t        dummy3316[4];                           /*                  */
    uint8_t        dummy3317[8];                           /*                  */
    uint32_t       CDAYR2_B;                               /*  CDAYR2_B        */
    uint32_t       CDACR2_B;                               /*  CDACR2_B        */
    uint32_t       CDBYR2_B;                               /*  CDBYR2_B        */
    uint32_t       CDBCR2_B;                               /*  CDBCR2_B        */
/* end of struct st_ceu_n */
    uint8_t        dummy3320[3936];                        /*                  */
/* start of struct st_ceu_n */
    uint8_t        dummy3321[4];                           /*                  */
    uint8_t        dummy3322[4];                           /*                  */
    uint8_t        dummy3323[4];                           /*                  */
    uint8_t        dummy3324[4];                           /*                  */
    uint32_t       CAMOR_M;                                /*  CAMOR_M         */
    uint32_t       CAPWR_M;                                /*  CAPWR_M         */
    uint8_t        dummy3330[4];                           /*                  */
    uint8_t        dummy3331[12];                          /*                  */
    uint8_t        dummy3332[4];                           /*                  */
    uint8_t        dummy3333[4];                           /*                  */
    uint32_t       CFLCR_M;                                /*  CFLCR_M         */
    uint32_t       CFSZR_M;                                /*  CFSZR_M         */
    uint32_t       CDWDR_M;                                /*  CDWDR_M         */
    uint32_t       CDAYR_M;                                /*  CDAYR_M         */
    uint32_t       CDACR_M;                                /*  CDACR_M         */
    uint32_t       CDBYR_M;                                /*  CDBYR_M         */
    uint32_t       CDBCR_M;                                /*  CDBCR_M         */
    uint32_t       CBDSR_M;                                /*  CBDSR_M         */
    uint8_t        dummy3340[12];                          /*                  */
    uint8_t        dummy3341[4];                           /*                  */
    uint32_t       CLFCR_M;                                /*  CLFCR_M         */
    uint32_t       CDOCR_M;                                /*  CDOCR_M         */
    uint8_t        dummy3350[8];                           /*                  */
    uint8_t        dummy3351[4];                           /*                  */
    uint8_t        dummy3352[4];                           /*                  */
    uint8_t        dummy3353[4];                           /*                  */
    uint8_t        dummy3354[4];                           /*                  */
    uint8_t        dummy3355[4];                           /*                  */
    uint8_t        dummy3356[4];                           /*                  */
    uint8_t        dummy3357[8];                           /*                  */
    uint32_t       CDAYR2_M;                               /*  CDAYR2_M        */
    uint32_t       CDACR2_M;                               /*  CDACR2_M        */
    uint32_t       CDBYR2_M;                               /*  CDBYR2_M        */
    uint32_t       CDBCR2_M;                               /*  CDBCR2_M        */
/* end of struct st_ceu_n */
};


struct st_ceu_n
{
    uint32_t       not_common1;                            /*                  */
    uint32_t       not_common2;                            /*                  */
    uint32_t       not_common3;                            /*                  */
    uint32_t       not_common4;                            /*                  */
    uint32_t       CAMOR;                                  /*  CAMOR           */
    uint32_t       CAPWR;                                  /*  CAPWR           */
    uint32_t       not_common5;                            /*                  */
    uint8_t        dummy322[12];                           /*                  */
    uint32_t       not_common6;                            /*                  */
    uint32_t       not_common7;                            /*                  */
    uint32_t       CFLCR;                                  /*  CFLCR           */
    uint32_t       CFSZR;                                  /*  CFSZR           */
    uint32_t       CDWDR;                                  /*  CDWDR           */
    uint32_t       CDAYR;                                  /*  CDAYR           */
    uint32_t       CDACR;                                  /*  CDACR           */
    uint32_t       CDBYR;                                  /*  CDBYR           */
    uint32_t       CDBCR;                                  /*  CDBCR           */
    uint32_t       CBDSR;                                  /*  CBDSR           */
    uint8_t        dummy323[12];                           /*                  */
    uint32_t       not_common8;                            /*                  */
    uint32_t       CLFCR;                                  /*  CLFCR           */
    uint32_t       CDOCR;                                  /*  CDOCR           */
    uint8_t        dummy324[8];                            /*                  */
    uint32_t       not_common9;                            /*                  */
    uint32_t       not_common10;                           /*                  */
    uint8_t        dummy325[4];                            /*                  */
    uint32_t       not_common11;                           /*                  */
    uint8_t        dummy326[4];                            /*                  */
    uint32_t       not_common12;                           /*                  */
    uint8_t        dummy327[8];                            /*                  */
    uint32_t       CDAYR2;                                 /*  CDAYR2          */
    uint32_t       CDACR2;                                 /*  CDACR2          */
    uint32_t       CDBYR2;                                 /*  CDBYR2          */
    uint32_t       CDBCR2;                                 /*  CDBCR2          */
};


#define CEU     (*(volatile struct st_ceu     *)0xE8210000uL) /* CEU */


/* Start of channnel array defines of CEU */

/* Channnel array defines of CEUn */
/*(Sample) value = CEUn[ channel ]->CAMOR; */
#define CEUn_COUNT  3
#define CEUn_ADDRESS_LIST \
    (volatile struct st_ceu_n*)&CEU_A, \
    (volatile struct st_ceu_n*)&CEU_B, \
    (volatile struct st_ceu_n*)&CEU_M
#define CEU_A   (*(volatile struct st_ceu_n *)&CEU.CAPSR)                 /* CEU_A */
#define CEU_B   (*(volatile struct st_ceu_n *)&CEU.dummy3281)             /* CEU_B */
#define CEU_M   (*(volatile struct st_ceu_n *)&CEU.dummy3321)             /* CEU_M */

/* End of channnel array defines of CEU */


#define CEUCAPSR CEU.CAPSR
#define CEUCAPCR CEU.CAPCR
#define CEUCAMCR CEU.CAMCR
#define CEUCMCYR CEU.CMCYR
#define CEUCAMOR_A CEU.CAMOR_A
#define CEUCAPWR_A CEU.CAPWR_A
#define CEUCAIFR CEU.CAIFR
#define CEUCRCNTR CEU.CRCNTR
#define CEUCRCMPR CEU.CRCMPR
#define CEUCFLCR_A CEU.CFLCR_A
#define CEUCFSZR_A CEU.CFSZR_A
#define CEUCDWDR_A CEU.CDWDR_A
#define CEUCDAYR_A CEU.CDAYR_A
#define CEUCDACR_A CEU.CDACR_A
#define CEUCDBYR_A CEU.CDBYR_A
#define CEUCDBCR_A CEU.CDBCR_A
#define CEUCBDSR_A CEU.CBDSR_A
#define CEUCFWCR CEU.CFWCR
#define CEUCLFCR_A CEU.CLFCR_A
#define CEUCDOCR_A CEU.CDOCR_A
#define CEUCEIER CEU.CEIER
#define CEUCETCR CEU.CETCR
#define CEUCSTSR CEU.CSTSR
#define CEUCDSSR CEU.CDSSR
#define CEUCDAYR2_A CEU.CDAYR2_A
#define CEUCDACR2_A CEU.CDACR2_A
#define CEUCDBYR2_A CEU.CDBYR2_A
#define CEUCDBCR2_A CEU.CDBCR2_A
#define CEUCAMOR_B CEU.CAMOR_B
#define CEUCAPWR_B CEU.CAPWR_B
#define CEUCFLCR_B CEU.CFLCR_B
#define CEUCFSZR_B CEU.CFSZR_B
#define CEUCDWDR_B CEU.CDWDR_B
#define CEUCDAYR_B CEU.CDAYR_B
#define CEUCDACR_B CEU.CDACR_B
#define CEUCDBYR_B CEU.CDBYR_B
#define CEUCDBCR_B CEU.CDBCR_B
#define CEUCBDSR_B CEU.CBDSR_B
#define CEUCLFCR_B CEU.CLFCR_B
#define CEUCDOCR_B CEU.CDOCR_B
#define CEUCDAYR2_B CEU.CDAYR2_B
#define CEUCDACR2_B CEU.CDACR2_B
#define CEUCDBYR2_B CEU.CDBYR2_B
#define CEUCDBCR2_B CEU.CDBCR2_B
#define CEUCAMOR_M CEU.CAMOR_M
#define CEUCAPWR_M CEU.CAPWR_M
#define CEUCFLCR_M CEU.CFLCR_M
#define CEUCFSZR_M CEU.CFSZR_M
#define CEUCDWDR_M CEU.CDWDR_M
#define CEUCDAYR_M CEU.CDAYR_M
#define CEUCDACR_M CEU.CDACR_M
#define CEUCDBYR_M CEU.CDBYR_M
#define CEUCDBCR_M CEU.CDBCR_M
#define CEUCBDSR_M CEU.CBDSR_M
#define CEUCLFCR_M CEU.CLFCR_M
#define CEUCDOCR_M CEU.CDOCR_M
#define CEUCDAYR2_M CEU.CDAYR2_M
#define CEUCDACR2_M CEU.CDACR2_M
#define CEUCDBYR2_M CEU.CDBYR2_M
#define CEUCDBCR2_M CEU.CDBCR2_M
#endif
