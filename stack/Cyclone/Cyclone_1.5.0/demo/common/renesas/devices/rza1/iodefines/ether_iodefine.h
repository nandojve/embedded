/******************************************************************************
*   DISCLAIMER
*
*   This software is supplied by Renesas Electronics Corporation and is only 
*   intended for use with Renesas products. No other uses are authorized.
*
*   This software is owned by Renesas Electronics Corporation and is protected under 
*   all applicable laws, including copyright laws.
*
*   THIS SOFTWARE IS PROVIDED "AS IS" AND RENESAS MAKES NO WARRANTIES 
*   REGARDING THIS SOFTWARE, WHETHER EXPRESS, IMPLIED OR STATUTORY, 
*   INCLUDING BUT NOT LIMITED TO WARRANTIES OF MERCHANTABILITY, FITNESS FOR A 
*   PARTICULAR PURPOSE AND NON-INFRINGEMENT.  ALL SUCH WARRANTIES ARE EXPRESSLY 
*   DISCLAIMED.
*
*   TO THE MAXIMUM EXTENT PERMITTED NOT PROHIBITED BY LAW, NEITHER RENESAS 
*   ELECTRONICS CORPORATION NOR ANY OF ITS AFFILIATED COMPANIES SHALL BE LIABLE 
*   FOR ANY DIRECT, INDIRECT, SPECIAL, INCIDENTAL OR CONSEQUENTIAL DAMAGES 
*   FOR ANY REASON RELATED TO THIS SOFTWARE, EVEN IF RENESAS OR ITS
*   AFFILIATES HAVE BEEN ADVISED OF THE POSSIBILITY OF SUCH DAMAGES.
*
*   Renesas reserves the right, without notice, to make changes to this 
*   software and to discontinue the availability of this software.
*   By using this software, you agree to the additional terms and 
*   conditions found by accessing the following link: 
*   http://www.renesas.com/disclaimer
********************************************************************************
*   Copyright (C) 2013 Renesas Electronics Corporation. All rights reserved.
**************************** Technical reference data **************************
*   System Name : 
*   File Name   : ether_iodefine.h
*   Abstract    : 
*   Version     : 1.00.00
*   Device      : ARM
*   Tool-Chain  : 
*   OS          : None
*   H/W Platform: 
*   Description : 
********************************************************************************
*   History     : Apr.24,2013 Ver.1.00.00
*******************************************************************************/
#ifndef __ETHER_IODEFINE_H__
#define __ETHER_IODEFINE_H__

#include "typedefine.h"

struct st_ether {                                      /* struct ETHER */
       union {                                         /* ARSTR        */
             unsigned long LONG;                       /*  Long Access */
             struct {                                  /*  Bit Access  */
                    unsigned long ARST:1;              /*   ARST       */
                    unsigned long :31;                 /*              */
                    } BIT;                             /*              */
             } ARSTR;                                  /*              */
};

