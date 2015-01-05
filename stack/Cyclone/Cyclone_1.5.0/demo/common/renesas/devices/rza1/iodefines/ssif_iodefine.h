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
* File Name : ssif_iodefine.h
* Description : Definition of I/O Register (V0.50j)
******************************************************************************/
#ifndef SSIF_IODEFINE_H
#define SSIF_IODEFINE_H

#if 0
struct st_ssif
{                                                          /* SSIF             */
    uint32_t       SSICR;                                  /*  SSICR           */
    uint32_t       SSISR;                                  /*  SSISR           */
    uint8_t        dummy1[8];                              /*                  */
    uint32_t       SSIFCR;                                 /*  SSIFCR          */
    uint32_t       SSIFSR;                                 /*  SSIFSR          */
    uint32_t       SSIFTDR;                                /*  SSIFTDR         */
    uint32_t       SSIFRDR;                                /*  SSIFRDR         */
    uint32_t       SSITDMR;                                /*  SSITDMR         */
    uint32_t       SSIFCCR;                                /*  SSIFCCR         */
    uint32_t       SSIFCMR;                                /*  SSIFCMR         */
    uint32_t       SSIFCSR;                                /*  SSIFCSR         */
};
#else
struct st_ssif {                                /* struct SSIF  */
       union {                                  /* SSICR        */
             uint32_t LONG;                     /*  Long Access */
             struct {                           /*  Bit Access  */
                    uint32_t REN:1;             /*   REN        */
                    uint32_t TEN:1;             /*   TEN        */
                    uint32_t :1;                /*              */
                    uint32_t MUEN:1;            /*   MUEN       */
                    uint32_t CKDV:4;            /*   CKDV       */
                    uint32_t DEL:1;             /*   DEL        */
                    uint32_t PDTA:1;            /*   PDTA       */
                    uint32_t SDTA:1;            /*   SDTA       */
                    uint32_t SPDP:1;            /*   SPDP       */
                    uint32_t SWSP:1;            /*   SWSP       */
                    uint32_t SCKP:1;            /*   SCKP       */
                    uint32_t SWSD:1;            /*   SWSD       */
                    uint32_t SCKD:1;            /*   SCKD       */
                    uint32_t SWL:3;             /*   SWL        */
                    uint32_t DWL:3;             /*   DWL        */
                    uint32_t CHNL:2;            /*   CHNL       */
                    uint32_t :1;                /*              */
                    uint32_t IIEN:1;            /*   IIEN       */
                    uint32_t ROIEN:1;           /*   ROIEN      */
                    uint32_t RUIEN:1;           /*   RUIEN      */
                    uint32_t TOIEN:1;           /*   TOIEN      */
                    uint32_t TUIEN:1;           /*   TUIEN      */
                    uint32_t CKS:1;             /*   CKS        */
                    uint32_t :1;                /*              */
                    } BIT;                      /*              */
             } SSICR;                           /*              */
       union {                                  /* SSISR        */
             uint32_t LONG;                     /*  Long Access */
             struct {                           /*  Bit Access  */
                    uint32_t IDST:1;            /*   IDST       */
                    uint32_t RSWNO:1;           /*   RSWNO      */
                    uint32_t RCHNO:2;           /*   RCHNO      */
                    uint32_t TSWNO:1;           /*   TSWNO      */
                    uint32_t TCHNO:2;           /*   TCHNO      */
                    uint32_t :18;               /*              */
                    uint32_t IIRQ:1;            /*   IIRQ       */
                    uint32_t ROIRQ:1;           /*   ROIRQ      */
                    uint32_t RUIRQ:1;           /*   RUIRQ      */
                    uint32_t TOIRQ:1;           /*   TOIRQ      */
                    uint32_t TUIRQ:1;           /*   TUIRQ      */
                    uint32_t :2;                /*              */
                    } BIT;                      /*              */
             } SSISR;                           /*              */
       uint8_t wk0[8];                          /*              */
       union {                                  /* SSIFCR       */
             uint32_t LONG;                     /*  Long Access */
             struct {                           /*  Bit Access  */
                    uint32_t RFRST:1;           /*   RFRST      */
                    uint32_t TFRST:1;           /*   TFRST      */
                    uint32_t RIE:1;             /*   RIE        */
                    uint32_t TIE:1;             /*   TIE        */
                    uint32_t RTRG:2;            /*   RTRG       */
                    uint32_t TTRG:2;            /*   TTRG       */
                    uint32_t :24;               /*              */
                    } BIT;                      /*              */
             } SSIFCR;                          /*              */
       union {                                  /* SSIFSR       */
             uint32_t LONG;                     /*  Long Access */
             struct {                           /*  Bit Access  */
                    uint32_t RDF:1;             /*   RDF        */
                    uint32_t :7;                /*              */
                    uint32_t RDC:4;             /*   RDC        */
                    uint32_t :4;                /*              */
                    uint32_t TDE:1;             /*   TDE        */
                    uint32_t :7;                /*              */
                    uint32_t TDC:4;             /*   TDC        */
                    uint32_t :4;                /*              */
                    } BIT;                      /*              */
             } SSIFSR;                          /*              */
       uint32_t SSIFTDR;                        /* SSIFTDR      */
       uint32_t SSIFRDR;                        /* SSIFRDR      */
       union {                                  /* SSITDMR      */
             uint32_t LONG;                     /*  Long Access */
             struct {                           /*  Bit Access  */
                    uint32_t TDM:1;             /*   TDM        */
                    uint32_t :7;                /*              */
                    uint32_t CONT:1;            /*   CONT       */
                    uint32_t :8;               /*              */
                    uint32_t RXDMUTE:1;         /*   RXDMUTE    */
                    uint32_t :14;               /*              */
                    } BIT;                      /*              */
             } SSITDMR;                         /*              */
};                                              /*              */
#endif

