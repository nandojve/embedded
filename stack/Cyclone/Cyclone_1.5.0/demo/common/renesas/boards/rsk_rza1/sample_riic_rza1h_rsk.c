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
/******************************************************************************
* File Name     : sample_riic_rza1h_rsk.c
* Device(s)     : RZ/A1H RSK+RZA1H
* H/W Platform  : RSK+RZA1H CPU Board
* Description   : RSK+RZA1H RI2C Access sample,
*               :
*               : DEVDRV_CH_0 RSK TFT APP BOARD YROK77210C000BE devices
*               :  Capacitive touch panel controller
*               :  PanelBus~(TM) flat panel display
*               :
*               : DEVDRV_CH_0 RSK TFT APP BOARD YROK77210C000BE devices
*               :  MAX9856 is a high-performance, low-power stereo audio CODEC
*               :  2 * CMOS 8 bit parallel input/output port expansion for I2C
*               :  Renesas two-wire serial interface EEPROM
******************************************************************************/
/*******************************************************************************
* History       : DD.MM.YYYY Version Description
*               : 18.06.2013 1.00
*******************************************************************************/

/******************************************************************************
Includes   <System Includes> , "Project Includes"
******************************************************************************/
#include <stdio.h>
#include <string.h>

#include "r_typedefs.h"                   /* Default  type definition header */
#include "iodefine.h"                            /* I/O Register root header */
#include "dev_drv.h"                                 /* Device driver header */
#include "devdrv_riic.h"                               /* RIIC Driver Header */
#include "devdrv_intc.h"                               /* INTC Driver Header */
#include "sample_riic_rza1h_rsk_i2c.h"   /* I2C RSK+RZA1H Eval-board header */
#include "rza_io_regrw.h"            /* Low level register read/write header */

/******************************************************************************
Typedef definitions
******************************************************************************/

/******************************************************************************
Macro definitions
******************************************************************************/
#define SAMPLE_RIIC_RWCODE_W        (0)               /* IIC data write code */
#define SAMPLE_RIIC_RWCODE_R        (1)               /* IIC data read code  */
#define DELAY_TIME_1MS       (260000uL)                     /* Sampling loop */


/******************************************************************************
Imported global variables and functions (from other files)
******************************************************************************/

/******************************************************************************
Exported global variables and functions (to be accessed by other files)
******************************************************************************/
uint8_t  g_w_byte;
uint8_t  g_config_io;
uint8_t  g_r_buffer = 0u;
uint8_t  port_x_state = 0u;

int32_t touch_read_request = 0;

/******************************************************************************
Private global variables and functions
******************************************************************************/


/******************************************************************************
* Function Name: Delay1ms
* Description  : Make the sampling interval
* Arguments    : None
* Return Value : None
******************************************************************************/
static void Delay1ms(void)
{
    uint32_t    delay_count = 0u;

    while (DELAY_TIME_1MS >= delay_count)
    {
    	delay_count++;
    }
}


#if RZA1H_APP_I2C_TFP410_ISR_EN > 0
/******************************************************************************
* Function Name: Delay10ms
* Description  : Make the sampling interval
* Arguments    : None
* Return Value : None
******************************************************************************/
static void Delay10ms(void)
{
    uint32_t    delay_count = 10u;

    while (delay_count--)
    {
    	Delay1ms();
    }
}
#endif

