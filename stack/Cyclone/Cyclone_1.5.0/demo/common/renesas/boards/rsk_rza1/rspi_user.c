/******************************************************************************
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
* Copyright (C) 2013 Renesas Electronics Corporation. All rights reserved.
******************************************************************************/
/******************************************************************************
* File Name     : rspi_user.c
* Device(s)     : RZ/A1H RSK+RZA1H
* H/W Platform  : RSK+RZA1H CPU Board
* Description   : This file implements user driver for RSPI module.
******************************************************************************/
/******************************************************************************
* History       : DD.MM.YYYY Version Description
*               : 18.06.2013 1.00
******************************************************************************/

/******************************************************************************
Pragma directive
******************************************************************************/

/******************************************************************************
Includes
******************************************************************************/
#include "r_typedefs.h"                   /* Default  type definition header */
#include "iodefine.h"                            /* I/O Register root header */
#include "dev_drv.h"                          /* Device Driver common header */
#include "devdrv_intc.h"                               /* INTC Driver Header */
#include "rspi.h"                           /* Device driver for RSPI header */
#include "rza_io_regrw.h"            /* Low level register read/write header */
#include "compiler_settings.h"   /* Interchangeable compiler specific header */

/******************************************************************************
Global variables and functions
******************************************************************************/
extern uint32_t * gp_rspi1_tx_address;      /* RSPI1 transmit buffer address */
extern uint16_t   g_rspi1_tx_count;         /* RSPI1 transmit data number */
extern uint32_t * gp_rspi1_rx_address;      /* RSPI1 receive buffer address */
extern uint16_t   g_rspi1_rx_count;         /* RSPI1 receive data number */
extern uint16_t   g_rspi1_rx_length;        /* RSPI1 receive data length */

void uint16_to_string (uint8_t * const output_string, uint8_t pos,
                              const uint16_t input_number);

/* Declare storage variable for received data */
volatile uint32_t g_rx_data = 0;

/* Stores the data to be transmitted */
volatile uint32_t g_tx_data;
volatile uint32_t g_trans_data;

/******************************************************************************
* Function Name: r_rspi1_transmit_interrupt
* Description  : None
* Arguments    : uint32_t int_sense : Interrupt detection
*              :                    :   INTC_LEVEL_SENSITIVE : Level sense
*              :                    :   INTC_EDGE_TRIGGER    : Edge trigger
* Return Value : None
******************************************************************************/
void r_rspi1_transmit_interrupt(uint32_t int_sense)
{
	UNUSED_PARAM(int_sense);

    if (g_rspi1_tx_count > 0U)
    {
        gp_rspi1_tx_address++;
        g_rspi1_tx_count--;
    }
}

/******************************************************************************
* Function Name: r_rspi1_receive_interrupt
* Description  : None
* Arguments    : uint32_t int_sense : Interrupt detection
*              :                    :   INTC_LEVEL_SENSITIVE : Level sense
*              :                    :   INTC_EDGE_TRIGGER    : Edge trigger
* Return Value : None
******************************************************************************/
void r_rspi1_receive_interrupt(uint32_t int_sense)
{
	UNUSED_PARAM(int_sense);

    if (g_rspi1_rx_length > g_rspi1_rx_count)
    {
        *(uint16_t *)gp_rspi1_rx_address = (uint16_t)(RSPI1.SPDR.UINT16[0] >> 16u);
        gp_rspi1_rx_address++;
        g_rspi1_rx_count++;

        if (g_rspi1_rx_length == g_rspi1_rx_count)
        {
            /* Disable receive interrupt */
        	RZA_IO_RegWrite_8(&(RSPI1.SPCR), 0, RSPIn_SPCR_SPRIE_SHIFT, RSPIn_SPCR_SPRIE);
            r_rspi1_callback_receiveend();
        }
    }
}

