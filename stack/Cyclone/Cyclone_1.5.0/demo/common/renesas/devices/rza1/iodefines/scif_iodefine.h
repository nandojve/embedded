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
* http://www.renesas.com/disclaimer
*
*******************************************************************************/
/*******************************************************************************
* Copyright (C) 2013 Renesas Electronics Corporation. All rights reserved.
*******************************************************************************/
/*******************************************************************************
*   File Name   : scif_iodefine.h
*   Description : scif interface file
*******************************************************************************/
/*******************************************************************************
* History       : DD.MM.YYYY Version Description
*               : 18.06.2013 1.00
*******************************************************************************/
#ifndef __SCIF_IODEFINE_H__
#define __SCIF_IODEFINE_H__

#include "typedefine.h"

struct st_scif_n {                              /* struct SCIF  */
       union {                                  /* SCSMR_0      */
             _UWORD WORD;                       /*  Word Access */
             struct {                           /*  Bit Access  */
                    _UWORD CKS:2;               /*   CKS        */
                    _UWORD :1;                  /*              */
                    _UWORD STOP:1;              /*   STOP       */
                    _UWORD OE:1;                /*   O/E        */
                    _UWORD PE:1;                /*   PE         */
                    _UWORD CHR:1;               /*   CHR        */
                    _UWORD CA:1;                /*   C/A        */
                    _UWORD :8;                  /*              */
                    } BIT;                      /*              */
             } SCSMR;                           /*              */
       _UBYTE wk0[2];                           /*              */
       union {                                  /* SCBRR        */
             _UBYTE BYTE;                       /*  Byte Access */
             struct {                           /*  Bit Access  */
                    _UBYTE D:8;                 /*   D          */
                    } BIT;                      /*              */
             } SCBRR;                           /*              */
       _UBYTE wk1[3];                           /*              */
       union {                                  /* SCSCR        */
             _UWORD WORD;                       /*  Word Access */
             struct {                           /*  Bit Access  */
                    _UWORD CKE:2;               /*   CKE        */
                    _UWORD :1;                  /*              */
                    _UWORD REIE:1;              /*   REIE       */
                    _UWORD RE:1;                /*   RE         */
                    _UWORD TE:1;                /*   TE         */
                    _UWORD RIE:1;               /*   RIE        */
                    _UWORD TIE:1;               /*   TIE        */
                    _UWORD :8;                  /*              */
                    } BIT;                      /*              */
             } SCSCR;                           /*              */
       _UBYTE wk2[2];                           /*              */
       union {                                  /* SCFTDR       */
             _UBYTE BYTE;                       /*  Byte Access */
             struct {                           /*  Bit Access  */
                    _UBYTE D:8;                 /*   D          */
                    } BIT;                      /*              */
             } SCFTDR;                          /*              */
       _UBYTE wk3[3];                           /*              */
       union {                                  /* SCFSR        */
             _UWORD WORD;                       /*  Word Access */
             struct {                           /*  Bit Access  */
                    _UWORD DR:1;                /*   DR         */
                    _UWORD RDF:1;               /*   RDF        */
                    _UWORD PER:1;               /*   PER        */
                    _UWORD FER:1;               /*   FER        */
                    _UWORD BRK:1;               /*   BRK        */
                    _UWORD TDFE:1;              /*   TDFE       */
                    _UWORD TEND:1;              /*   TEND       */
                    _UWORD ER:1;                /*   ER         */
                    _UWORD FERN:4;              /*   FERN       */
                    _UWORD PERN:4;              /*   PERN       */
                    } BIT;                      /*              */
             } SCFSR;                           /*              */
       _UBYTE wk4[2];                           /*              */
       union {                                  /* SCFRDR       */
             _UBYTE BYTE;                       /*  Byte Access */
             struct {                           /*  Bit Access  */
                    _UBYTE D:8;                 /*   D          */
                    } BIT;                      /*              */
             } SCFRDR;                          /*              */
       _UBYTE wk5[3];                           /*              */
       union {                                  /* SCFCR        */
             _UWORD WORD;                       /*  Word Access */
             struct {                           /*  Bit Access  */
                    _UWORD LOOP:1;              /*   LOOP       */
                    _UWORD RFRST:1;             /*   RFRST      */
                    _UWORD TFRST:1;             /*   TFRST      */
                    _UWORD MCE:1;               /*   MCE        */
                    _UWORD TTRG:2;              /*   TTRG       */
                    _UWORD RTRG:2;              /*   RTRG       */
                    _UWORD RSTRG:3;             /*   RSTRG      */
                    _UWORD :5;                  /*              */
                    } BIT;                      /*              */
             } SCFCR;                           /*              */
       _UBYTE wk6[2];                           /*              */
       union {                                  /* SCFDR        */
             _UWORD WORD;                       /*  Word Access */
             struct {                           /*  Bit Access  */
                    _UWORD R:5;                 /*   R          */
                    _UWORD :3;                  /*              */
                    _UWORD T:5;                 /*   T          */
                    _UWORD :3;                  /*              */
                    } BIT;                      /*              */
             } SCFDR;                           /*              */
       _UBYTE wk7[2];                           /*              */
       union {                                  /* SCSPTR       */
             _UWORD WORD;                       /*  Word Access */
             struct {                           /*  Bit Access  */
                    _UWORD SPB2DT:1;            /*   SPB2DT     */
                    _UWORD SPB2IO:1;            /*   SPB2IO     */
                    _UWORD SCKDT:1;             /*   SCKDT      */
                    _UWORD SCKIO:1;             /*   SCKIO      */
                    _UWORD CTSDT:1;             /*   CTSDT      */
                    _UWORD CTSIO:1;             /*   CTSIO      */
                    _UWORD RTSDT:1;             /*   RTSDT      */
                    _UWORD RTSIO:1;             /*   RTSIO      */
                    _UWORD :8;                  /*              */
                    } BIT;                      /*              */
             } SCSPTR;                          /*              */
       _UBYTE wk8[2];                           /*              */
       union {                                  /* SCLSR        */
             _UWORD WORD;                       /*  Word Access */
             struct {                           /*  Bit Access  */
                    _UWORD ORER:1;              /*   ORER       */
                    _UWORD :15;                 /*              */
                    } BIT;                      /*              */
             } SCLSR;                           /*              */
       _UBYTE wk9[2];                           /*              */
       union {                                  /* SCEMR        */
             _UWORD WORD;                       /*  Word Access */
             struct {                           /*  Bit Access  */
                    _UWORD ABCS:1;              /*   ABCS       */
                    _UWORD :6;                  /*              */
                    _UWORD BGDM:1;              /*   BGDM       */
                    _UWORD :8;                  /*              */
                    } BIT;                      /*              */
             } SCEMR;                           /*              */
};


