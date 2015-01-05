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
* Copyright (C) 2012 Renesas Electronics Corporation. All rights reserved.
*******************************************************************************/
/*******************************************************************************
* File Name    : resetprg.c
* $Rev: 19312 $
* $Date:: 2013-09-10 15:54:27 +0100#$
* Device(s)    : Aragon
* Tool-Chain   : DS-5 Ver 5.13
*              : ARM Complier
* OS           :
* H/W Platform : Aragon CPU Board
* Description  : Aragon Sample Program - Sub Main
* Operation    :
* Limitations  :
*******************************************************************************/
#include "r_typedefs.h"
#include "devdrv_common.h"      /* Common Driver Header */
#include "devdrv_intc.h"        /* INTC Driver Header   */
#include "resetprg.h"
#include "stb_init.h"
#include "port_init.h"
#include "bsc_userdef.h"


#if defined(__thumb2__) || (defined(__thumb__) && defined(__ARM_ARCH_6M__))
# define THUMB_V7_V6M
#endif

/* Defined if this target supports the BLX Rm instruction.  */
#if  !defined(__ARM_ARCH_2__) && !defined(__ARM_ARCH_3__) && !defined(__ARM_ARCH_3M__)    && !defined(__ARM_ARCH_4__)    && !defined(__ARM_ARCH_4T__)
# define HAVE_CALL_INDIRECT
#endif

#ifdef HAVE_INITFINI_ARRAY
#define _init    __libc_init_array
#define _fini    __libc_fini_array
#endif

extern int __enable_irq(void);
extern int __enable_fiq(void);
extern int L1CacheInit(void);
extern int VbarInit(void);

char Banner[] = {"\nRZ/A1 R7S72100 CPU Board Sample Program (GNU build RAM Version).\n"};

/*******************************************************************************
* Function Name: $Sub$$main
* Description  :
* Arguments    : none
* Return Value : none
*******************************************************************************/
void PowerON_Reset (void)
{
    STB_Init();

    PORT_Init();

    /* ==== BSC setting ==== */
    /* Note BSC_AREA_CS0 & BSC_AREA_CS1 initialised by Peripheral_BasicInit() */
    R_BSC_Init((uint8_t)(BSC_AREA_CS2 | BSC_AREA_CS3));

    /* ==== INTC setting ==== */
    R_INTC_Init();

    /* ==== Initial setting of the level 1 cache ==== */
    L1CacheInit();

    /* ==== Vector base address setting ==== */
    VbarInit();

    __enable_irq();
    __enable_fiq();

    main();

    while(1); /* Stops program from running off */
}

