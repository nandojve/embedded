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
* File Name : lin_iodefine.h
* Description : Definition of I/O Register (V0.50j)
******************************************************************************/
#ifndef LIN_IODEFINE_H
#define LIN_IODEFINE_H

struct st_lin
{                                                          /* LIN              */
    uint8_t        dummy1[1];                              /*                  */
    uint8_t        LWBR;                                   /*  LWBR            */
    uint8_t        LBRP0;                                  /*  LBRP0           */
    uint8_t        LBRP1;                                  /*  LBRP1           */
    uint8_t        LSTC;                                   /*  LSTC            */
    uint8_t        dummy2[3];                              /*                  */
    uint8_t        LMD;                                    /*  LMD             */
    uint8_t        LBFC;                                   /*  LBFC            */
    uint8_t        LSC;                                    /*  LSC             */
    uint8_t        LWUP;                                   /*  LWUP            */
    uint8_t        LIE;                                    /*  LIE             */
    uint8_t        LEDE;                                   /*  LEDE            */
    uint8_t        LCUC;                                   /*  LCUC            */
    uint8_t        dummy3[1];                              /*                  */
    uint8_t        LTRC;                                   /*  LTRC            */
    uint8_t        LMST;                                   /*  LMST            */
    uint8_t        LST;                                    /*  LST             */
    uint8_t        LEST;                                   /*  LEST            */
    uint8_t        LDFC;                                   /*  LDFC            */
    uint8_t        LIDB;                                   /*  LIDB            */
    uint8_t        LCBR;                                   /*  LCBR            */
    uint8_t        LUDB0;                                  /*  LUDB0           */
#define LIN_LDBn_COUNT 8
    uint8_t        LDB1;                                   /*  LDB1            */
    uint8_t        LDB2;                                   /*  LDB2            */
    uint8_t        LDB3;                                   /*  LDB3            */
    uint8_t        LDB4;                                   /*  LDB4            */
    uint8_t        LDB5;                                   /*  LDB5            */
    uint8_t        LDB6;                                   /*  LDB6            */
    uint8_t        LDB7;                                   /*  LDB7            */
    uint8_t        LDB8;                                   /*  LDB8            */
    uint8_t        LUOER;                                  /*  LUOER           */
    uint8_t        LUOR1;                                  /*  LUOR1           */
    uint8_t        dummy4[2];                              /*                  */
    
    union                                                  /*                  */
    {                                                      /* LUTDR            */
        uint16_t       UINT16;                             /*  16-bit Access   */
        uint8_t        UINT8[2];                           /*  8-bit Access    */
    } LUTDR;                                               /*                  */
    
    union                                                  /*                  */
    {                                                      /* LURDR            */
        uint16_t       UINT16;                             /*  16-bit Access   */
        uint8_t        UINT8[2];                           /*  8-bit Access    */
    } LURDR;                                               /*                  */
    
    union                                                  /*                  */
    {                                                      /* LUWTDR           */
        uint16_t       UINT16;                             /*  16-bit Access   */
        uint8_t        UINT8[2];                           /*  8-bit Access    */
    } LUWTDR;                                              /*                  */
    
};


#define LIN0    (*(volatile struct st_lin     *)0xFCFE9000uL) /* LIN0 */
#define LIN1    (*(volatile struct st_lin     *)0xFCFE9800uL) /* LIN1 */


/* Start of channnel array defines of LIN */

/* Channnel array defines of LIN */
/*(Sample) value = LIN[ channel ]->LWBR; */
#define LIN_COUNT  2
#define LIN_ADDRESS_LIST \
    &LIN0, &LIN1

/* End of channnel array defines of LIN */