/******************************************************************************
* Function Name: RIIC0_PORT_Init
* Description  : Initialises channel 0 i2c port register for this board
* Arguments    : None
* Return Value : None
******************************************************************************/
static void RIIC0_PORT_Init(void)
{
    /* ---- P1_0 : SCL0 ---- */
    /* Port initialize */
    RZA_IO_RegWrite_16(&GPIO.PIBC1,  0, GPIO_PIBC1_PIBC10_SHIFT,   GPIO_PIBC1_PIBC10);
    RZA_IO_RegWrite_16(&GPIO.PBDC1,  1, GPIO_PBDC1_PBDC10_SHIFT,   GPIO_PBDC1_PBDC10);
    RZA_IO_RegWrite_16(&GPIO.PM1,    1, GPIO_PM1_PM10_SHIFT,       GPIO_PM1_PM10);
    RZA_IO_RegWrite_16(&GPIO.PMC1,   0, GPIO_PMC1_PMC10_SHIFT,     GPIO_PMC1_PMC10);
    RZA_IO_RegWrite_16(&GPIO.PIPC1,  1, GPIO_PIPC1_PIPC10_SHIFT,   GPIO_PIPC1_PIPC10);
    /* Port mode : Multiplex mode                     */
    /* Port function setting : 1st multiplex function */
    /* I/O control mode : Peripheral function         */
    /* Bidirectional mode : Enable                    */
    RZA_IO_RegWrite_16(&GPIO.PBDC1,  1, GPIO_PBDC1_PBDC10_SHIFT,   GPIO_PBDC1_PBDC10);
    RZA_IO_RegWrite_16(&GPIO.PFC1,   0, GPIO_PFC1_PFC10_SHIFT,     GPIO_PFC1_PFC10);
    RZA_IO_RegWrite_16(&GPIO.PFCE1,  0, GPIO_PFCE1_PFCE10_SHIFT,   GPIO_PFCE1_PFCE10);
    RZA_IO_RegWrite_16(&GPIO.PFCAE1, 0, GPIO_PFCAE1_PFCAE10_SHIFT, GPIO_PFCAE1_PFCAE10);
    RZA_IO_RegWrite_16(&GPIO.PIPC1,  1, GPIO_PIPC1_PIPC10_SHIFT,   GPIO_PIPC1_PIPC10);
    RZA_IO_RegWrite_16(&GPIO.PMC1,   1, GPIO_PMC1_PMC10_SHIFT,     GPIO_PMC1_PMC10);

    /* ---- P1_1 : SDA0 ---- */
    /* Port initialise */
    RZA_IO_RegWrite_16(&GPIO.PIBC1,  0, GPIO_PIBC1_PIBC11_SHIFT,   GPIO_PIBC1_PIBC11);
    RZA_IO_RegWrite_16(&GPIO.PBDC1,  1, GPIO_PBDC1_PBDC11_SHIFT,   GPIO_PBDC1_PBDC11);
    RZA_IO_RegWrite_16(&GPIO.PM1,    1, GPIO_PM1_PM11_SHIFT,       GPIO_PM1_PM11);
    RZA_IO_RegWrite_16(&GPIO.PMC1,   0, GPIO_PMC1_PMC11_SHIFT,     GPIO_PMC1_PMC11);
    RZA_IO_RegWrite_16(&GPIO.PIPC1,  1, GPIO_PIPC1_PIPC11_SHIFT,   GPIO_PIPC1_PIPC11);
    /* Port mode : Multiplex mode                     */
    /* Port function setting : 1st multiplex function */
    /* I/O control mode : Peripheral function         */
    /* Bidirectional mode : Enable                    */
    RZA_IO_RegWrite_16(&GPIO.PBDC1,  1, GPIO_PBDC1_PBDC11_SHIFT,   GPIO_PBDC1_PBDC11);
    RZA_IO_RegWrite_16(&GPIO.PFC1,   0, GPIO_PFC1_PFC11_SHIFT,     GPIO_PFC1_PFC11);
    RZA_IO_RegWrite_16(&GPIO.PFCE1,  0, GPIO_PFCE1_PFCE11_SHIFT,   GPIO_PFCE1_PFCE11);
    RZA_IO_RegWrite_16(&GPIO.PFCAE1, 0, GPIO_PFCAE1_PFCAE11_SHIFT, GPIO_PFCAE1_PFCAE11);
    RZA_IO_RegWrite_16(&GPIO.PIPC1,  1, GPIO_PIPC1_PIPC11_SHIFT,   GPIO_PIPC1_PIPC11);
    RZA_IO_RegWrite_16(&GPIO.PMC1,   1, GPIO_PMC1_PMC11_SHIFT,     GPIO_PMC1_PMC11);
}

#if RZA1H_APP_I2C_TFP410_ISR_EN > 0
/******************************************************************************
* Function Name: IntIRQ1 (TP_INT)
* Description  : IRQ1 interrupt
*              : Handler for IRQ1 interrupts
* Arguments    : uint32_t int_sense : Interrupt detection
*              :                    :   INTC_LEVEL_SENSITIVE : Level sense
*              :                    :   INTC_EDGE_TRIGGER    : Edge trigger
* Return Value : none
******************************************************************************/
static void IntIRQ1_TouchPanel(uint32_t int_sense)
{
	UNUSED_PARAM(int_sense);

    R_INTC_Disable(INTC_ID_IRQ1);

	Delay10ms();

	touch_read_request++;

    /* Clear IRQ flag  */
    if (1 == RZA_IO_RegRead_16(&(INTCIRQRR),
    		                    INTC_IRQRR_IRQ1F_SHIFT,
    		                    INTC_IRQRR_IRQ1F_SHIFT))
    {
        RZA_IO_RegWrite_16(&INTCIRQRR,
        		            0,
        		            INTC_IRQRR_IRQ1F_SHIFT,
        		            INTC_IRQRR_IRQ1F_SHIFT);
    }


    R_INTC_Enable(INTC_ID_IRQ1);
}
#endif

