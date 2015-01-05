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
* File Name     : rspi.c
* Device(s)     : RZ/A1H RSK+RZA1H
* H/W Platform  : RSK+RZA1H CPU Board
* Description   : This file implements device driver for RSPI module.
*******************************************************************************/
/*******************************************************************************
* History       : DD.MM.YYYY Version Description
*               : 18.06.2013 1.00
*******************************************************************************/

/******************************************************************************
Pragma directive
******************************************************************************/

/******************************************************************************
Includes
******************************************************************************/
#include "r_typedefs.h"                   /* Default  type definition header */
#include "iodefine.h"                            /* I/O Register root header */
#include "devdrv_intc.h"                               /* INTC Driver Header */
#include "rspi.h"                           /* Device driver for RSPI header */
#include "rza_io_regrw.h"            /* Low level register read/write header */
#include "compiler_settings.h"   /* Interchangeable compiler specific header */

/******************************************************************************
Global variables and functions
******************************************************************************/
/* RSPI1 transmit buffer address */
uint32_t * gp_rspi1_tx_address;
/* RSPI1 transmit data number */
uint16_t   g_rspi1_tx_count;
/* RSPI1 receive buffer address */
uint32_t * gp_rspi1_rx_address;
/* RSPI1 receive data number */
uint16_t   g_rspi1_rx_count;
/* RSPI1 receive data length */
uint16_t   g_rspi1_rx_length;

/* SCI5 transmit buffer address */
uint8_t * gp_spi_tx_address;
/* SCI5 transmit data number */
uint16_t  g_spi_tx_count;