#define LIN0LWBR LIN0.LWBR
#define LIN0LBRP0 LIN0.LBRP0
#define LIN0LBRP1 LIN0.LBRP1
#define LIN0LSTC LIN0.LSTC
#define LIN0LMD LIN0.LMD
#define LIN0LBFC LIN0.LBFC
#define LIN0LSC LIN0.LSC
#define LIN0LWUP LIN0.LWUP
#define LIN0LIE LIN0.LIE
#define LIN0LEDE LIN0.LEDE
#define LIN0LCUC LIN0.LCUC
#define LIN0LTRC LIN0.LTRC
#define LIN0LMST LIN0.LMST
#define LIN0LST LIN0.LST
#define LIN0LEST LIN0.LEST
#define LIN0LDFC LIN0.LDFC
#define LIN0LIDB LIN0.LIDB
#define LIN0LCBR LIN0.LCBR
#define LIN0LUDB0 LIN0.LUDB0
#define LIN0LDB1 LIN0.LDB1
#define LIN0LDB2 LIN0.LDB2
#define LIN0LDB3 LIN0.LDB3
#define LIN0LDB4 LIN0.LDB4
#define LIN0LDB5 LIN0.LDB5
#define LIN0LDB6 LIN0.LDB6
#define LIN0LDB7 LIN0.LDB7
#define LIN0LDB8 LIN0.LDB8
#define LIN0LUOER LIN0.LUOER
#define LIN0LUOR1 LIN0.LUOR1
#define LIN0LUTDR LIN0.LUTDR.UINT16
#define LIN0LUTDR_BYTE_L LIN0.LUTDR.UINT8[L]
#define LIN0LUTDR_BYTE_H LIN0.LUTDR.UINT8[H]
#define LIN0LURDR LIN0.LURDR.UINT16
#define LIN0LURDR_BYTE_L LIN0.LURDR.UINT8[L]
#define LIN0LURDR_BYTE_H LIN0.LURDR.UINT8[H]
#define LIN0LUWTDR LIN0.LUWTDR.UINT16
#define LIN0LUWTDR_BYTE_L LIN0.LUWTDR.UINT8[L]
#define LIN0LUWTDR_BYTE_H LIN0.LUWTDR.UINT8[H]
#define LIN1LWBR LIN1.LWBR
#define LIN1LBRP0 LIN1.LBRP0
#define LIN1LBRP1 LIN1.LBRP1
#define LIN1LSTC LIN1.LSTC
#define LIN1LMD LIN1.LMD
#define LIN1LBFC LIN1.LBFC
#define LIN1LSC LIN1.LSC
#define LIN1LWUP LIN1.LWUP
#define LIN1LIE LIN1.LIE
#define LIN1LEDE LIN1.LEDE
#define LIN1LCUC LIN1.LCUC
#define LIN1LTRC LIN1.LTRC
#define LIN1LMST LIN1.LMST
#define LIN1LST LIN1.LST
#define LIN1LEST LIN1.LEST
#define LIN1LDFC LIN1.LDFC
#define LIN1LIDB LIN1.LIDB
#define LIN1LCBR LIN1.LCBR
#define LIN1LUDB0 LIN1.LUDB0
#define LIN1LDB1 LIN1.LDB1
#define LIN1LDB2 LIN1.LDB2
#define LIN1LDB3 LIN1.LDB3
#define LIN1LDB4 LIN1.LDB4
#define LIN1LDB5 LIN1.LDB5
#define LIN1LDB6 LIN1.LDB6
#define LIN1LDB7 LIN1.LDB7
#define LIN1LDB8 LIN1.LDB8
#define LIN1LUOER LIN1.LUOER
#define LIN1LUOR1 LIN1.LUOR1
#define LIN1LUTDR LIN1.LUTDR.UINT16
#define LIN1LUTDR_BYTE_L LIN1.LUTDR.UINT8[L]
#define LIN1LUTDR_BYTE_H LIN1.LUTDR.UINT8[H]
#define LIN1LURDR LIN1.LURDR.UINT16
#define LIN1LURDR_BYTE_L LIN1.LURDR.UINT8[L]
#define LIN1LURDR_BYTE_H LIN1.LURDR.UINT8[H]
#define LIN1LUWTDR LIN1.LUWTDR.UINT16
#define LIN1LUWTDR_BYTE_L LIN1.LUWTDR.UINT8[L]
#define LIN1LUWTDR_BYTE_H LIN1.LUWTDR.UINT8[H]
#endif