/******************************************************************************
* Function Name: R_RIIC_Init_CH0
* Description  : Initialises channel 0 i2c for this board
* Arguments    : None
* Return Value : None
******************************************************************************/
void R_RIIC_Init_CH0(void)
{
	uint32_t cks;
	uint32_t brl;
	uint32_t brh;

	RIIC0_PORT_Init();

	/* End configuration register settings */
	cks = RIIC_CKS_DIVISION_8;
	brl = 19;
	brh = 16;

	R_RIIC_Init(DEVDRV_CH_0, cks, brl, brh);

#if RZA1H_APP_I2C_TFP410_ISR_EN > 0

	/* Initialise Touch Panel Interrupt Set TP_INT (P4_9) as IRQ1 */
    RZA_IO_RegWrite_16(&GPIO.PBDC4,  1, GPIO_PBDC4_PBDC49_SHIFT,   GPIO_PBDC4_PBDC49);
    RZA_IO_RegWrite_16(&GPIO.PM4,    1, GPIO_PM4_PM49_SHIFT,       GPIO_PM4_PM49);
    RZA_IO_RegWrite_16(&GPIO.P4,     1, GPIO_P4_P49_SHIFT,         GPIO_P4_P49);
    RZA_IO_RegWrite_16(&GPIO.PMC4,   1, GPIO_PMC4_PMC49_SHIFT,     GPIO_PMC4_PMC49);
    RZA_IO_RegWrite_16(&GPIO.PFCAE4, 1, GPIO_PFCAE4_PFCAE49_SHIFT, GPIO_PFCAE4_PFCAE49);
    RZA_IO_RegWrite_16(&GPIO.PFCE4,  1, GPIO_PFCE4_PFCE49_SHIFT,   GPIO_PFCE4_PFCE49);
    RZA_IO_RegWrite_16(&GPIO.PFC4,   1, GPIO_PFC4_PFC49_SHIFT,     GPIO_PFC4_PFC49);
    RZA_IO_RegWrite_16(&GPIO.PIPC4,  1, GPIO_PIPC4_PIPC49_SHIFT,   GPIO_PIPC4_PIPC49);

    /* Configure and enable IRQ1 interrupts received from the touch panel */
	R_INTC_Disable(INTC_ID_IRQ1);
	R_INTC_RegistIntFunc(INTC_ID_IRQ1, &IntIRQ1_TouchPanel);
	R_INTC_SetPriority(INTC_ID_IRQ1, 10uL);

   /* Enable the touch panel interrupt if required */
    R_INTC_Enable(INTC_ID_IRQ1);
#endif

}

