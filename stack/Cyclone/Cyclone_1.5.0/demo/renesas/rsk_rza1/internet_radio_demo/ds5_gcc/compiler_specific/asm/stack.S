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
* File Name     : stackInit.S
* Version       : 0.01
* Device(s)     : Aragon - RZ/A1H
* Tool-Chain    : DS-5 Ver 5.13
*                 ARM Complier
*               :
* H/W Platform  : Aragon CPU Board
* Description   : This is the code to be executed on the target
*******************************************************************************/
/*******************************************************************************
* History : DD.MM.YYYY Version Description
*         : 18.06.2013 0.01
*******************************************************************************/

    .text
    .code 32

    .global stackInit
    .func   stackInit
stackInit:
#ifdef ARM_RDP_MONITOR
    ldr    sl, .LC2
    add    sl, sl, #256
#else

#ifdef ARM_RDI_MONITOR
                                       /*  Issue Angel SWI to read stack info */
    movs    r0, #0x16
    adr    r1, .LC0                  /*  point at ptr to 4 words to receive data */

#if defined(__thumb2__)
                 /*  We are in thumb mode for startup on armv7 architectures. */
    swi    0xAB
#else
                /*  We are always in ARM mode for startup on pre armv7 archs. */
    swi    0x123456
#endif

    ldr    r0, .LC0                                     /*  point at values read */
    ldr     r1, [r0, #0]
    cmp     r1, #0
    bne     .LC32
       /* If the heap base value [r0, #0] is 0 then the heap base is actually */
       /* at the end of program data (i.e. __end__).                          */
    ldr     r1, .LC31
    str     r1, [r0, #0]

.LC32:
    ldr    r1, [r0, #8]
    ldr    r2, [r0, #12]

          /*  We skip setting sp/sl if 0 returned from semihosting.           */
          /* - According to semihosting docs, if 0 returned from semihosting, */
          /*   the system was unable to calculate the real value, so it's ok  */
          /*   to skip setting sp/sl to 0 here.                               */
          /* - Considering M-profile processors, We might want to initialize  */
          /*   sp by the first entry of vector table and return 0 to          */
          /*   SYS_HEAPINFO semihosting call, which will be skipped here.     */
    cmp    r1, #0
    beq    .LC26
    mov    sp, r1

.LC26:
    cmp    r2, #0
    beq    .LC27

                  /*  allow slop for stack overflow handling and small frames */
    add    sl, r2, #256

.LC27:
#else
         /*  Set up the stack pointer to a fixed value                        */
         /*  Changes by toralf:                                               */
         /*   - Allow linker script to provide stack via __stack symbol - see */
         /*     definition of .Lstack                                         */
         /*   - Provide "hooks" that may be used by the application to add    */
         /*     custom init code - see .Lhwinit and .Lswinit                  */
         /*   - Go through all execution modes and set up stack for each      */

    ldr    r3, .Lstack
    cmp    r3, #0

#ifdef __thumb2__
    it    eq
#endif

.LC28:
    ldreq    r3, .LC0
    /* Note: This 'mov' is essential when starting in User, and ensures we
         always get *some* sp value for the initial mode, even if we
         have somehow missed it below (in which case it gets the same
         value as FIQ - not ideal, but better than nothing.) */
    mov    sp, r3

#ifdef THUMB_V7_V6M
                         /* XXX Fill in stack assignments for interrupt modes. */
#else
    mrs    r2, CPSR
    tst    r2, #0x0F                     /* Test mode bits - in User of all are 0 */
    beq    .LC23                                  /* "eq" means r2 AND #0x0F is 0 */
    msr     CPSR_c, #0xD1                    /* FIRQ mode, interrupts disabled */
    mov     sp, r3
    sub    sl, sp, #0x1000             /* This mode also has its own sl (see below) */

    mov    r3, sl
    msr     CPSR_c, #0xD7                   /* Abort mode, interrupts disabled */
    mov    sp, r3
    sub    r3, r3, #0x1000

    msr     CPSR_c, #0xDB                 /* Undefined mode, interrupts disabled */
    mov    sp, r3
    sub    r3, r3, #0x1000

    msr     CPSR_c, #0xD2                     /* IRQ mode, interrupts disabled */
    mov    sp, r3
    sub    r3, r3, #0x2000

    msr     CPSR_c, #0xD3             /* Supervisory mode, interrupts disabled */

    mov    sp, r3
    sub    r3, r3, #0x8000                                          /* Min size 32k */
    bic    r3, r3, #0x00FF                       /* Align with current 64k block */
    bic    r3, r3, #0xFF00

    str    r3, [r3, #-4]                  /* Move value into user mode sp without */
    ldmdb    r3, {sp}^                  /* changing modes, via '^' form of ldm */
    orr    r2, r2, #0xC0                /* Back to original mode, presumably SVC, */
    msr    CPSR_c, r2                     /* with FIQ/IRQ disable bits forced to 1 */
#endif

.LC23:
    /* Setup a default stack-limit in-case the code has been  compiled with   */
    /* "-mapcs-stack-check".  Hard-wiring this value is not ideal, since there*/
    /* is currently no support for checking that the heap and stack have not  */
    /* collided, or that this default 64k is enough for the program being     */
    /* executed.                                                              */
    /* However, it ensures that this simple crt0 world will not immediately   */
    /* cause an overflow event:                                               */
    sub    sl, r3, #64 << 10                   /* Still assumes 256bytes below sl */
#endif
#endif
    ISB
    BX        lr
.LC0:
#ifdef ARM_RDI_MONITOR
    .word    HeapBase
#else
#ifndef ARM_RDP_MONITOR
       /* Changes by toralf: Provide alternative "stack" variable whose value */
       /* may be defined externally  .Lstack will be used instead of .LC0 if  */
       /* it points to a non-0 value. Also set up references to "hooks" that  */
       /* may be used by the application to provide additional init code.     */
#ifdef __pe__
        .word    0x800000
#else
        .word    0x80000                             /* Top of RAM on the board.  */
#endif
    .align 4
.LC1:
    .word    __bss_start__
.LC2:
    .word    __bss_end__
.LC31:
    .word    _end
.Lstack:
    .word    abt_stack_end
HeapBase:
    .word    0
    .endfunc
    .end