//for SCIF0 and SCIF1 without FIFO. In Registers perspective it appears under SCIM0/SCIM1
struct st_scifm_n {                             /* struct SCIF  */
       union {                                  /* SCSMR_0      */
    	   _UBYTE BYTE;                         /*  Byte Access */
             struct {                           /*  Bit Access  */
    		   _UBYTE CKS:2;               /*   CKS        */  //LSB
    		   _UBYTE MP:1;                /*              */
    		   _UBYTE STOP:1;              /*   STOP       */
    		   _UBYTE PM:1;                /*   O/E        */
    		   _UBYTE PE:1;                /*   PE         */
    		   _UBYTE CHR:1;               /*   CHR        */
    		   _UBYTE CM:1;                /*   C/A        */ //MSB
                    } BIT;                      /*              */
             } SCSMR;                           /*              */
       //_UBYTE wk0[1];                           /*              */
       union {                                  /* SCBRR        */
             _UBYTE BYTE;                       /*  Byte Access */
             struct {                           /*  Bit Access  */
                    _UBYTE D:8;                 /*   D          */
                    } BIT;                      /*              */
             } SCBRR;                           /*              */
       //_UBYTE wk1[1];                           /*              */
       union {                                  /* SCSCR        */
    	   _UBYTE BYTE;                         /*  Byte Access */
             struct {                           /*  Bit Access  */
    		   _UBYTE CKE:2;               /*   CKE        */
    		   _UBYTE TEIE:1;              /*   TEIE       */
    		   _UBYTE MPIE:1;              /*   MPIE       */
    		   _UBYTE RE:1;                /*   RE         */
    		   _UBYTE TE:1;                /*   TE         */
    		   _UBYTE RIE:1;               /*   RIE        */
    		   _UBYTE TIE:1;               /*   TIE        */
                    } BIT;                      /*              */
             } SCSCR;                           /*              */
       //_UBYTE wk2[1];                           /*              */
       union {                                  /* SCFTDR       */
             _UBYTE BYTE;                       /*  Byte Access */
             struct {                           /*  Bit Access  */
                    _UBYTE D:8;                 /*   D          */
                    } BIT;                      /*              */
             } SCTDR;                          /*              */
       //_UBYTE wk3[1];                           /*              */
       union {                                  /* SCFSR        */
    	   _UBYTE BYTE;                         /*  Byte Access */
             struct {                           /*  Bit Access  */
    		   _UBYTE MPBT:1;              /*   DR         */
    		   _UBYTE MPB:1;               /*   RDF        */
    		   _UBYTE TEND:1;              /*  TEND        */
    		   _UBYTE PER:1;               /*   pER        */
    		   _UBYTE FER:1;               /*   FER        */
    		   _UBYTE ORER:1;              /*   ORER       */
    		   _UBYTE :2;                  /*              */
                    } BIT;                      /*              */
             } SCSSR;                           /*              */
       //_UBYTE wk4[1];                           /*              */
       union {                                  /* SCFRDR       */
             _UBYTE BYTE;                       /*  Byte Access */
             struct {                           /*  Bit Access  */
                    _UBYTE D:8;                 /*   D          */
                    } BIT;                      /*              */
             } SCRDR;                          /*              */
       //_UBYTE wk5[1];                           /*              */
       union {                                  /* SCFDR        */
    	   _UBYTE BYTE;                         /*  Byte Access */
             struct {                           /*  Bit Access  */
    		   _UBYTE SMIF:1;              /*  SMIF        */
    		   _UBYTE :1;                  /*              */
    		   _UBYTE SINV:1;              /*  SINV        */
    		   _UBYTE SDIR:1;              /*  SDIR        */
    		   _UBYTE :3;                  /*              */
    		   _UBYTE BCP2:1;              /*  BCP2        */
                    } BIT;                      /*              */
             } SCSCMR;                          /*              */
       //_UBYTE wk7[1];                           /*              */
       union {                                  /* SCFDR        */
    	   _UBYTE BYTE;                         /*  Byte Access */
             struct {                           /*  Bit Access  */
    		   _UBYTE :4;                  /*              */
    		   _UBYTE ABCS:1;              /*   ABCS       */
    		   _UBYTE NFEN:1;              /*   NFEN       */
    		   _UBYTE :2;                  /*              */
                    } BIT;                      /*              */
             } SCSEMR;                          /*              */
       //_UBYTE wk8[1];                           /*              */
       union {                                  /* SCFCR        */
    	   _UBYTE BYTE;                         /*  Byte Access */
             struct {                           /*  Bit Access  */
    		   _UBYTE NFCS:3;              /*   NFCS       */
    		   _UBYTE :5;                  /*              */
                    } BIT;                      /*              */
             } SCSNFR;                          /*              */
       _UBYTE wk9[4];                           /*              */
       union {                                  /* SCFDR        */
    	   _UBYTE BYTE;                         /*  Byte Access */
             struct {                           /*  Bit Access  */
    		   _UBYTE :1;                  /*              */
    		   _UBYTE CTSE:1;              /*  CTSE        */
    		   _UBYTE :6;                  /*              */
                    } BIT;                      /*              */
             } SCSECR;                          /*              */
};                                              /*              */
//End of for SCIF0 and SCIF1 without FIFO