/******************************************************************************
* Function Name: RIIC3_PORT_Init
* Description  : Initialises channel 3 port register for this board
* Arguments    : None
* Return Value : None
******************************************************************************/
static void RIIC3_PORT_Init(void)
{

	/* ---- P1_6 : SCL3 ---- */
    /* Port initialise */
    RZA_IO_RegWrite_16(&GPIO.PIBC1,  0, GPIO_PIBC1_PIBC16_SHIFT,   GPIO_PIBC1_PIBC16);
    RZA_IO_RegWrite_16(&GPIO.PBDC1,  1, GPIO_PBDC1_PBDC16_SHIFT,   GPIO_PBDC1_PBDC16);
    RZA_IO_RegWrite_16(&GPIO.PM1,    1, GPIO_PM1_PM16_SHIFT,       GPIO_PM1_PM16);
    RZA_IO_RegWrite_16(&GPIO.PMC1,   0, GPIO_PMC1_PMC16_SHIFT,     GPIO_PMC1_PMC16);
    RZA_IO_RegWrite_16(&GPIO.PIPC1,  1, GPIO_PIPC1_PIPC16_SHIFT,   GPIO_PIPC1_PIPC16);
    /* Port mode : Multiplex mode                     */
    /* Port function setting : 1st multiplex function */
    /* I/O control mode : Peripheral function         */
    /* Bidirectional mode : Enable                    */
    RZA_IO_RegWrite_16(&GPIO.PBDC1,  1, GPIO_PBDC1_PBDC16_SHIFT,   GPIO_PBDC1_PBDC16);
    RZA_IO_RegWrite_16(&GPIO.PFC1,   0, GPIO_PFC1_PFC16_SHIFT,     GPIO_PFC1_PFC16);
    RZA_IO_RegWrite_16(&GPIO.PFCE1,  0, GPIO_PFCE1_PFCE16_SHIFT,   GPIO_PFCE1_PFCE16);
    RZA_IO_RegWrite_16(&GPIO.PFCAE1, 0, GPIO_PFCAE1_PFCAE16_SHIFT, GPIO_PFCAE1_PFCAE16);
    RZA_IO_RegWrite_16(&GPIO.PIPC1,  1, GPIO_PIPC1_PIPC16_SHIFT,   GPIO_PIPC1_PIPC16);
    RZA_IO_RegWrite_16(&GPIO.PMC1,   1, GPIO_PMC1_PMC16_SHIFT,     GPIO_PMC1_PMC16);

    /* ---- P1_7 : SDA3 ---- */
    /* Port initialise */
    RZA_IO_RegWrite_16(&GPIO.PIBC1,  0, GPIO_PIBC1_PIBC17_SHIFT,   GPIO_PIBC1_PIBC17);
    RZA_IO_RegWrite_16(&GPIO.PBDC1,  1, GPIO_PBDC1_PBDC17_SHIFT,   GPIO_PBDC1_PBDC17);
    RZA_IO_RegWrite_16(&GPIO.PM1,    1, GPIO_PM1_PM17_SHIFT,       GPIO_PM1_PM17);
    RZA_IO_RegWrite_16(&GPIO.PMC1,   0, GPIO_PMC1_PMC17_SHIFT,     GPIO_PMC1_PMC17);
    RZA_IO_RegWrite_16(&GPIO.PIPC1,  1, GPIO_PIPC1_PIPC17_SHIFT,   GPIO_PIPC1_PIPC17);
    /* Port mode : Multiplex mode                     */
    /* Port function setting : 1st multiplex function */
    /* I/O control mode : Peripheral function         */
    /* Bidirectional mode : Enable                    */
    RZA_IO_RegWrite_16(&GPIO.PBDC1,  1, GPIO_PBDC1_PBDC17_SHIFT,   GPIO_PBDC1_PBDC17);
    RZA_IO_RegWrite_16(&GPIO.PFC1,   0, GPIO_PFC1_PFC17_SHIFT,     GPIO_PFC1_PFC17);
    RZA_IO_RegWrite_16(&GPIO.PFCE1,  0, GPIO_PFCE1_PFCE17_SHIFT,   GPIO_PFCE1_PFCE17);
    RZA_IO_RegWrite_16(&GPIO.PFCAE1, 0, GPIO_PFCAE1_PFCAE17_SHIFT, GPIO_PFCAE1_PFCAE17);
    RZA_IO_RegWrite_16(&GPIO.PIPC1,  1, GPIO_PIPC1_PIPC17_SHIFT,   GPIO_PIPC1_PIPC17);
    RZA_IO_RegWrite_16(&GPIO.PMC1,   1, GPIO_PMC1_PMC17_SHIFT,     GPIO_PMC1_PMC17);

}