struct st_mac {                                        /* struct MAC   */
       union {                                         /* ECMR0        */
             unsigned long LONG;                       /*  Long Access */
             struct {                                  /*  Bit Access  */
                    unsigned long PRM:1;               /*   PRM        */
                    unsigned long DM:1;                /*   DM         */
                    unsigned long :1;                  /*              */
                    unsigned long :1;                  /*   ILB        */
                    unsigned long :1;                  /*              */
                    unsigned long TE:1;                /*   TE         */
                    unsigned long RE:1;                /*   RE         */
                    unsigned long :6;                  /*              */
                    unsigned long MCT:1;               /*   MCT        */
                    unsigned long :2;                  /*              */
                    unsigned long TXF:1;               /*   TXF        */
                    unsigned long RXF:1;               /*   RXF        */
                    unsigned long PFR:1;               /*   PFR        */
                    unsigned long ZPF:1;               /*   ZPF        */
                    unsigned long RZPF:1;              /*   RZPF       */
                    unsigned long DPAD:1;              /*   DPAD       */
                    unsigned long :1;                  /*              */
                    unsigned long RCSC:1;              /*   RCSC       */
                    unsigned long :2;                  /*              */
                    unsigned long TRCCM:1;             /*   TRCCM      */
                    unsigned long :5;                  /*              */
                    } BIT;                             /*              */
             } ECMR0;                                  /*              */
       unsigned char wk0[4];                           /*              */
       unsigned long RFLR0;                            /* RFLR0        */
       unsigned char wk1[4];                           /*              */
       unsigned long ECSR0;                            /* ECSR0        */
       unsigned char wk2[4];                           /*              */
       union {                                         /* ECSIPR0      */
             unsigned long LONG;                       /*  Long Access */
             struct {                                  /*  Bit Access  */
                    unsigned long ICDIP:1;             /*   ICDIP      */
                    unsigned long :1;                  /*   MPDIP      */
                    unsigned long :1;                  /*   LCHNGIP    */
                    unsigned long :1;                  /*   PHYIP      */
                    unsigned long PFROIP:1;            /*   PFROIP     */
                    unsigned long :27;                 /*              */
                    } BIT;                             /*              */
             } ECSIPR0;                                /*              */
       unsigned char wk3[4];                           /*              */
       //unsigned long PIR0;                             /* PIR0         */
	    union {                                         /* PIR0         */
             unsigned long LONG;                       /*  Long Access */
             struct {                                  /*  Bit Access  */
                    unsigned long MDC:1;               /*   MDC        */
                    unsigned long MMD:1;               /*   MMD        */
                    unsigned long MDO:1;               /*   MDO        */
                    unsigned long MDI:1;               /*   MDI        */
                    unsigned long :28;                 /*              */
                    } BIT;                             /*              */
             } PIR0;                                   /*              */
       unsigned char wk4[4];                           /*              */
       unsigned char wk5[44];                          /*              */
       unsigned long APR0;                             /* APR0         */
       unsigned long MPR0;                             /* MPR0         */
       unsigned long PFTCR0;                           /* PFTCR0       */
       unsigned long PFRCR0;                           /* PFRCR0       */
       unsigned long TPAUSER0;                         /* TPAUSER0     */
       unsigned char wk6[72];                          /*              */
       unsigned char wk7[16];                           /*              */
       unsigned long MAHR0;                            /* MAHR0        */
       unsigned char wk8[4];                           /*              */
       unsigned long MALR0;                            /* MALR0        */
       unsigned char wk9[308];                         /*              */
       unsigned long TROCR0;                           /* TROCR0       */
       unsigned char wk10[4];                          /*              */
       unsigned long CDCR0;                            /* CDCR0        */
       unsigned char wk11[4];                          /*              */
       unsigned long LCCR0;                            /* LCCR0        */
       unsigned char wk12[44];                         /*              */
       unsigned long CEFCR0;                           /* CEFCR0       */
       unsigned char wk13[4];                          /*              */
       unsigned long FRECR0;                           /* FRECR0       */
       unsigned char wk14[4];                          /*              */
       unsigned long TSFRCR0;                          /* TSFRCR0      */
       unsigned char wk15[4];                          /*              */
       unsigned long TLFRCR0;                          /* TLFRCR0      */
       unsigned char wk16[4];                          /*              */
       unsigned long RFCR0;                            /* RFCR0        */
       unsigned char wk17[4];                          /*              */
       unsigned long CERCR0;                           /* CERCR0       */
       unsigned char wk18[4];                          /*              */
       unsigned long CEECR0;                           /* CEECR0       */
       unsigned char wk19[4];                          /*              */
       unsigned long MAFCR0;                           /* MAFCR0       */
};

