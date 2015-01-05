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
* File Name : pwm_iodefine.h
* Description : Definition of I/O Register (V0.50j)
******************************************************************************/
#ifndef PWM_IODEFINE_H
#define PWM_IODEFINE_H

struct st_pwm
{                                                          /* PWM              */
/* start of struct st_pwm_common */
    union                                                  /*                  */
    {                                                      /* PWCR_1           */
        uint16_t       UINT16;                             /*  16-bit Access   */
        uint8_t        UINT8[2];                           /*  8-bit Access    */
    } PWCR_1;                                              /*                  */
    
    uint8_t        dummy572[2];                            /*                  */
    
    union                                                  /*                  */
    {                                                      /* PWPR_1           */
        uint16_t       UINT16;                             /*  16-bit Access   */
        uint8_t        UINT8[2];                           /*  8-bit Access    */
    } PWPR_1;                                              /*                  */
    
    uint16_t       PWCYR_1;                                /*  PWCYR_1         */
    uint16_t       PWBFR_1A;                               /*  PWBFR_1A        */
    uint16_t       PWBFR_1C;                               /*  PWBFR_1C        */
    uint16_t       PWBFR_1E;                               /*  PWBFR_1E        */
    uint16_t       PWBFR_1G;                               /*  PWBFR_1G        */
/* end of struct st_pwm_common */
    
/* start of struct st_pwm_common */
    union                                                  /*                  */
    {                                                      /* PWCR_2           */
        uint16_t       UINT16;                             /*  16-bit Access   */
        uint8_t        UINT8[2];                           /*  8-bit Access    */
    } PWCR_2;                                              /*                  */
    
    uint8_t        dummy573[2];                            /*                  */
    
    union                                                  /*                  */
    {                                                      /* PWPR_2           */
        uint16_t       UINT16;                             /*  16-bit Access   */
        uint8_t        UINT8[2];                           /*  8-bit Access    */
    } PWPR_2;                                              /*                  */
    
    uint16_t       PWCYR_2;                                /*  PWCYR_2         */
    uint16_t       PWBFR_2A;                               /*  PWBFR_2A        */
    uint16_t       PWBFR_2C;                               /*  PWBFR_2C        */
    uint16_t       PWBFR_2E;                               /*  PWBFR_2E        */
    uint16_t       PWBFR_2G;                               /*  PWBFR_2G        */
/* end of struct st_pwm_common */
};


struct st_pwm_common
{
    union                                                  /*                  */
    {                                                      /* PWCR_1           */
        uint16_t       UINT16;                             /*  16-bit Access   */
        uint8_t        UINT8[2];                           /*  8-bit Access    */
    } PWCR_1;                                              /*                  */
    
    uint8_t        dummy572[2];                            /*                  */
    
    union                                                  /*                  */
    {                                                      /* PWPR_1           */
        uint16_t       UINT16;                             /*  16-bit Access   */
        uint8_t        UINT8[2];                           /*  8-bit Access    */
    } PWPR_1;                                              /*                  */
    
    uint16_t       PWCYR_1;                                /*  PWCYR_1         */
    uint16_t       PWBFR_1A;                               /*  PWBFR_1A        */
    uint16_t       PWBFR_1C;                               /*  PWBFR_1C        */
    uint16_t       PWBFR_1E;                               /*  PWBFR_1E        */
    uint16_t       PWBFR_1G;                               /*  PWBFR_1G        */
};


#define PWM     (*(volatile struct st_pwm     *)0xFCFF50E0uL) /* PWM */


/* Start of channnel array defines of PWM */

/* Channnel array defines of PWMn */
/*(Sample) value = PWMn[ channel ]->PWCR_1.UINT16; */
#define PWMn_COUNT  2
#define PWMn_ADDRESS_LIST \
    &PWM1, &PWM2
#define PWM1    (*(volatile struct st_pwm_common *)&PWM.PWCR_1)           /* PWM1 */
#define PWM2    (*(volatile struct st_pwm_common *)&PWM.PWCR_2)           /* PWM2 */

/* End of channnel array defines of PWM */


#define PWMPWCR_1 PWM.PWCR_1.UINT16
#define PWMPWCR_1_BYTE_L PWM.PWCR_1.UINT8[L]
#define PWMPWCR_1_BYTE_H PWM.PWCR_1.UINT8[H]
#define PWMPWPR_1 PWM.PWPR_1.UINT16
#define PWMPWPR_1_BYTE_L PWM.PWPR_1.UINT8[L]
#define PWMPWPR_1_BYTE_H PWM.PWPR_1.UINT8[H]
#define PWMPWCYR_1 PWM.PWCYR_1
#define PWMPWBFR_1A PWM.PWBFR_1A
#define PWMPWBFR_1C PWM.PWBFR_1C
#define PWMPWBFR_1E PWM.PWBFR_1E
#define PWMPWBFR_1G PWM.PWBFR_1G
#define PWMPWCR_2 PWM.PWCR_2.UINT16
#define PWMPWCR_2_BYTE_L PWM.PWCR_2.UINT8[L]
#define PWMPWCR_2_BYTE_H PWM.PWCR_2.UINT8[H]
#define PWMPWPR_2 PWM.PWPR_2.UINT16
#define PWMPWPR_2_BYTE_L PWM.PWPR_2.UINT8[L]
#define PWMPWPR_2_BYTE_H PWM.PWPR_2.UINT8[H]
#define PWMPWCYR_2 PWM.PWCYR_2
#define PWMPWBFR_2A PWM.PWBFR_2A
#define PWMPWBFR_2C PWM.PWBFR_2C
#define PWMPWBFR_2E PWM.PWBFR_2E
#define PWMPWBFR_2G PWM.PWBFR_2G
#endif