/******************************************************************************
* Function Name: R_RIIC_Init_CH3
* Description  : Initialises channel 3 i2c for this board
* Arguments    : None
* Return Value : None
******************************************************************************/
void R_RIIC_Init_CH3(void)
{
	uint32_t cks;
	uint32_t brl;
	uint32_t brh;
	uint8_t  iic_dev_io;
	uint8_t  iic_w_value = 0xff;

	uint8_t buf[3] = {0,0,0};
	uint8_t buffer = 0;

	RIIC3_PORT_Init();


	cks = RIIC_CKS_DIVISION_8;
	brl = 19;
	brh = 16;

	R_RIIC_Init(DEVDRV_CH_3, cks, brl, brh);

	//Port Expander 1
	R_RIIC_SendCond(DEVDRV_CH_3, RIIC_TX_MODE_START);

    /* Set the port expander device address */
	buffer = (uint8_t)(((RZA1H_RSK_I2C_PX_IO1 & 0x7) << 1) | 0);

	buf[0]   = buffer | (RZA1H_RSK_I2C_PX_IO1 & 0xF0);
	buf[1]   = PX_CMD_CONFIG_REG;//d_command;
	buf[2]   = 0x00u;	//set the port pin directions
						/*
						 * IO[0] = LED1 			(output)
						 * IO[1] = LED2 			(output)
						 * IO[2] = LED3 			(output)
						 * IO[3] = NOR_A25 			(output)
						 * IO[4] = PMOD1_RST 		(output)
						 * IO[5] = PMOD2_RST 		(output)
						 * IO[6] = SD_CONN_PWR_EN 	(output)
						 * IO[7] = SD_MMC_PWR_EN 	(output)
						 */

	R_RIIC_Write(DEVDRV_CH_3, buf, 3);

	R_RIIC_SendCond(DEVDRV_CH_3, RIIC_TX_MODE_STOP);
	R_RIIC_DetectStop(DEVDRV_CH_3);

	Delay1ms();

	//Port Expander 2
	R_RIIC_SendCond(DEVDRV_CH_3, RIIC_TX_MODE_START);

   /* Set the port expander device address */
	buffer = (uint8_t)(((RZA1H_RSK_I2C_PX_IO2 & 0x7) << 1) | 0);

	buf[0]   = buffer | (RZA1H_RSK_I2C_PX_IO2 & 0xF0);
	buf[1]   = PX_CMD_CONFIG_REG;//d_command;
	buf[2]   = 0x10u;	//set the port pin directions
						/*
						 * IO[0] = PX1_EN0 			(output)
						 * IO[1] = PX1_EN1 			(output)
						 * IO[2] = TFT_CS 			(output)
						 * IO[3] = PX1_EN3 			(output)
						 * IO[4] = USB_OVR_CURRENT 	(input)
						 * IO[5] = USB_PWR_ENA 		(output)
						 * IO[6] = USB_PWR_ENB 		(output)
						 * IO[7] = PX1_EN7 			(output)
						 */

	R_RIIC_Write(DEVDRV_CH_3, buf, 3);

	R_RIIC_SendCond(DEVDRV_CH_3, RIIC_TX_MODE_STOP);
	R_RIIC_DetectStop(DEVDRV_CH_3);

	Delay1ms();

    /* Configure the port mode of each I/O of the port expander */
    iic_dev_io  = (uint8_t)(0xFF);	
	/* Indicates which I/O is to be modified
    IO[1] = LED2                   = 1 (OFF)
   	IO[2] = LED3                   = 1 (OFF)
    IO[3] = NOR A25                = 0
	IO[4] = PMOD1 Reset            = 1
    IO[5] = PMOD2 Reset            = 1
    IO[6] = SD Header Power Enable = 0 (OFF)
    IO[7] = SD/MMC Power Enable    = 1 (ON) */

    Sample_RIIC_PortExpAccess(DEVDRV_CH_3, RZA1H_RSK_I2C_PX_IO1, (uint8_t)PORTX_OUTPUT_REG, iic_dev_io, iic_w_value);

    /* Default switch off user LEDs LED1-LED3 on Port Expander 1 */
	Sample_RIIC_PortExpAccess(DEVDRV_CH_3, RZA1H_RSK_I2C_PX_IO1, (uint8_t)PORTX_OUTPUT_REG, 0, 7);

    /* Configure the port mode of each I/O of the port expander */
    iic_dev_io  = (uint8_t)(0xFF);	//Indicates which I/O is to be modified
    iic_w_value = (uint8_t)(0x13);  	//IO[0] = PX1_EN0 Enable        = 1 (Selects IC37/IC38 MUXes BL[x] signals. Also enables PMOD connectors as they use the SPI from IC38)
										//IO[1] = PX1_EN1 Enable        = 1 (ON)
										//IO[2] = TFT_CS                = 0 (Optional chip select for connecting to TFTs which use SPI. The RSK TFT is not configured by SPI.)
										//IO[3] = PX1_EN3 Enable        = 0 (Selects Audio signals. If set to 1, selects optional PWM signals.)
										//IO[4] = USB_OVR_CURRENT       = 1 (Input signal from IC7. By default it is pulled high. It signals overcurrent or thermal shutdown conditions.)
										//IO[5] = USB_PWR_ENA           = 0 (Pulled low by default, enabling power to VBUS0. JP11 needs to be shorted)
										//IO[6] = USB_PWR_ENB           = 0 (Pulled low by default, enabling power to VBUS1. JP12 needs to be shorted)
										//IO[7] = PX1_EN7               = 0 (Pulled low by default, selecting A18 - A21 lines. If set to 1, SGOUTx signals are selected.)

    Sample_RIIC_PortExpAccess(DEVDRV_CH_3, RZA1H_RSK_I2C_PX_IO2, (uint8_t)PORTX_OUTPUT_REG, iic_dev_io, iic_w_value);

}


/******************************************************************************
* Function Name: R_RIIC_rza1h_rsk_init
* Description  : Manages the initialisation of the i2c channels
* Arguments    : None
* Return Value : None
******************************************************************************/
void R_RIIC_rza1h_rsk_init(void)
{
    R_RIIC_Init_CH0();
    R_RIIC_Init_CH3();
}