#define SSIF0   (*(volatile struct st_ssif    *)0xE820B000uL) /* SSIF0 */
#define SSIF1   (*(volatile struct st_ssif    *)0xE820B800uL) /* SSIF1 */
#define SSIF2   (*(volatile struct st_ssif    *)0xE820C000uL) /* SSIF2 */
#define SSIF3   (*(volatile struct st_ssif    *)0xE820C800uL) /* SSIF3 */
#define SSIF4   (*(volatile struct st_ssif    *)0xE820D000uL) /* SSIF4 */
#define SSIF5   (*(volatile struct st_ssif    *)0xE820D800uL) /* SSIF5 */


/* Start of channnel array defines of SSIF */

/* Channnel array defines of SSIF */
/*(Sample) value = SSIF[ channel ]->SSICR; */
#define SSIF_COUNT  6
#define SSIF_ADDRESS_LIST \
    &SSIF0, &SSIF1, &SSIF2, &SSIF3, &SSIF4, &SSIF5

/* End of channnel array defines of SSIF */


#define SSICR_0 SSIF0.SSICR
#define SSISR_0 SSIF0.SSISR
#define SSIFCR_0 SSIF0.SSIFCR
#define SSIFSR_0 SSIF0.SSIFSR
#define SSIFTDR_0 SSIF0.SSIFTDR
#define SSIFRDR_0 SSIF0.SSIFRDR
#define SSITDMR_0 SSIF0.SSITDMR
#define SSIFCCR_0 SSIF0.SSIFCCR
#define SSIFCMR_0 SSIF0.SSIFCMR
#define SSIFCSR_0 SSIF0.SSIFCSR
#define SSICR_1 SSIF1.SSICR
#define SSISR_1 SSIF1.SSISR
#define SSIFCR_1 SSIF1.SSIFCR
#define SSIFSR_1 SSIF1.SSIFSR
#define SSIFTDR_1 SSIF1.SSIFTDR
#define SSIFRDR_1 SSIF1.SSIFRDR
#define SSITDMR_1 SSIF1.SSITDMR
#define SSIFCCR_1 SSIF1.SSIFCCR
#define SSIFCMR_1 SSIF1.SSIFCMR
#define SSIFCSR_1 SSIF1.SSIFCSR
#define SSICR_2 SSIF2.SSICR
#define SSISR_2 SSIF2.SSISR
#define SSIFCR_2 SSIF2.SSIFCR
#define SSIFSR_2 SSIF2.SSIFSR
#define SSIFTDR_2 SSIF2.SSIFTDR
#define SSIFRDR_2 SSIF2.SSIFRDR
#define SSITDMR_2 SSIF2.SSITDMR
#define SSIFCCR_2 SSIF2.SSIFCCR
#define SSIFCMR_2 SSIF2.SSIFCMR
#define SSIFCSR_2 SSIF2.SSIFCSR
#define SSICR_3 SSIF3.SSICR
#define SSISR_3 SSIF3.SSISR
#define SSIFCR_3 SSIF3.SSIFCR
#define SSIFSR_3 SSIF3.SSIFSR
#define SSIFTDR_3 SSIF3.SSIFTDR
#define SSIFRDR_3 SSIF3.SSIFRDR
#define SSITDMR_3 SSIF3.SSITDMR
#define SSIFCCR_3 SSIF3.SSIFCCR
#define SSIFCMR_3 SSIF3.SSIFCMR
#define SSIFCSR_3 SSIF3.SSIFCSR
#define SSICR_4 SSIF4.SSICR
#define SSISR_4 SSIF4.SSISR
#define SSIFCR_4 SSIF4.SSIFCR
#define SSIFSR_4 SSIF4.SSIFSR
#define SSIFTDR_4 SSIF4.SSIFTDR
#define SSIFRDR_4 SSIF4.SSIFRDR
#define SSITDMR_4 SSIF4.SSITDMR
#define SSIFCCR_4 SSIF4.SSIFCCR
#define SSIFCMR_4 SSIF4.SSIFCMR
#define SSIFCSR_4 SSIF4.SSIFCSR
#define SSICR_5 SSIF5.SSICR
#define SSISR_5 SSIF5.SSISR
#define SSIFCR_5 SSIF5.SSIFCR
#define SSIFSR_5 SSIF5.SSIFSR
#define SSIFTDR_5 SSIF5.SSIFTDR
#define SSIFRDR_5 SSIF5.SSIFRDR
#define SSITDMR_5 SSIF5.SSITDMR
#define SSIFCCR_5 SSIF5.SSIFCCR
#define SSIFCMR_5 SSIF5.SSIFCMR
#define SSIFCSR_5 SSIF5.SSIFCSR
#endif