/***********************************************************************************************************************
* Function Name: R_RSPI1_Create
* Description  : This function initialises the RSPI1 module.
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
void R_RSPI1_Create(void)
{
	uint16_t dummy_word  = 0u;
	uint8_t  dummy_byte  = 0u;

    UNUSED_VARIABLE(dummy_word);
    UNUSED_VARIABLE(dummy_byte);

	/* Configure and disable SPI0 transmit interrupts */
	R_INTC_Disable(INTC_ID_SPTI4);
	R_INTC_RegistIntFunc(INTC_ID_SPTI4, &r_rspi1_transmit_interrupt);
	R_INTC_SetPriority(INTC_ID_SPTI4, ADC_PRI_SPTI1);

	RSPI1.SPPCR = 0u;
	dummy_byte = RSPI1.SPPCR;
    /* P1 clock = 66.67MHz, SPI bit rate = 11.11Mbits/s Check Table 16.3 */
    RSPI1.SPBR  = 2u;
	dummy_byte = RSPI1.SPBR;
    RSPI1.SPDCR = 0x20u;
    dummy_byte = RSPI1.SPDCR;
    RSPI1.SPSCR = 0u;
	dummy_byte = RSPI1.SPSCR;
    RSPI1.SPCKD = 0;
	dummy_byte = RSPI1.SPCKD;
    RSPI1.SSLND = 0u;
    dummy_byte =  RSPI1.SSLND;
    RSPI1.SPND  = 0u;
    dummy_byte = RSPI1.SPND;
    RSPI1.SSLP  = 0u;
    dummy_byte = RSPI1.SSLP;
    RSPI1.SPSSR  = 0u;
    dummy_byte = RSPI1.SPSSR;
    RSPI1.SPBFCR  |= 0x30u;			//Receive Buffer Data Triggering Number set to 1bytes
    dummy_byte = RSPI1.SPBFCR;
    /* LSB first, use 16 bit data length */
    RSPI1.SPCMD0 = 0x0703u;
    dummy_word = RSPI1.SPCMD0;

    /* Set P1_4 as CS pin for PMOD1 */
    RZA_IO_RegWrite_16(&GPIO.PM1,   0, GPIO_PM1_PM14_SHIFT,    GPIO_PM1_PM14);
    RZA_IO_RegWrite_16(&GPIO.PMC1,  0, GPIO_PMC1_PMC14_SHIFT,  GPIO_PMC1_PMC14);
    RZA_IO_RegWrite_16(&GPIO.PFCAE1,0, GPIO_PFCAE1_PFCAE14_SHIFT,GPIO_PFCAE1_PFCAE14);
    RZA_IO_RegWrite_16(&GPIO.PFCE1, 0, GPIO_PFCE1_PFCE14_SHIFT, GPIO_PFCE1_PFCE14);
    RZA_IO_RegWrite_16(&GPIO.PFC1,  0, GPIO_PFC1_PFC14_SHIFT,  GPIO_PFC1_PFC14);
    RZA_IO_RegWrite_16(&GPIO.PIPC1, 0, GPIO_PIPC1_PIPC14_SHIFT, GPIO_PIPC1_PIPC14);

	/* Set P1_5 as CS pin for PMOD2 */
    RZA_IO_RegWrite_16(&GPIO.PM1,   0, GPIO_PM1_PM15_SHIFT,    GPIO_PM1_PM15);
    RZA_IO_RegWrite_16(&GPIO.PMC1,  0, GPIO_PMC1_PMC15_SHIFT,  GPIO_PMC1_PMC15);
    RZA_IO_RegWrite_16(&GPIO.PFCAE1,0, GPIO_PFCAE1_PFCAE15_SHIFT,GPIO_PFCAE1_PFCAE15);
    RZA_IO_RegWrite_16(&GPIO.PFCE1, 0, GPIO_PFCE1_PFCE15_SHIFT, GPIO_PFCE1_PFCE15);
    RZA_IO_RegWrite_16(&GPIO.PFC1,  0, GPIO_PFC1_PFC15_SHIFT,  GPIO_PFC1_PFC15);
    RZA_IO_RegWrite_16(&GPIO.PIPC1, 0, GPIO_PIPC1_PIPC15_SHIFT, GPIO_PIPC1_PIPC15);

    /* Set RSPCKA as output pin (P11_12) */
    RZA_IO_RegWrite_16(&GPIO.PM11,   0, GPIO_PM11_PM1112_SHIFT,    GPIO_PM11_PM1112);
    RZA_IO_RegWrite_16(&GPIO.PMC11,  1, GPIO_PMC11_PMC1112_SHIFT,  GPIO_PMC11_PMC1112);
    RZA_IO_RegWrite_16(&GPIO.PFCAE11,0, GPIO_PFCAE11_PFCAE1112_SHIFT,GPIO_PFCAE11_PFCAE1112);
    RZA_IO_RegWrite_16(&GPIO.PFCE11, 0, GPIO_PFCE11_PFCE1112_SHIFT, GPIO_PFCE11_PFCE1112);
    RZA_IO_RegWrite_16(&GPIO.PFC11,  1, GPIO_PFC11_PFC1112_SHIFT,  GPIO_PFC11_PFC1112);
    RZA_IO_RegWrite_16(&GPIO.PIPC11, 1, GPIO_PIPC11_PIPC1112_SHIFT, GPIO_PIPC11_PIPC1112);

	/* Set MOSIA as output pin (P11_14) */
    RZA_IO_RegWrite_16(&GPIO.PM11,   0, GPIO_PM11_PM1114_SHIFT,    GPIO_PM11_PM1114);
    RZA_IO_RegWrite_16(&GPIO.PMC11,  1, GPIO_PMC11_PMC1114_SHIFT,  GPIO_PMC11_PMC1114);
    RZA_IO_RegWrite_16(&GPIO.PFCAE11,0, GPIO_PFCAE11_PFCAE1114_SHIFT,GPIO_PFCAE11_PFCAE1114);
    RZA_IO_RegWrite_16(&GPIO.PFCE11, 0, GPIO_PFCE11_PFCE1114_SHIFT, GPIO_PFCE11_PFCE1114);
    RZA_IO_RegWrite_16(&GPIO.PFC11,  1, GPIO_PFC11_PFC1114_SHIFT,  GPIO_PFC11_PFC1114);
    RZA_IO_RegWrite_16(&GPIO.PIPC11, 1, GPIO_PIPC11_PIPC1114_SHIFT, GPIO_PIPC11_PIPC1114);

    /* Set MISOA as input pin (P11_15) */
    RZA_IO_RegWrite_16(&GPIO.PM11,   1, GPIO_PM11_PM1115_SHIFT,    GPIO_PM11_PM1115);
    RZA_IO_RegWrite_16(&GPIO.PMC11,  1, GPIO_PMC11_PMC1115_SHIFT,  GPIO_PMC11_PMC1115);
    RZA_IO_RegWrite_16(&GPIO.PFCAE11,0, GPIO_PFCAE11_PFCAE1115_SHIFT,GPIO_PFCAE11_PFCAE1115);
    RZA_IO_RegWrite_16(&GPIO.PFCE11, 0, GPIO_PFCE11_PFCE1115_SHIFT, GPIO_PFCE11_PFCE1115);
    RZA_IO_RegWrite_16(&GPIO.PFC11,  1, GPIO_PFC11_PFC1115_SHIFT,  GPIO_PFC11_PFC1115);
    RZA_IO_RegWrite_16(&GPIO.PIPC11, 1, GPIO_PIPC11_PIPC1115_SHIFT, GPIO_PIPC11_PIPC1115);

    /* The following two should have been set during port expander initialisation.
     * PMOD LCD code does not seem to use the reset line */
	/* Set Port EXpander 1 IO[4] as PMOD1 reset */
	/* Set Port EXpander 2 IO[5] as PMOD2 reset */

	/* Set P1_3 as PMOD IRQ pin (input) for both PMOD connectors */
   	RZA_IO_RegWrite_16(&GPIO.PM1,   1, GPIO_PM1_PM13_SHIFT,    GPIO_PM1_PM13);
    RZA_IO_RegWrite_16(&GPIO.PMC1,  1, GPIO_PMC1_PMC13_SHIFT,  GPIO_PMC1_PMC13);
    RZA_IO_RegWrite_16(&GPIO.PFCAE1,0, GPIO_PFCAE1_PFCAE13_SHIFT,GPIO_PFCAE1_PFCAE13);
    RZA_IO_RegWrite_16(&GPIO.PFCE1, 0, GPIO_PFCE1_PFCE13_SHIFT, GPIO_PFCE1_PFCE13);
    RZA_IO_RegWrite_16(&GPIO.PFC1,  1, GPIO_PFC1_PFC13_SHIFT,  GPIO_PFC1_PFC13);
    RZA_IO_RegWrite_16(&GPIO.PIPC1, 1, GPIO_PIPC1_PIPC13_SHIFT, GPIO_PIPC1_PIPC13);

	/* Set P4_15 as PMOD pin9 (output) */
   	RZA_IO_RegWrite_16(&GPIO.PM4,   0, GPIO_PM4_PM415_SHIFT,    GPIO_PM4_PM415);
    RZA_IO_RegWrite_16(&GPIO.PMC4,  0, GPIO_PMC4_PMC415_SHIFT,  GPIO_PMC4_PMC415);
    RZA_IO_RegWrite_16(&GPIO.PFCAE4,0, GPIO_PFCAE4_PFCAE415_SHIFT,GPIO_PFCAE4_PFCAE415);
    RZA_IO_RegWrite_16(&GPIO.PFCE4, 0, GPIO_PFCE4_PFCE415_SHIFT, GPIO_PFCE4_PFCE415);
    RZA_IO_RegWrite_16(&GPIO.PFC4,  0, GPIO_PFC4_PFC415_SHIFT,  GPIO_PFC4_PFC415);
    RZA_IO_RegWrite_16(&GPIO.PIPC4, 0, GPIO_PIPC4_PIPC415_SHIFT, GPIO_PIPC4_PIPC415);

	/* Set P3_7 as PMOD pin10 (output) */
   	RZA_IO_RegWrite_16(&GPIO.PM3,   0, GPIO_PM3_PM37_SHIFT,    GPIO_PM3_PM37);
    RZA_IO_RegWrite_16(&GPIO.PMC3,  0, GPIO_PMC3_PMC37_SHIFT,  GPIO_PMC3_PMC37);
    RZA_IO_RegWrite_16(&GPIO.PFCAE3,0, GPIO_PFCAE3_PFCAE37_SHIFT,GPIO_PFCAE3_PFCAE37);
    RZA_IO_RegWrite_16(&GPIO.PFCE3, 0, GPIO_PFCE3_PFCE37_SHIFT, GPIO_PFCE3_PFCE37);
    RZA_IO_RegWrite_16(&GPIO.PFC3,  0, GPIO_PFC3_PFC37_SHIFT,  GPIO_PFC3_PFC37);
    RZA_IO_RegWrite_16(&GPIO.PIPC3, 0, GPIO_PIPC3_PIPC37_SHIFT, GPIO_PIPC3_PIPC37);


   	/* Enable master mode */
	RSPI1.SPCR |= 0x28u;
	dummy_byte = RSPI1.SPCR;
}