/******************************************************************************
* Function Name: R_RIIC_rza1h_rsk_read
* Description  : Read data form slave in single byte addressing mode
* Arguments    : uint32_t channel : RIIC channel (0 to 3)
*              : uint8_t d_adr    : Slave device address
*              : uint16_t r_adr   : Slave sub-address
*              : uint32_t r_byte  : Number of bytes
*              : uint8_t * r_buffer : buffer for data
* Return Value : DEVDRV_SUCCESS   : Success of RIIC operation
*              : DEVDRV_ERROR     : Failure of RIIC operation
******************************************************************************/
int32_t R_RIIC_rza1h_rsk_read(uint32_t channel,
                              uint8_t d_adr,
                              uint16_t r_adr,
                              uint32_t r_byte,
                              uint8_t * r_buffer)
{
    int32_t  ret;
    uint8_t  w_buffer[3];
    uint8_t  dummy_data;

    w_buffer[0] = (uint8_t)(d_adr | SAMPLE_RIIC_RWCODE_W);
    w_buffer[1] = (uint8_t)(r_adr & 0x00ff);
    w_buffer[2] = (uint8_t)(d_adr | SAMPLE_RIIC_RWCODE_R);

    R_RIIC_SendCond(channel, RIIC_TX_MODE_START);

	/* For EEPROM devices with 1 byte addressing */
	ret = R_RIIC_Write(channel, w_buffer, 2);

    if (DEVDRV_SUCCESS == ret)
    {
    	R_RIIC_SendCond(channel, RIIC_TX_MODE_RESTART);

        ret = R_RIIC_SingleWrite(channel, w_buffer[2], RIIC_TEND_WAIT_RECEIVE);

        if (DEVDRV_SUCCESS == ret)
        {
			R_RIIC_SingleRead(channel, &dummy_data);

			R_RIIC_Read(channel, r_buffer, r_byte);

			R_RIIC_SendCond(channel, RIIC_TX_MODE_STOP);
		}
		else
		{
			R_RIIC_SendCond(channel, RIIC_TX_MODE_STOP);
			R_RIIC_SingleRead(channel, &dummy_data);
			R_RIIC_DetectStop(channel);
		}
    }

    return ret;
}

/******************************************************************************
* Function Name: R_RIIC_rza1h_rsk_write
* Description  : Write data to slave in single byte addressing mode
* Arguments    : uint32_t channel : RIIC channel (0 to 3)
*              : uint8_t d_adr    : Slave device address
*              : uint16_t w_adr   : Slave sub-address
*              : uint32_t w_byte  : Number of bytes
*              : uint8_t * w_buffer : buffer for data
* Return Value : DEVDRV_SUCCESS   : Success of RIIC operation
*              : DEVDRV_ERROR     : Failure of RIIC operation
******************************************************************************/
int32_t R_RIIC_rza1h_rsk_write(uint32_t channel,
		                       uint8_t d_adr,
		                       uint16_t w_adr,
		                       uint32_t w_byte,
		                       uint8_t * w_buffer)
{
    int32_t  ret;
    uint8_t  buf[3];

    buf[0] = (uint8_t)(d_adr | SAMPLE_RIIC_RWCODE_W);
    buf[1] = (uint8_t)(w_adr  & 0x00ff);

    R_RIIC_SendCond(channel, RIIC_TX_MODE_START);

	ret = R_RIIC_Write(channel, buf, 2);

    if (DEVDRV_SUCCESS == ret)
    {
        ret = R_RIIC_Write(channel, w_buffer, w_byte);
    }

    R_RIIC_SendCond(channel, RIIC_TX_MODE_STOP);
    R_RIIC_DetectStop(channel);

    return ret;
}


/******************************************************************************
* Function Name: Sample_RIIC_PortExpAccess
* Description  : Function to write to a port expander's registers:
*                output port
*                configuration
*                polarity
* Arguments    : none
* Return Value : ret : errorcode
******************************************************************************/
int32_t Sample_RIIC_PortExpAccess(uint32_t channel, uint8_t d_code, uint8_t reg_command, uint8_t config_io, uint8_t w_byte)
{
    int32_t  ret = DEVDRV_ERROR;
    uint8_t  buf[3];
    uint8_t j;

	port_x_state = g_r_buffer;

	/* Update the port expander's IO states if required */
	if (PORTX_OUTPUT_REG == reg_command)
	{
		/* Re-write the new bit sequence to the buffer */
		for (j = 0u; j < 8u; j++)
		{
			/* Check which port expander output bit needs change */
			if (1u == ((config_io >> j) & (uint8_t)1u))
			{
				/* Check if a config_io bit needs to be set or cleared */
				if (1u == ((uint8_t)1u & (w_byte >> j)))
				{
					/* Set the bit pointed by the loop counter j */
					port_x_state |= (uint8_t)(1u << j);
				}
				else
				{
					/* Clear the bit pointed by the loop counter j */
					port_x_state &= (uint8_t)(~(1u << j));
				}
			}
		}

		/* Set the port expander's device address */
		buf[0] = (uint8_t)(((d_code & 0x7u) << 1u) | SAMPLE_RIIC_RWCODE_W);
		buf[0] = (uint8_t)(buf[0] | (d_code & 0xF0));

		/* Set the command byte */
		buf[1] = reg_command;

		/* Set the port expander's port data */
		buf[2] = port_x_state;

		/* While for the bus to become free */
	    R_RIIC_DetectStop(channel);

	    R_RIIC_SendCond(channel, RIIC_TX_MODE_START);

		/* Write 3 bytes,
		 * port expander address,
		 * command,
		 * port expander output data */
		ret = R_RIIC_Write(channel, buf, 3);

		R_RIIC_SendCond(channel, RIIC_TX_MODE_STOP);
		R_RIIC_DetectStop(channel);

		/* Ensure to check for acknowledge */
		__asm("nop");
	}

    return ret;
}