struct st_tsu {                                        /* struct TSU   */
       union {                                         /* TSU_CTRST    */
             unsigned long LONG;                       /*  Long Access */
             struct {                                  /*  Bit Access  */
                    unsigned long :8;                  /*              */
                    unsigned long CTRST:1;             /*   CTRST      */
                    unsigned long :23;                 /*              */
                    } BIT;                             /*              */
             } TSU_CTRST;                              /*              */
       unsigned char wk0[48];                          /*              */
       unsigned char wk1[32];                          /*              */
       union {                                         /* TSU_VTAG0    */
             unsigned long LONG;                       /*  Long Access */
             struct {                                  /*  Bit Access  */
                    unsigned long VID0:12;             /*   VID0       */
                    unsigned long :19;                 /*              */
                    unsigned long VTAG0:1;             /*   VTAG0      */
                    } BIT;                             /*              */
             } TSU_VTAG0;                              /*              */
       unsigned char wk2[4];                           /*              */
       union {                                         /* TSU_ADSBSY   */
             unsigned long LONG;                       /*  Long Access */
             struct {                                  /*  Bit Access  */
                    unsigned long ADSBSY:1;            /*   ADSBSY     */
                    unsigned long :31;                 /*              */
                    } BIT;                             /*              */
             } TSU_ADSBSY;                             /*              */
       union {                                         /* TSU_TEN      */
             unsigned long LONG;                       /*  Long Access */
             struct {                                  /*  Bit Access  */
                    unsigned long TEN31:1;             /*   TEN31      */
                    unsigned long TEN30:1;             /*   TEN30      */
                    unsigned long TEN29:1;             /*   TEN29      */
                    unsigned long TEN28:1;             /*   TEN28      */
                    unsigned long TEN27:1;             /*   TEN27      */
                    unsigned long TEN26:1;             /*   TEN26      */
                    unsigned long TEN25:1;             /*   TEN25      */
                    unsigned long TEN24:1;             /*   TEN24      */
                    unsigned long TEN23:1;             /*   TEN23      */
                    unsigned long TEN22:1;             /*   TEN22      */
                    unsigned long TEN21:1;             /*   TEN21      */
                    unsigned long TEN20:1;             /*   TEN20      */
                    unsigned long TEN19:1;             /*   TEN19      */
                    unsigned long TEN18:1;             /*   TEN18      */
                    unsigned long TEN17:1;             /*   TEN17      */
                    unsigned long TEN16:1;             /*   TEN16      */
                    unsigned long TEN15:1;             /*   TEN15      */
                    unsigned long TEN14:1;             /*   TEN14      */
                    unsigned long TEN13:1;             /*   TEN13      */
                    unsigned long TEN12:1;             /*   TEN12      */
                    unsigned long TEN11:1;             /*   TEN11      */
                    unsigned long TEN10:1;             /*   TEN10      */
                    unsigned long TEN9:1;              /*   TEN9       */
                    unsigned long TEN8:1;              /*   TEN8       */
                    unsigned long TEN7:1;              /*   TEN7       */
                    unsigned long TEN6:1;              /*   TEN6       */
                    unsigned long TEN5:1;              /*   TEN5       */
                    unsigned long TEN4:1;              /*   TEN4       */
                    unsigned long TEN3:1;              /*   TEN3       */
                    unsigned long TEN2:1;              /*   TEN2       */
                    unsigned long TEN1:1;              /*   TEN1       */
                    unsigned long TEN0:1;              /*   TEN0       */
                    } BIT;                             /*              */
             } TSU_TEN;                                /*              */
       unsigned char wk3[24];                          /*              */
       unsigned long TXNLCR0;                          /* TXNLCR0      */
       unsigned long TXALCR0;                          /* TXALCR0      */
       unsigned long RXNLCR0;                          /* RXNLCR0      */
       unsigned long RXALCR0;                          /* RXALCR0      */
       unsigned char wk4[112];                         /*              */
       unsigned long TSU_ADRH0;                        /* TSU_ADRH0    */
       unsigned long TSU_ADRL0;                        /* TSU_ADRL0    */
       unsigned long TSU_ADRH1;                        /* TSU_ADRH1    */
       unsigned long TSU_ADRL1;                        /* TSU_ADRL1    */
       unsigned long TSU_ADRH2;                        /* TSU_ADRH2    */
       unsigned long TSU_ADRL2;                        /* TSU_ADRL2    */
       unsigned long TSU_ADRH3;                        /* TSU_ADRH3    */
       unsigned long TSU_ADRL3;                        /* TSU_ADRL3    */
       unsigned long TSU_ADRH4;                        /* TSU_ADRH4    */
       unsigned long TSU_ADRL4;                        /* TSU_ADRL4    */
       unsigned long TSU_ADRH5;                        /* TSU_ADRH5    */
       unsigned long TSU_ADRL5;                        /* TSU_ADRL5    */
       unsigned long TSU_ADRH6;                        /* TSU_ADRH6    */
       unsigned long TSU_ADRL6;                        /* TSU_ADRL6    */
       unsigned long TSU_ADRH7;                        /* TSU_ADRH7    */
       unsigned long TSU_ADRL7;                        /* TSU_ADRL7    */
       unsigned long TSU_ADRH8;                        /* TSU_ADRH8    */
       unsigned long TSU_ADRL8;                        /* TSU_ADRL8    */
       unsigned long TSU_ADRH9;                        /* TSU_ADRH9    */
       unsigned long TSU_ADRL9;                        /* TSU_ADRL9    */
       unsigned long TSU_ADRH10;                       /* TSU_ADRH10   */
       unsigned long TSU_ADRL10;                       /* TSU_ADRL10   */
       unsigned long TSU_ADRH11;                       /* TSU_ADRH11   */
       unsigned long TSU_ADRL11;                       /* TSU_ADRL11   */
       unsigned long TSU_ADRH12;                       /* TSU_ADRH12   */
       unsigned long TSU_ADRL12;                       /* TSU_ADRL12   */
       unsigned long TSU_ADRH13;                       /* TSU_ADRH13   */
       unsigned long TSU_ADRL13;                       /* TSU_ADRL13   */
       unsigned long TSU_ADRH14;                       /* TSU_ADRH14   */
       unsigned long TSU_ADRL14;                       /* TSU_ADRL14   */
       unsigned long TSU_ADRH15;                       /* TSU_ADRH15   */
       unsigned long TSU_ADRL15;                       /* TSU_ADRL15   */
       unsigned long TSU_ADRH16;                       /* TSU_ADRH16   */
       unsigned long TSU_ADRL16;                       /* TSU_ADRL16   */
       unsigned long TSU_ADRH17;                       /* TSU_ADRH17   */
       unsigned long TSU_ADRL17;                       /* TSU_ADRL17   */
       unsigned long TSU_ADRH18;                       /* TSU_ADRH18   */
       unsigned long TSU_ADRL18;                       /* TSU_ADRL18   */
       unsigned long TSU_ADRH19;                       /* TSU_ADRH19   */
       unsigned long TSU_ADRL19;                       /* TSU_ADRL19   */
       unsigned long TSU_ADRH20;                       /* TSU_ADRH20   */
       unsigned long TSU_ADRL20;                       /* TSU_ADRL20   */
       unsigned long TSU_ADRH21;                       /* TSU_ADRH21   */
       unsigned long TSU_ADRL21;                       /* TSU_ADRL21   */
       unsigned long TSU_ADRH22;                       /* TSU_ADRH22   */
       unsigned long TSU_ADRL22;                       /* TSU_ADRL22   */
       unsigned long TSU_ADRH23;                       /* TSU_ADRH23   */
       unsigned long TSU_ADRL23;                       /* TSU_ADRL23   */
       unsigned long TSU_ADRH24;                       /* TSU_ADRH24   */
       unsigned long TSU_ADRL24;                       /* TSU_ADRL24   */
       unsigned long TSU_ADRH25;                       /* TSU_ADRH25   */
       unsigned long TSU_ADRL25;                       /* TSU_ADRL25   */
       unsigned long TSU_ADRH26;                       /* TSU_ADRH26   */
       unsigned long TSU_ADRL26;                       /* TSU_ADRL26   */
       unsigned long TSU_ADRH27;                       /* TSU_ADRH27   */
       unsigned long TSU_ADRL27;                       /* TSU_ADRL27   */
       unsigned long TSU_ADRH28;                       /* TSU_ADRH28   */
       unsigned long TSU_ADRL28;                       /* TSU_ADRL28   */
       unsigned long TSU_ADRH29;                       /* TSU_ADRH29   */
       unsigned long TSU_ADRL29;                       /* TSU_ADRL29   */
       unsigned long TSU_ADRH30;                       /* TSU_ADRH30   */
       unsigned long TSU_ADRL30;                       /* TSU_ADRL30   */
       unsigned long TSU_ADRH31;                       /* TSU_ADRH31   */
       unsigned long TSU_ADRL31;                       /* TSU_ADRL31   */
};                                                     /*              */