/***********************************************************************************************************************
* Function Name: R_RSPI1_Start
* Description  : This function starts the RSPI1 module operation.
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
void R_RSPI1_Start(void)
{
    volatile uint8_t dummy;

    UNUSED_VARIABLE(dummy);

    /* Enable RSPI interrupts */
	R_INTC_Enable(INTC_ID_SPTI4);
	R_INTC_Enable(INTC_ID_SPRI4);

    /* Clear error sources */
    dummy = RSPI1.SPSR;
    RSPI1.SPSR = 0x00U;

    if (0 == RZA_IO_RegRead_8(&(RSPI1.SPSR),
                                RSPIn_SPSR_MODF_SHIFT,
    		                    RSPIn_SPSR_MODF))
	{
		//test
		RSPI1.SPCR |= 0x40U;
		dummy = RSPI1.SPCR;
	}
	else
	{
		//clear the MODF flag then set the SPE bit
		dummy = RSPI1.SPSR;

		RSPI1.SPCR |= 0x40U;
		dummy = RSPI1.SPCR;
	}
}

/***********************************************************************************************************************
* Function Name: R_RSPI1_Stop
* Description  : This function stops the RSPI1 module operation.
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
void R_RSPI1_Stop(void)
{
    /* Disable RSPI interrupts */
	R_INTC_Disable(INTC_ID_SPTI4);
    /* Disable RSPI function */
	RZA_IO_RegWrite_8(&(RSPI1.SPCR), 0, RSPIn_SPCR_SPE_SHIFT, RSPIn_SPCR_SPE);

}
/***********************************************************************************************************************
* Function Name: R_RSPI1_Send_Receive
* Description  : This function sends and receives CSI#n1 data.
* Arguments    : tx_buf -
*                    transfer buffer pointer (not used when data is handled by DTC)
*                tx_num -
*                    buffer size
*                rx_buf -
*                    receive buffer pointer (not used when data is handled by DTC)
* Return Value : status -
*                    MD_OK or MD_ARGERROR
***********************************************************************************************************************/
uint8_t R_RSPI1_Send_Receive(uint32_t * const tx_buf, uint16_t tx_num, uint32_t * const rx_buf)
{
	uint8_t status = 0u;

    if (tx_num < 1U)
    {
        status = 1u;
    }
    else
    {
		gp_rspi1_tx_address = tx_buf;
        g_rspi1_tx_count = tx_num;

        gp_rspi1_rx_address = rx_buf;
        g_rspi1_rx_length = tx_num;
        g_rspi1_rx_count = 0U;

        /* Enable transmit interrupt */
    	RZA_IO_RegWrite_8(&(RSPI1.SPCR), (uint8_t)1, RSPIn_SPCR_SPTIE_SHIFT, RSPIn_SPCR_SPTIE);

        /* Enable receive interrupt */
    	RZA_IO_RegWrite_8(&(RSPI1.SPCR), 1, RSPIn_SPCR_SPRIE_SHIFT, RSPIn_SPCR_SPRIE);

        /* Enable RSPI function */
    	RZA_IO_RegWrite_8(&(RSPI1.SPCR), 1, RSPIn_SPCR_SPE_SHIFT, RSPIn_SPCR_SPE);
    }

    return (status);
}

/***********************************************************************************************************************
* Function Name: R_RSPI1_LoopBackReversed
* Description  : This function .
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
void R_RSPI1_LoopBackReversed(void)
{
	RZA_IO_RegWrite_8(&(RSPI1.SPPCR), (uint8_t)1, RSPIn_SPPCR_SPLP_SHIFT, RSPIn_SPPCR_SPLP);
}

/***********************************************************************************************************************
* Function Name: R_RSPI1_LoopBackDisable
* Description  : This function disable loopback mode.
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
void R_RSPI1_LoopBackDisable(void)
{
	RZA_IO_RegWrite_8(&(RSPI1.SPPCR), 0, RSPIn_SPPCR_SPLP_SHIFT, RSPIn_SPPCR_SPLP);
}