/******************************************************************************
* Function Name: Sample_RIIC_ScanPort
* Description  : Function reads any port expander's register
*              : REQUIRES SERIAL IO.
* Arguments    : uint32_t channel : RIIC channel (0 to 3)
* Return Value : none
******************************************************************************/
int32_t Sample_RIIC_ScanPort(uint32_t channel)
{
    uint8_t  buf[32];
    uint8_t  dummy_data = 0u;
    uint8_t  addr;

    /*  Argument check  */
    if (channel >= RIIC_CH_TOTAL)
    {
        return DEVDRV_ERROR;
    }

    for( addr = 0; addr < 0x100; addr++)
    {
    	memset(&buf[0], 0, 16);
    	R_RIIC_rza1h_rsk_read(channel,addr,0,16,buf);
		printf("Addr [0x%02x] - ", addr);
		for(dummy_data = 0; dummy_data < 16; dummy_data++)
		{
			printf("[0x%02x] ", buf[dummy_data]);
		}
		printf("\r\n");

		memset(&buf[0], 0, 16);
    }

	return DEVDRV_SUCCESS;
}


/******************************************************************************
* Function Name: Sample_R_RIIC_LED_PE_Toggle
* Description  : Toggle function for expander 1's LED's
* Arguments    : uint8_t status : Active LED's PX_1_LED1, PX_1_LED2, PX_1_LED3
* Return Value : none
******************************************************************************/
void Sample_R_RIIC_LED_PE_Toggle(uint8_t status)
{
	uint8_t current;

    R_RIIC_rza1h_rsk_read(DEVDRV_CH_3,
    		              RZA1H_RSK_I2C_PX_IO1,
    		              (uint8_t)PORTX_OUTPUT_REG, 0x1, &current);

	if(PX_1_LED1 == (status & PX_1_LED1))
	{
		if(current & PX_1_LED1)
		{
			current = (uint8_t)(current & ~PX_1_LED1);
		}
		else
		{
			current = (uint8_t)(current | PX_1_LED1);
		}
	}

	if(PX_1_LED2 == (status & PX_1_LED2))
	{
		if(current & PX_1_LED2)
		{
			current = (uint8_t)(current & ~PX_1_LED2);
		}
		else
		{
			current = (uint8_t)(current | PX_1_LED2);
		}
	}

	if(PX_1_LED3 == (status & PX_1_LED3))
	{
		if(current & PX_1_LED3)
		{
			current = (uint8_t)(current & ~PX_1_LED3);
		}
		else
		{
			current = (uint8_t)(current | PX_1_LED3);
		}
	}


    R_RIIC_rza1h_rsk_write(DEVDRV_CH_3,
    		               RZA1H_RSK_I2C_PX_IO1,
    		               (uint8_t)PORTX_OUTPUT_REG, 0x1, &current);
}

/******************************************************************************
* Function Name: Sample_R_RIIC_LED_PE_On
* Description  : LED Off function for expander 1's LED's
* Arguments    : uint8_t status : Active LED's PX_1_LED1, PX_1_LED2, PX_1_LED3
* Return Value : none
******************************************************************************/
void Sample_R_RIIC_LED_PE_On(uint8_t status)
{
	uint8_t current;

    R_RIIC_rza1h_rsk_read(DEVDRV_CH_3,
    		              RZA1H_RSK_I2C_PX_IO1,
    		              (uint8_t)PORTX_OUTPUT_REG, 0x1, &current);

	if(PX_1_LED1 == (status & PX_1_LED1))
	{
		current = (uint8_t)(current & ~PX_1_LED1);
	}

	if(PX_1_LED2 == (status & PX_1_LED2))
	{
		current = (uint8_t)(current & ~PX_1_LED2);
	}

	if(PX_1_LED3 == (status & PX_1_LED3))
	{
		current = (uint8_t)(current & ~PX_1_LED3);
	}

    R_RIIC_rza1h_rsk_write(DEVDRV_CH_3,
    		               RZA1H_RSK_I2C_PX_IO1,
    		               (uint8_t)PORTX_OUTPUT_REG, 0x1, &current);
}

