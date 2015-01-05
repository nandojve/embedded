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
* File Name : iodefine.h
* Description : Definition of I/O Register (V0.50j)
******************************************************************************/
#ifndef R7S72100_IODEFINE_H
#define R7S72100_IODEFINE_H

#include    "r_typedefs.h"

enum iodefine_byte_select_t {
    L = 0, H = 1,
    LL= 0, LH = 1, HL = 2, HH = 3,
};

union iodefine_reg32_t
{
    uint32_t  UINT32;                                  /*  32-bit Access   */
    uint16_t  UINT16[2];                               /*  16-bit Access   */
    uint8_t   UINT8[4];                                /*  8-bit Access    */
};

union iodefine_reg32_16_t
{
    uint32_t  UINT32;                                  /*  32-bit Access   */
    uint16_t  UINT16[2];                               /*  16-bit Access   */
};





#include "iodefines/spibsc_iodefine.h"
#include "iodefines/bsc_iodefine.h"
#include "iodefines/l2c_iodefine.h"
#include "iodefines/romdec_iodefine.h"
#include "iodefines/adc_iodefine.h"
#include "iodefines/scif_iodefine.h"
#include "iodefines/scim_iodefine.h"
#include "iodefines/rspi_iodefine.h"
#include "iodefines/spdif_iodefine.h"
#include "iodefines/irda_iodefine.h"
#include "iodefines/jcu_iodefine.h"
#include "iodefines/uahv_iodefine.h"
#include "iodefines/mlb_iodefine.h"
#include "iodefines/rscan0_iodefine.h"
#include "iodefines/mmc_iodefine.h"
#include "iodefines/dmac_iodefine.h"
#include "iodefines/intc_iodefine.h"
#include "iodefines/pfc_iodefine.h"
#include "iodefines/ether_iodefine.h"
#include "iodefines/pfv_iodefine.h"
#include "iodefines/usb20_iodefine.h"
#include "iodefines/scux_iodefine.h"
#include "iodefines/ssif_iodefine.h"
#include "iodefines/ceu_iodefine.h"
#include "iodefines/wdt_iodefine.h"
#include "iodefines/cpg_iodefine.h"
#include "iodefines/gpio_iodefine.h"
#include "iodefines/lin_iodefine.h"
#include "iodefines/ostm_iodefine.h"
#include "iodefines/riic_iodefine.h"
#include "iodefines/ieb_iodefine.h"
#include "iodefines/mtu2_iodefine.h"
#include "iodefines/rtc_iodefine.h"
#include "iodefines/flctl_iodefine.h"
#include "iodefines/sdg_iodefine.h"
#include "iodefines/pwm_iodefine.h"
#include "iodefines/lvds_iodefine.h"
#include "iodefines/vdc5_iodefine.h"
#include "iodefines/disc_iodefine.h"
#include "iodefines/dvdec_iodefine.h"
#endif