struct st_edmac {                                      /* struct EDMAC */
       //unsigned long EDSR0;                            /* EDSR0        */
       union {                                         /* EDSR0        */
             unsigned long LONG;                       /*  Long Access */
             struct {                                  /*  Bit Access  */
                    unsigned long ENR:1;               /*   ENR        */
                    unsigned long ENT:1;               /*   ENT        */
                    unsigned long :30;                 /*              */
                    } BIT;                             /*              */
             } EDSR0;                                  /*              */
       unsigned char wk0[12];                          /*              */
       void         *TDLAR0;                           /* TDLAR0       */
       void         *TDFAR0;                           /* TDFAR0       */
       void         *TDFXR0;                           /* TDFXR0       */
       union {                                         /* TDFFR0       */
             unsigned long LONG;                       /*  Long Access */
             struct {                                  /*  Bit Access  */
                    unsigned long TDLF:1;              /*   TDLF       */
                    unsigned long :31;                 /*              */
                    } BIT;                             /*              */
             } TDFFR0;                                 /*              */
       unsigned char wk1[16];                          /*              */
       void         *RDLAR0;                           /* RDLAR0       */
       void         *RDFAR0;                           /* RDFAR0       */
       void         *RDFXR0;                           /* RDFXR0       */
       union {                                         /* RDFFR0       */
             unsigned long LONG;                       /*  Long Access */
             struct {                                  /*  Bit Access  */
                    unsigned long RDLF:1;              /*   RDLF       */
                    unsigned long :31;                 /*              */
                    } BIT;                             /*              */
             } RDFFR0;                                 /*              */
       unsigned char wk2[960];                         /*              */
       //unsigned long EDMR0;                            /* EDMR0        */
       union {                                         /* EDMR0        */
             unsigned long LONG;                       /*  Long Access */
             struct {                                  /*  Bit Access  */
                    unsigned long SWRR:1;              /*   SWRR       */
                    unsigned long SWRT:1;              /*   SWRT       */
                    unsigned long :2;                  /*              */
                    unsigned long DL:2;                /*   DL         */
                    unsigned long DE:1;                /*   DE         */
                    unsigned long :25;                 /*              */
                    } BIT;                             /*              */
             } EDMR0;                                  /*              */
       unsigned char wk3[4];                           /*              */
       union {                                         /* EDTRR0       */
             unsigned long LONG;                       /*  Long Access */
             struct {                                  /*  Bit Access  */
                    unsigned long TR:2;                /*   TR         */
                    unsigned long :30;                 /*              */
                    } BIT;                             /*              */
             } EDTRR0;                                 /*              */
       unsigned char wk4[4];                           /*              */
       union {                                         /* EDRRR0       */
             unsigned long LONG;                       /*  Long Access */
             struct {                                  /*  Bit Access  */
                    unsigned long RR:1;                /*   RR         */
                    unsigned long :31;                 /*              */
                    } BIT;                             /*              */
             } EDRRR0;                                 /*              */
       unsigned char wk5[20];                          /*              */
       //unsigned long EESR0;                            /* EESR0        */
       union {                                         /* EESR0        */
             unsigned long LONG;                       /*  Long Access */
             struct {                                  /*  Bit Access  */
                    unsigned long CERF:1;              /*   CERF       */
                    unsigned long PRE:1;               /*   PRE        */
                    unsigned long RTSF:1;              /*   RTSF       */
                    unsigned long RTLF:1;              /*   RTLF       */
                    unsigned long RRF:1;               /*   RRF        */
                    unsigned long :2;                  /*              */
                    unsigned long RMAF:1;              /*   RMAF       */
                    unsigned long :8;                  /*              */
                    unsigned long RFOF:1;              /*   RFOF       */
                    unsigned long RDE:1;               /*   RDE        */
                    unsigned long FR:1;                /*   FR         */
                    unsigned long TFUF:1;              /*   TFUF       */
                    unsigned long TDE:1;               /*   TDE        */
                    unsigned long TC0:1;               /*   TC0        */
                    unsigned long ECI:1;               /*   ECI        */
                    unsigned long :1;                  /*              */
                    unsigned long RFCOF:1;             /*   RFCOF      */
                    unsigned long RABT:1;              /*   RABT       */
                    unsigned long TABT:1;              /*   TABT       */
                    unsigned long ROC:1;               /*   ROC        */
                    unsigned long TUC:1;               /*   TUC        */
                    unsigned long TC1:1;               /*   TC1        */
                    unsigned long TWB:2;               /*   TWB        */
                    } BIT;                             /*              */
             } EESR0;                                  /*              */
       unsigned char wk6[4];                           /*              */
       union {                                         /* EESIPR0      */
             unsigned long LONG;                       /*  Long Access */
             struct {                                  /*  Bit Access  */
                    unsigned long CERFIP:1;            /*   CERFIP     */
                    unsigned long PREIP:1;             /*   PREIP      */
                    unsigned long RTSFIP:1;            /*   RTSFIP     */
                    unsigned long RTLFIP:1;            /*   RTLFIP     */
                    unsigned long RRFIP:1;             /*   RRFIP      */
                    unsigned long CELFIP:1;            /*   CELFIP     */
                    unsigned long CEEFIP:1;            /*   CEEFIP     */
                    unsigned long RMAFIP:1;            /*   RMAFIP     */
                    unsigned long TROIP:1;             /*   TROIP      */
                    unsigned long CDIP:1;              /*   CDIP       */
                    unsigned long DLCIP:1;             /*   DLCIP      */
                    unsigned long :5;                  /*              */
                    unsigned long RFOFIP:1;            /*   RFOFIP     */
                    unsigned long RDEIP:1;             /*   RDEIP      */
                    unsigned long FRIP:1;              /*   FRIP       */
                    unsigned long TFUFIP:1;            /*   TFUFIP     */
                    unsigned long TDEIP:1;             /*   TDEIP      */
                    unsigned long TC0IP:1;             /*   TC0IP      */
                    unsigned long ECIIP:1;             /*   ECIIP      */
                    unsigned long :1;                  /*              */
                    unsigned long RFCOFIP:1;           /*   RFCOFIP    */
                    unsigned long RABTIP:1;            /*   RABTIP     */
                    unsigned long TABTIP:1;            /*   TABTIP     */
                    unsigned long ROCIP:1;             /*   ROCIP      */
                    unsigned long TUCIP:1;             /*   TUCIP      */
                    unsigned long TC1IP:1;             /*   TC1IP      */
                    unsigned long TWBIP:2;             /*   TWBIP      */
                    } BIT;                             /*              */
             } EESIPR0;                                /*              */
       unsigned char wk7[4];                           /*              */
       union {                                         /* TRSCER0      */
             unsigned long LONG;                       /*  Long Access */
             struct {                                  /*  Bit Access  */
                    unsigned long CERFCE:1;            /*   CERFCE     */
                    unsigned long PRECE:1;             /*   PRECE      */
                    unsigned long RTSFCE:1;            /*   RTSFCE     */
                    unsigned long RTLFCE:1;            /*   RTLFCE     */
                    unsigned long RRFCE:1;             /*   RRFCE      */
                    unsigned long CELFCE:1;            /*   CELFCE     */
                    unsigned long CEEFCE:1;            /*   CEEFCE     */
                    unsigned long RMAFCE:1;            /*   RMAFCE     */
                    unsigned long TROCE:1;             /*   TROCE      */
                    unsigned long CDCE:1;              /*   CDCE       */
                    unsigned long DLCCE:1;             /*   DLCCE      */
                    unsigned long :5;                  /*              */
                    unsigned long RABTCE:1;            /*   RABTCE     */
                    unsigned long TABTCE:1;            /*   TABTCE     */
                    unsigned long :14;                 /*              */
                    } BIT;                             /*              */
             } TRSCER0;                                /*              */
       unsigned char wk8[4];                           /*              */
       unsigned long RMFCR0;                           /* RMFCR0       */
       unsigned char wk9[4];                           /*              */
       unsigned long TFTR0;                            /* TFTR0        */
       unsigned char wk10[4];                          /*              */
       union {                                         /* FDR0         */
             unsigned long LONG;                       /*  Long Access */
             struct {                                  /*  Bit Access  */
                    unsigned long RFD:5;               /*   RFD        */
                    unsigned long :3;                  /*              */
                    unsigned long TFD:3;               /*   TFD        */
                    unsigned long :21;                 /*              */
                    } BIT;                             /*              */
             } FDR0;                                   /*              */
       unsigned char wk11[4];                          /*              */
       union {                                         /* RMCR0        */
             unsigned long LONG;                       /*  Long Access */
             struct {                                  /*  Bit Access  */
                    unsigned long RNC:1;               /*   RNC        */
                    unsigned long :31;                 /*              */
                    } BIT;                             /*              */
             } RMCR0;                                  /*              */
       unsigned char wk12[4];                          /*              */
       union {                                         /* RPADIR0      */
             unsigned long LONG;                       /*  Long Access */
             struct {                                  /*  Bit Access  */
                    unsigned long PADR:16;             /*   PADR       */
                    unsigned long PADS:5;              /*   PADS       */
                    unsigned long :11;                 /*              */
                    } BIT;                             /*              */
             } RPADIR0;                                /*              */
       unsigned char wk13[4];                          /*              */
       union {                                         /* FCFTR0       */
             unsigned long LONG;                       /*  Long Access */
             struct {                                  /*  Bit Access  */
                    unsigned long RFD:8;               /*   RFD        */
                    unsigned long :8;                  /*              */
                    unsigned long RFF:5;               /*   RFF        */
                    unsigned long :11;                 /*              */
                    } BIT;                             /*              */
             } FCFTR0;                                 /*              */
       unsigned char wk14[120];                        /*              */
       union {                                         /* CSMR         */
             unsigned long LONG;                       /*  Long Access */
             struct {                                  /*  Bit Access  */
                    unsigned long SB:6;                /*   SB         */
                    unsigned long :24;                 /*              */
                    unsigned long CSMD:1;              /*   CSMD       */
                    unsigned long CSEBL:1;             /*   CSEBL      */
                    } BIT;                             /*              */
             } CSMR;                                   /*              */
       union {                                         /* CSSBM        */
             unsigned long LONG;                       /*  Long Access */
             struct {                                  /*  Bit Access  */
                    unsigned long SBM:6;               /*   SBM        */
                    unsigned long :26;                 /*              */
                    } BIT;                             /*              */
             } CSSBM;                                  /*              */
       unsigned long CSSMR;                            /* CSSMR        */
       unsigned char wk15[676];                        /*              */
};                                                     /*              */

#define ETHER (*(volatile struct st_ether *)0xE8204800)/* ETHER Address */
#define MAC (*(volatile struct st_mac *)0xE8203500)    /* MAC Address  */
#define TSU (*(volatile struct st_tsu *)0xE8204804)    /* TSU Address  */
#define EDMAC (*(volatile struct st_edmac *)0xE8203000)/* EDMAC Address */


#endif /* __ETHER_IODEFINE_H__ */

/* End of File */