#ifndef ARM_SIM
	#define SCIF0 (*(volatile struct st_scif_n *)0xE8007000)	/* SCIF0 Address */
	#define SCIF1 (*(volatile struct st_scif_n *)0xE8007800)	/* SCIF1 Address */
	#define SCIF2 (*(volatile struct st_scif_n *)0xE8008000)	/* SCIF2 Address */
	#define SCIF3 (*(volatile struct st_scif_n *)0xE8008800)	/* SCIF3 Address */
	#define SCIF4 (*(volatile struct st_scif_n *)0xE8009000)	/* SCIF4 Address */
	#define SCIF5 (*(volatile struct st_scif_n *)0xE8009800)	/* SCIF5 Address */
	#define SCIF6 (*(volatile struct st_scif_n *)0xE800A000)	/* SCIF6 Address */
	#define SCIF7 (*(volatile struct st_scif_n *)0xE800A800)	/* SCIF7 Address */
	#define SCIM0 (*(volatile struct st_scifm_n *)0xE800B000)	/* SCIF0 Address */
	#define SCIM1 (*(volatile struct st_scifm_n *)0xE800B800)	/* SCIF1 Address */
#else	/* ARM_SIM */
	#define SCIF0 (*(volatile struct st_scif_n *)0x48A07000)	/* SCIF0 Address */
	#define SCIF1 (*(volatile struct st_scif_n *)0x48A07800)	/* SCIF1 Address */
	#define SCIF2 (*(volatile struct st_scif_n *)0x48A08000)	/* SCIF2 Address */
	#define SCIF3 (*(volatile struct st_scif_n *)0x48A08800)	/* SCIF3 Address */
	#define SCIF4 (*(volatile struct st_scif_n *)0x48A09000)	/* SCIF4 Address */
	#define SCIF5 (*(volatile struct st_scif_n *)0x48A09800)	/* SCIF5 Address */
	#define SCIF6 (*(volatile struct st_scif_n *)0x48A0A000)	/* SCIF6 Address */
	#define SCIF7 (*(volatile struct st_scif_n *)0x48A0A800)	/* SCIF7 Address */
    #define SCIM0 (*(volatile struct st_scif_n *)0x4800B000)	/* SCIF0 Address */
	#define SCIM1 (*(volatile struct st_scif_n *)0x4800B800)	/* SCIF1 Address */
#endif	/* ARM_SIM */

#endif /* __SCIF_IODEFINE_H__ */

/* End of File */