/******************************************************************************
* Function Name: r_rspi0_error_interrupt
* Description  : None
* Arguments    : uint32_t int_sense : Interrupt detection
*              :                    :   INTC_LEVEL_SENSITIVE : Level sense
*              :                    :   INTC_EDGE_TRIGGER    : Edge trigger
* Return Value : None
******************************************************************************/
void r_rspi1_error_interrupt(uint32_t int_sense)
{
    uint8_t err_type;

	UNUSED_PARAM(int_sense);

    /* Disable RSPI function */
	RZA_IO_RegWrite_8(&(RSPI1.SPCR), 0, RSPIn_SPCR_SPE_SHIFT, RSPIn_SPCR_SPE);

    /* Disable transmit interrupt */
	RZA_IO_RegWrite_8(&(RSPI1.SPCR), 0, RSPIn_SPCR_SPTIE_SHIFT, RSPIn_SPCR_SPTIE);

    /* Disable receive interrupt */
	RZA_IO_RegWrite_8(&(RSPI1.SPCR), 0, RSPIn_SPCR_SPRIE_SHIFT, RSPIn_SPCR_SPRIE);

    /* Disable error interrupt */
	RZA_IO_RegWrite_8(&(RSPI1.SPCR), 0, RSPIn_SPCR_SPEIE_SHIFT, RSPIn_SPCR_SPEIE);

    /* Clear error sources */
    err_type = RSPI1.SPSR;
    RSPI1.SPSR = 0xA0U;

    if (err_type != 0U)
    {
        r_rspi1_callback_error(err_type);
    }
}

/******************************************************************************
* Function Name: r_rspi1_idle_interrupt
* Description  : None
* Arguments    : uint32_t int_sense : Interrupt detection
*              :                    :   INTC_LEVEL_SENSITIVE : Level sense
*              :                    :   INTC_EDGE_TRIGGER    : Edge trigger
* Return Value : None
******************************************************************************/
void r_rspi1_idle_interrupt(uint32_t int_sense)
{
	UNUSED_PARAM(int_sense);

    /* Disable RSPI function */
	RZA_IO_RegWrite_8(&(RSPI1.SPCR), 0, RSPIn_SPCR_SPE_SHIFT, RSPIn_SPCR_SPE);

    r_rspi1_callback_transmitend();
}

/******************************************************************************
* Function Name: r_rspi0_callback_transmitend
* Description  : This function is a callback function when RSPI0 finishes 
*                transmission.
* Arguments    : None
* Return Value : None
******************************************************************************/
void r_rspi1_callback_transmitend(void)
{
}

/******************************************************************************
* Function Name: r_rspi0_callback_receiveend
* Description  : This function is a callback function when RSPI0 finishes 
*                reception.
* Arguments    : None
* Return Value : None
******************************************************************************/
void r_rspi1_callback_receiveend(void)
{
}

/******************************************************************************
* Function Name: r_rspi0_callback_error
* Description  : This function is a callback function when RSPI0 error occurs.
* Arguments    : err_type -
*                    error type value
* Return Value : None
******************************************************************************/
void r_rspi1_callback_error(uint8_t err_type)
{
}

/******************************************************************************
* Function Name : Init_SPI
* Description   : Initialises the SPI functions for this sample
* Argument      : none
* Return value  : none
******************************************************************************/
void Init_SPI (void)
{
	R_RSPI1_Create();

    /* start rSPI */
    R_RSPI1_Start();
}
/******************************************************************************
* End of function Init_SPI
******************************************************************************/

/*******************************************************************************
* Function Name: uint16_to_string
* Description  : Function converts a 16 bit integer into a character string,
*                inserts it into the array via the pointer passed at execution.
* Arguments    : (char *)output_string        - Pointer to char array that will
*                                               hold character string.
*                (uint8_t)pos                 - uint8_t number, element number
*                                               to begin inserting the character
*                                               string from (offset).
*                (const uint16_t)input_number - 16 bit integer to convert into
*                                               a string.
* Return Value : none
* Note         : No input validation is used, so output data can overflow the
*                array passed.
*******************************************************************************/
void uint16_to_string (uint8_t * const output_string, uint8_t pos,
                              const uint16_t input_number)
{
    /* Declare temporary character storage variable, and bit_shift variable */
    uint8_t  a = 0x00;

    /* Number of bits in 3 hex characters */
    uint8_t  bit_shift = 12u;

    /* Declare 16bit mask variable */
    uint16_t mask = 0xF000u;

    /* Loop through until each hex digit is converted to an ASCII character */
    while (30u > bit_shift)
    {
        /* Mask and shift the hex digit, and store in temporary variable, a */
        a = (uint8_t)((input_number & mask) >> bit_shift);

        /* Convert the hex digit into an ASCII character, and store in output
           string */
        output_string[pos] = (uint8_t)((a < 0x0Au) ? (a + 0x30u) : (a + 0x37u));

        /* Shift the bit mask 4 bits to the right, to convert the next digit */
        mask = (uint16_t) (mask >> 4u);

        /* Decrement the bit_shift counter by 4 (bits in each digit) */
        bit_shift = (uint8_t)(bit_shift - 4u);

        /* Increment the output string location */
        pos++;
    }
}
/*******************************************************************************
* End of function uint16_to_string
*******************************************************************************/