/******************************************************************************
* Function Name: Sample_R_RIIC_LED_PE_Off
* Description  : LED Off function for expander 1's LED's
* Arguments    : uint8_t status : Active LED's PX_1_LED1, PX_1_LED2, PX_1_LED3
* Return Value : none
******************************************************************************/
void Sample_R_RIIC_LED_PE_Off(uint8_t status)
{
	uint8_t current;

    R_RIIC_rza1h_rsk_read(DEVDRV_CH_3,
    		              RZA1H_RSK_I2C_PX_IO1,
    		              (uint8_t)PORTX_OUTPUT_REG, 0x1, &current);

	if(PX_1_LED1 == (status & PX_1_LED1))
	{
		current = (uint8_t)(current | PX_1_LED1);
	}

	if(PX_1_LED2 == (status & PX_1_LED2))
	{
		current = (uint8_t)(current | PX_1_LED2);
	}

	if(PX_1_LED3 == (status & PX_1_LED3))
	{
		current = (uint8_t)(current | PX_1_LED3);
	}

    R_RIIC_rza1h_rsk_write(DEVDRV_CH_3,
    		               RZA1H_RSK_I2C_PX_IO1,
    		               (uint8_t)PORTX_OUTPUT_REG, 0x1, &current);
}


/******************************************************************************
* Function Name: Sample_RIIC_Ri0_Interrupt
* Description  : Channel 0's receive data full interrupt handler
* Arguments    : uint32_t int_sense : Interrupt detection
*              :                    :   INTC_LEVEL_SENSITIVE : Level sense
*              :                    :   INTC_EDGE_TRIGGER    : Edge trigger
* Return Value : none
******************************************************************************/
void Sample_RIIC_Ri0_Interrupt(uint32_t int_sense)
{
    UNUSED_PARAM(int_sense);

    R_RIIC_RiInterrupt(DEVDRV_CH_0);
}

/******************************************************************************
* Function Name: Sample_RIIC_Ti0_Interrupt
* Description  : Channel 0's transmit data empty interrupt handler
* Arguments    : uint32_t int_sense : Interrupt detection
*              :                    :   INTC_LEVEL_SENSITIVE : Level sense
*              :                    :   INTC_EDGE_TRIGGER    : Edge trigger
* Return Value : none
******************************************************************************/
void Sample_RIIC_Ti0_Interrupt(uint32_t int_sense)
{
    UNUSED_PARAM(int_sense);

    R_RIIC_TiInterrupt(DEVDRV_CH_0);
}

/******************************************************************************
* Function Name: Sample_RIIC_Tei0_Interrupt
* Description  : Channel 0's transmission end interrupt handler
* Arguments    : uint32_t int_sense : Interrupt detection
*              :                    :   INTC_LEVEL_SENSITIVE : Level sense
*              :                    :   INTC_EDGE_TRIGGER    : Edge trigger
* Return Value : none
******************************************************************************/
void Sample_RIIC_Tei0_Interrupt(uint32_t int_sense)
{
    UNUSED_PARAM(int_sense);

    R_RIIC_TeiInterrupt(DEVDRV_CH_0);
}

/******************************************************************************
* Function Name: Sample_RIIC_Ri3_Interrupt
* Description  : Channel 3's receive data full interrupt
* Arguments    : uint32_t int_sense : Interrupt detection
*              :                    :   INTC_LEVEL_SENSITIVE : Level sense
*              :                    :   INTC_EDGE_TRIGGER    : Edge trigger
* Return Value : none
******************************************************************************/
void Sample_RIIC_Ri3_Interrupt(uint32_t int_sense)
{
    UNUSED_PARAM(int_sense);

    R_RIIC_RiInterrupt(DEVDRV_CH_3);
}

/******************************************************************************
* Function Name: Sample_RIIC_Ti3_Interrupt
* Description  : Channel 3's transmit data empty interrupt handler
* Arguments    : uint32_t int_sense : Interrupt detection
*              :                    :   INTC_LEVEL_SENSITIVE : Level sense
*              :                    :   INTC_EDGE_TRIGGER    : Edge trigger
* Return Value : none
******************************************************************************/
void Sample_RIIC_Ti3_Interrupt(uint32_t int_sense)
{
    UNUSED_PARAM(int_sense);

    R_RIIC_TiInterrupt(DEVDRV_CH_3);
}

/******************************************************************************
* Function Name: Sample_RIIC_Tei3_Interrupt
* Description  : Channel 3's transmission end interrupt handler
* Arguments    : uint32_t int_sense : Interrupt detection
*              :                    :   INTC_LEVEL_SENSITIVE : Level sense
*              :                    :   INTC_EDGE_TRIGGER    : Edge trigger
* Return Value : none
******************************************************************************/
void Sample_RIIC_Tei3_Interrupt(uint32_t int_sense)
{
    UNUSED_PARAM(int_sense);

    R_RIIC_TeiInterrupt(DEVDRV_CH_3);
}

/* End of File */

