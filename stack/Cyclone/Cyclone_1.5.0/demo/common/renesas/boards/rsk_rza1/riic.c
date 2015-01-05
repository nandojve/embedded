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
* File Name     : riic.c
* Device(s)     : RZ/A1H RSK+RZA1H
* H/W Platform  : RSK+RZA1H CPU Board
* Description   : RIIC driver support for all 4 available i2c channels
*******************************************************************************/
/*******************************************************************************
* History       : DD.MM.YYYY Version Description
*               : 18.06.2013 1.00
*******************************************************************************/

/******************************************************************************
Includes   <System Includes> , "Project Includes"
******************************************************************************/
#include "r_typedefs.h"                   /* Default  type definition header */
#include "iodefine.h"                            /* I/O Register root header */
#include "dev_drv.h"                          /* Device Driver common header */
#include "devdrv_riic.h"                               /* RIIC Driver Header */
#include "sample_riic_rza1h_rsk_i2c.h"   /* I2C RSK+RZA1H Eval-board header */
#include "rza_io_regrw.h"            /* Low level register read/write header */

/******************************************************************************
Typedef definitions
******************************************************************************/
typedef enum riic_rx_mode
{
    RIIC_RX_MODE_NORMAL,
    RIIC_RX_MODE_ACK,
    RIIC_RX_MODE_LOW_HOLD,
    RIIC_RX_MODE_NACK,
    RIIC_RX_MODE_STOP
} riic_rx_mode_t;

/******************************************************************************
Macro definitions
******************************************************************************/

/******************************************************************************
Imported global variables and functions (from other files)
******************************************************************************/

/******************************************************************************
Exported global variables and functions (to be accessed by other files)
******************************************************************************/

/******************************************************************************
Private global variables and functions
******************************************************************************/
static void    RIIC_Receive(uint32_t channel, riic_rx_mode_t mode, uint8_t * data);
static int32_t RIIC_Transmit(uint32_t channel, uint8_t data);
static void    RIIC_TransmitStart(uint32_t channel);
static void    RIIC_TransmitRestart(uint32_t channel);
static void    RIIC_TransmitStop(uint32_t channel);
static void    RIIC_DetectStop(uint32_t channel);
static void    RIIC_ClearNack(volatile struct st_riic * riic);
volatile struct st_riic * RIIC_GetRegAddr(uint32_t channel);

/******************************************************************************
* Function Name: R_RIIC_Init
*              : Calls the relevant I2C channel initialization function
*              : Uses the following interrupts:
*              : Receive data full interrupt
*              : Transmit data empty interrupt
*              : Transmission complete interrupt
* Arguments    : uint32_t channel : I2c channel selection
*              : uint32_t cks     : I2c Clock Selection
*              :                  : (Settings in the CKS bit of RIICnMR1 register)
*              :                  :   RIIC_CKS_DIVISION_1   : P0/1 clock
*              :                  :   RIIC_CKS_DIVISION_2   : P0/2 clock
*              :                  :   RIIC_CKS_DIVISION_4   : P0/4 clock
*              :                  :   RIIC_CKS_DIVISION_8   : P0/8 clock
*              :                  :   RIIC_CKS_DIVISION_16  : P0/16 clock
*              :                  :   RIIC_CKS_DIVISION_32  : P0/32 clock
*              :                  :   RIIC_CKS_DIVISION_64  : P0/64 clock
*              :                  :   RIIC_CKS_DIVISION_128 : P0/128 clock
*              : uint32_t brl     : Lower byte value of the width of the SCL clock
*              : uint32_t brh     : High byte value of the width of the SCL clock
* Return Value : DEVDRV_SUCCESS   : Success of RIIC initialisation
*              : DEVDRV_ERROR     : Failure of RIIC initialisation
******************************************************************************/
int32_t R_RIIC_Init(uint32_t channel, uint32_t cks, uint32_t brl, uint32_t brh)
{
    /*  Argument check  */
    if ((channel >= RIIC_CH_TOTAL) || (cks > 7) || (brl > 31) || (brh > 31))
    {
        return DEVDRV_ERROR;        /* Argument error */
    }

    /*  RIIC initialization  */
    switch (channel)
    {
        case DEVDRV_CH_0:
            Userdef_RIIC0_Init(cks, brl, brh);
        break;
        case DEVDRV_CH_1:
            Userdef_RIIC1_Init(cks, brl, brh);
        break;
        case DEVDRV_CH_2:
            Userdef_RIIC2_Init(cks, brl, brh);
        break;
        case DEVDRV_CH_3:
            Userdef_RIIC3_Init(cks, brl, brh);
        break;
        default:
            /* Do not reach here based on the assumption */
        break;
    }

    return DEVDRV_SUCCESS;
}

/******************************************************************************
* Function Name: R_RIIC_SingleRead
* Description  : Reads 1 byte only of data from specified I2c channel
* Arguments    : uint32_t channel : I2c channel selection
*              : uint8_t * data   : Pointer to allocated storage (1 byte)
* Return Value : DEVDRV_SUCCESS   : Success of RIIC operation
*              : DEVDRV_ERROR     : Failure of RIIC operation
******************************************************************************/
int32_t R_RIIC_SingleRead(uint32_t channel, uint8_t * data)
{
    /* Ensure channel is valid */
    if (channel >= RIIC_CH_TOTAL)
    {
        return DEVDRV_ERROR;
    }

    /* Read single byte */
    RIIC_Receive(channel, RIIC_RX_MODE_NORMAL, data);

    return DEVDRV_SUCCESS;
}

/******************************************************************************
* End of Function R_RIIC_SingleRead
******************************************************************************/

/******************************************************************************
* Function Name: R_RIIC_SingleWrite
* Description  : 
* Description  : Writes 1 byte only of data to specified I2c channel
* Arguments    : uint32_t channel : I2c channel selection
*              : uint8_t * data   : Pointer to allocated storage (1 byte)
*              : mode             : RIIC_TEND_WAIT_TRANSMIT : Transmission Mode
*              :                  : RIIC_TEND_WAIT_RECEIVE  : Receive Mode
* Return Value : DEVDRV_SUCCESS   : Success of RIIC operation
*              : DEVDRV_ERROR     : Failure of RIIC operation
******************************************************************************/
int32_t R_RIIC_SingleWrite(uint32_t channel, uint8_t data, uint32_t mode)
{
    volatile struct st_riic * riic;
    int32_t ret;

    /* Ensure channel is valid */
    if (channel >= RIIC_CH_TOTAL)
    {
        return DEVDRV_ERROR;
    }

    /* Write single byte */
    ret = RIIC_Transmit(channel, data);

    if (DEVDRV_SUCCESS == ret)
    {
        switch (channel)
        {
            case DEVDRV_CH_0:
                Userdef_RIIC0_WaitTransmitEnd(mode);
            break;
            case DEVDRV_CH_1:
                Userdef_RIIC1_WaitTransmitEnd(mode);
            break;
            case DEVDRV_CH_2:
                Userdef_RIIC2_WaitTransmitEnd(mode);
            break;
            case DEVDRV_CH_3:
                Userdef_RIIC3_WaitTransmitEnd(mode);
            break;
            default:
                /* Can not be reached due to parameter checking above */
            break;
        }

        /* Check for transmission status */
        riic = RIIC_GetRegAddr(channel);
        if (0 == RZA_IO_RegRead_8(&(riic->RIICnSR2.UINT8[0]),
        		                    RIICn_RIICnSR2_NACKF_SHIFT,
        		                    RIICn_RIICnSR2_NACKF))
        {
            ret = DEVDRV_SUCCESS;  /* ACK */
        }
        else
        {
            ret = DEVDRV_ERROR;    /* NACK */
        }
    }

    return ret;
}

/******************************************************************************
* End of Function R_RIIC_SingleWrite
******************************************************************************/

/******************************************************************************
* Function Name: R_RIIC_SendCond
* Description  : Send specified condition on selected channel
* Arguments    : uint32_t channel : I2c channel selection
*              : uint32_t mode    : Three condition types are available
*              :                  : RIIC_TX_MODE_START   : Start Condition
*              :                  : RIIC_TX_MODE_RESTART : Restart Condition
*              :                  : RIIC_TX_MODE_STOP    : Stop Condition
* Return Value : DEVDRV_SUCCESS   : Success of RIIC operation
*              : DEVDRV_ERROR     : Failure of RIIC operation
******************************************************************************/
int32_t R_RIIC_SendCond(uint32_t channel, uint32_t mode)
{
    /* Argument check */
    if (channel >= RIIC_CH_TOTAL || mode > RIIC_TX_MODE_STOP)
    {
        return DEVDRV_ERROR;        /* Argument error */
    }

    /* Mode check */
    switch (mode)
    {
        case RIIC_TX_MODE_START:
            RIIC_TransmitStart(channel);
        break;
        case RIIC_TX_MODE_RESTART:
            RIIC_TransmitRestart(channel);
        break;
        case RIIC_TX_MODE_STOP:
            RIIC_TransmitStop(channel);
        break;
        default:
            /* Do not reach here based on the assumption */
        break;
    }

    return DEVDRV_SUCCESS;
}
/******************************************************************************
* End of Function R_RIIC_WriteCond
******************************************************************************/

/******************************************************************************
* Function Name: R_RIIC_Write
* Description  : Send specified condition on selected channel
* Arguments    : uint32_t channel : I2c channel selection
*              : uint8_t *buffer  : pointer to data for writing
*              : uint32_t byte    : number of bytes to transmit
* Return Value : DEVDRV_SUCCESS   : Success of RIIC operation
*              : DEVDRV_ERROR     : Failure of RIIC operation
******************************************************************************/
int32_t R_RIIC_Write(uint32_t channel, const uint8_t * buffer, uint32_t byte)
{
    volatile struct st_riic * riic;
    uint32_t offset;
    int32_t  ret;

    /* Ensure channel is valid (parameter checking) */
    if (channel >= RIIC_CH_TOTAL)
    {
        return DEVDRV_ERROR;        /* Argument error */
    }

    if (0 == byte)
    {
        return DEVDRV_SUCCESS;
    }

    ret = DEVDRV_SUCCESS;

    for (offset = 0; offset < byte; offset++)
    {
        /* Single byte transmission */
        ret = RIIC_Transmit(channel, *(buffer + offset));

        if (ret != DEVDRV_SUCCESS)
        {
            break;
        }
    }

    if (DEVDRV_SUCCESS == ret)
    {
        switch (channel)
        {
            case DEVDRV_CH_0:
                Userdef_RIIC0_WaitTransmitEnd(RIIC_TEND_WAIT_TRANSMIT);
                Userdef_RIIC0_InitTransmitEnd();
            break;
            case DEVDRV_CH_1:
                Userdef_RIIC1_WaitTransmitEnd(RIIC_TEND_WAIT_TRANSMIT);
                Userdef_RIIC1_InitTransmitEnd();
            break;
            case DEVDRV_CH_2:
                Userdef_RIIC2_WaitTransmitEnd(RIIC_TEND_WAIT_TRANSMIT);
                Userdef_RIIC2_InitTransmitEnd();
            break;
            case DEVDRV_CH_3:
                Userdef_RIIC3_WaitTransmitEnd(RIIC_TEND_WAIT_TRANSMIT);
                Userdef_RIIC3_InitTransmitEnd();
            break;
            default:
                /* Do not reach here based on the assumption */
            break;
        }

        /* Check for transmission status */
        riic = RIIC_GetRegAddr(channel);
        /* ACK */
        if (0 == RZA_IO_RegRead_8(&(riic->RIICnSR2.UINT8[0]),
        		                    RIICn_RIICnSR2_NACKF_SHIFT,
        		                    RIICn_RIICnSR2_NACKF))
        {
            ret = DEVDRV_SUCCESS;
        }
        /* NACK */
        else
        {
            ret = DEVDRV_ERROR;
        }
    }

    return ret;
}
/******************************************************************************
* End of Function R_RIIC_Write
******************************************************************************/

/******************************************************************************
* Function Name: R_RIIC_Read
* Description  : Send specified condition on selected channel
* Arguments    : uint32_t channel : I2c channel selection
*              : uint8_t *buffer  : pointer to allocated space for data
*              : uint32_t byte    : number of bytes to receive
* Return Value : DEVDRV_SUCCESS   : Success of RIIC operation
*              : DEVDRV_ERROR     : Failure of RIIC operation
******************************************************************************/
int32_t R_RIIC_Read(uint32_t channel, uint8_t * buffer, uint32_t byte)
{
    volatile struct st_riic * riic;
    uint32_t  cnt;
    uint8_t * buffer_ptr;

    /* Ensure channel is valid (parameter checking) */
    if (channel >= RIIC_CH_TOTAL)
    {
        return DEVDRV_ERROR;        /* Argument error */
    }

    /* Ensure at least 1 byte is specified to receive */
    if (0 == byte)
    {
        return DEVDRV_SUCCESS;
    }

    buffer_ptr = buffer;

    if (1 == byte)
    {
        /* (In the case of only 1 byte) reception start */
        /* Do Nothing */
    }

    else if (2 == byte)
    {
        /* (In the case of 2 bytes) reception start */
        /* Complete first byte read*/
        RIIC_Receive(channel, RIIC_RX_MODE_NACK, buffer_ptr);
        buffer_ptr++;
    }

    else
    {
        /* (In the case of 3 bytes or more) reception start */
        cnt = byte;
        while (cnt > 3)
        {
            /* Data lead of the first bytes to the last byte (-3) */
            RIIC_Receive(channel, RIIC_RX_MODE_NORMAL, buffer_ptr);
            buffer_ptr++;
            cnt--;
        }

        /* Data read of the last byte (-2) */
        RIIC_Receive(channel, RIIC_RX_MODE_LOW_HOLD, buffer_ptr);
        buffer_ptr++;

        /* Data read of the last byte (-1) */
        RIIC_Receive(channel, RIIC_RX_MODE_NACK, buffer_ptr);
        buffer_ptr++;
    }

    /* Last byte read */
    RIIC_Receive(channel, RIIC_RX_MODE_STOP, buffer_ptr);


    riic = RIIC_GetRegAddr(channel);

    if( riic != (struct st_riic *)DEVDRV_ERROR)
    {
        /* Process next Read */
        RZA_IO_RegWrite_8(&(riic->RIICnMR3.UINT8[0]),
        		            0,
        		            RIICn_RIICnMR3_WAIT_SHIFT,
        		            RIICn_RIICnMR3_WAIT);

        /* Stop condition detection confirmation */
        RIIC_DetectStop(channel);
    }
    return DEVDRV_SUCCESS;
}
/******************************************************************************
* End of Function R_RIIC_Read
******************************************************************************/

/******************************************************************************
* Function Name: R_RIIC_ReadDummy
* Description  : Perform dummy read on selected channel
* Arguments    : uint32_t channel : I2c channel selection
* Return Value : DEVDRV_SUCCESS   : Success of RIIC operation
*              : DEVDRV_ERROR     : Failure of RIIC operation
******************************************************************************/
int32_t R_RIIC_ReadDummy(uint32_t channel)
{
    uint8_t dummy_buf_8b;

    /* Argument check */
    if (channel >= RIIC_CH_TOTAL)
    {
        return DEVDRV_ERROR;        /* Argument error */
    }

    /* Perform Dummy byte read */
    RIIC_Receive(channel, RIIC_RX_MODE_NORMAL, &dummy_buf_8b);

    return DEVDRV_SUCCESS;
}
/******************************************************************************
* End of Function R_RIIC_ReadDummy
******************************************************************************/

/******************************************************************************
* Function Name: R_RIIC_DetectStop
* Description  : Detect Stop Condition on selected channel
* Arguments    : uint32_t channel : I2c channel selection
* Return Value : DEVDRV_SUCCESS   : Success of RIIC operation
*              : DEVDRV_ERROR     : Failure of RIIC operation
******************************************************************************/
int32_t R_RIIC_DetectStop(uint32_t channel)
{
    /* Argument check */
    if (channel >= RIIC_CH_TOTAL)
    {
        return DEVDRV_ERROR;        /* Argument error */
    }

    /* Perform detection */
    RIIC_DetectStop(channel);

    return DEVDRV_SUCCESS;
}
/******************************************************************************
* End of Function R_RIIC_DetectStop
******************************************************************************/

/******************************************************************************
* Function Name: R_RIIC_ClearNack
* Description  : Clear NACK bit on selected channel
* Arguments    : uint32_t channel : I2c channel selection
* Return Value : DEVDRV_SUCCESS   : Success of RIIC operation
*              : DEVDRV_ERROR     : Failure of RIIC operation
******************************************************************************/
int32_t R_RIIC_ClearNack(uint32_t channel)
{
    volatile struct st_riic * riic;

    /*  Argument check  */
    if (channel >= RIIC_CH_TOTAL)
    {
        return DEVDRV_ERROR;
    }

    /* NACK clear flag */
    riic = RIIC_GetRegAddr(channel);
    RIIC_ClearNack(riic);

    return DEVDRV_SUCCESS;
}

/******************************************************************************
* Function Name: R_RIIC_RiInterrupt
* Description  : Receive data full interrupt notification for RIIC of channel
*              : specified in the channel argument.
*              : Receive data full by a user-defined function
*              : Userdef_RIICn_SetReceiveFull.
* Arguments    : uint32_t channel: RIIC channel (0 to 3)
* Return Value : DEVDRV_SUCCESS   : Function completes with no errors
*              : DEVDRV_ERROR     : Function fails to complete with errors
******************************************************************************/
int32_t R_RIIC_RiInterrupt(uint32_t channel)
{
    /*  Argument check  */
    if (channel >= RIIC_CH_TOTAL)
    {
        return DEVDRV_ERROR;
    }

    /* Receive data full set of conditions */
    switch (channel)
    {
        case DEVDRV_CH_0:
            Userdef_RIIC0_SetReceiveFull();
        break;
        case DEVDRV_CH_1:
            Userdef_RIIC1_SetReceiveFull();
        break;
        case DEVDRV_CH_2:
            Userdef_RIIC2_SetReceiveFull();
        break;
        case DEVDRV_CH_3:
            Userdef_RIIC3_SetReceiveFull();
        break;
        default:
            /* Do not reach here based on the assumption */
        break;
    }

    return DEVDRV_SUCCESS;
}

/******************************************************************************
* Function Name: R_RIIC_TiInterrupt
* Description  : Transmit data empty interrupt notification for RIIC of channel
*              : specified in the channel argument.
*              : Send data by user-defined function
*              : Userdef_RIICn_SetTransmitEmpty
* Arguments    : uint32_t channel: RIIC channel (0 to 3)
* Return Value : DEVDRV_SUCCESS   : Function completes with no errors
*              : DEVDRV_ERROR     : Function fails to complete with errors
******************************************************************************/
int32_t R_RIIC_TiInterrupt(uint32_t channel)
{
    /*  Argument check  */
    if (channel >= RIIC_CH_TOTAL)
    {
        return DEVDRV_ERROR;        /* Argument error */
    }

    /* Transmit-data-empty set of conditions */
    switch (channel)
    {
        case DEVDRV_CH_0:
            Userdef_RIIC0_SetTransmitEmpty();
        break;
        case DEVDRV_CH_1:
            Userdef_RIIC1_SetTransmitEmpty();
        break;
        case DEVDRV_CH_2:
            Userdef_RIIC2_SetTransmitEmpty();
        break;
        case DEVDRV_CH_3:
            Userdef_RIIC3_SetTransmitEmpty();
        break;
        default:
            /* Do not reach here based on the assumption */
        break;
    }

    return DEVDRV_SUCCESS;
}

/******************************************************************************
* Function Name: R_RIIC_TeiInterrupt
* Description  : Transmission end interrupt notification for RIIC of channel
*              : specified in the channel argument.
*              : Send notice of termination by user-defined function
*              : Userdef_RIICn_SetTransmitEnd
* Arguments    : uint32_t channel: RIIC channel (0 to 3)
* Return Value : DEVDRV_SUCCESS   : Function completes with no errors
*              : DEVDRV_ERROR     : Function fails to complete with errors
******************************************************************************/
int32_t R_RIIC_TeiInterrupt(uint32_t channel)
{
    volatile struct  st_riic * riic;
    volatile uint8_t dummy_buf_8b = 0u;

    UNUSED_VARIABLE(dummy_buf_8b);

    /*  Argument check  */
    if (channel >= RIIC_CH_TOTAL)
    {
        return DEVDRV_ERROR;
    }

    switch (channel)
    {
        case DEVDRV_CH_0:
            Userdef_RIIC0_SetTransmitEnd();
        break;
        case DEVDRV_CH_1:
            Userdef_RIIC1_SetTransmitEnd();
        break;
        case DEVDRV_CH_2:
            Userdef_RIIC2_SetTransmitEnd();
        break;
        case DEVDRV_CH_3:
            Userdef_RIIC3_SetTransmitEnd();
        break;
        default:
            /* Do not reach here based on the assumption */
        break;
    }

    /* Transmit end flag clear */
    riic = RIIC_GetRegAddr(channel);
    RZA_IO_RegWrite_8(&(riic->RIICnSR2.UINT8[0]),
    		            0,
    		            RIICn_RIICnSR2_TEND_SHIFT,
    		            RIICn_RIICnSR2_TEND);

    dummy_buf_8b = RZA_IO_RegRead_8(&(riic->RIICnSR2.UINT8[0]),
    		                          RIICn_RIICnSR2_TEND_SHIFT,
    		                          RIICn_RIICnSR2_TEND);

    return DEVDRV_SUCCESS;
}

/******************************************************************************
* Function Name: RIIC_Receive
* Description  : Receive data (single byte)
* Arguments    : uint32_t channel    : RIIC channel (0 to 3)
*              : riic_rx_mode_t mode : receive mode
*              :                     : RIIC_RX_MODE_NORMAL: Normal mode
*              :                     : RIIC_RX_MODE_ACK: ACK mode
*              :                     : RIIC_RX_MODE_LOW_HOLD: Low hold mode
*              :                     : RIIC_RX_MODE_NACK: NACK mode
*              :                     : RIIC_RX_MODE_STOP: Stop condition mode
*              : uint8_t * data      : received data
* Return Value : none
******************************************************************************/
static void RIIC_Receive(uint32_t channel, riic_rx_mode_t mode, uint8_t * data)
{
    volatile struct st_riic * riic;

    /* Wait until Receive data full conditions and clear receive-data-full
       condition have been satisfied */
    switch (channel)
    {
        case DEVDRV_CH_0:
            Userdef_RIIC0_WaitReceiveFull();
            Userdef_RIIC0_InitReceiveFull();
        break;
        case DEVDRV_CH_1:
            Userdef_RIIC1_WaitReceiveFull();
            Userdef_RIIC1_InitReceiveFull();
        break;
        case DEVDRV_CH_2:
            Userdef_RIIC2_WaitReceiveFull();
            Userdef_RIIC2_InitReceiveFull();
        break;
        case DEVDRV_CH_3:
            Userdef_RIIC3_WaitReceiveFull();
            Userdef_RIIC3_InitReceiveFull();
        break;
        default:
            /* Do not reach here based on the assumption */
        break;
    }

    riic = RIIC_GetRegAddr(channel);

    switch (mode)
    {
        case RIIC_RX_MODE_ACK:
        	/* Enable Modification of the ACKBT bit */
            RZA_IO_RegWrite_8(&(riic->RIICnMR3.UINT8[0]),
            		            1,
            		            RIICn_RIICnMR3_ACKWP_SHIFT,
            		            RIICn_RIICnMR3_ACKWP);

            /* Send '0' acknowledge bit NACK Transmission */
            RZA_IO_RegWrite_8(&(riic->RIICnMR3.UINT8[0]),
                                0,
                                RIICn_RIICnMR3_ACKBT_SHIFT,
                                RIICn_RIICnMR3_ACKBT);
        break;

        case RIIC_RX_MODE_LOW_HOLD:
        	/* Period between ninth clock cycle and first clock cycle is held low. */
            RZA_IO_RegWrite_8(&(riic->RIICnMR3.UINT8[0]),
            		            1,
            		            RIICn_RIICnMR3_WAIT_SHIFT,
            		            RIICn_RIICnMR3_WAIT);
        break;

        case RIIC_RX_MODE_NACK:
            RZA_IO_RegWrite_8(&(riic->RIICnMR3.UINT8[0]),
            		            1,
            		            RIICn_RIICnMR3_ACKWP_SHIFT,
            		            RIICn_RIICnMR3_ACKWP);

            /* Send '1' acknowledge bit NACK Transmission */
            RZA_IO_RegWrite_8(&(riic->RIICnMR3.UINT8[0]),
            		            1,
            		            RIICn_RIICnMR3_ACKBT_SHIFT,
            		            RIICn_RIICnMR3_ACKBT);
        break;

        case RIIC_RX_MODE_STOP:
        	/* Stop condition request */
            RIIC_TransmitStop(channel);
        break;

        case RIIC_RX_MODE_NORMAL:
            /* Do Nothing */
        break;

        default:
            /* Do not reach here based on the assumption */
        break;
    }

    /* Read data from wire */
    *data = riic->RIICnDRR.UINT8[0];
}

/******************************************************************************
* Function Name: RIIC_Transmit
* Description  : Transmit data (single byte)
* Arguments    : uint32_t channel : RIIC channel (0 to 3)
*              : uint8_t * data   : received data
* Return Value : DEVDRV_SUCCESS   : Function completes with no errors
*              : DEVDRV_ERROR     : Function fails to complete with errors
******************************************************************************/
static int32_t RIIC_Transmit(uint32_t channel, uint8_t data)
{
    volatile struct st_riic * riic;
    int32_t ret;

    riic = RIIC_GetRegAddr(channel);

    /* ACK */
    if (0 == RZA_IO_RegRead_8(&(riic->RIICnSR2.UINT8[0]),
    		                    RIICn_RIICnSR2_NACKF_SHIFT,
    		                    RIICn_RIICnSR2_NACKF))
    {
        ret = DEVDRV_SUCCESS;
    }
    /* NACK */
    else
    {
        ret = DEVDRV_ERROR;
    }

    if (DEVDRV_SUCCESS == ret)
    {
        switch (channel)
        {
            case DEVDRV_CH_0:
                Userdef_RIIC0_WaitTransmitEmpty();
                Userdef_RIIC0_InitTransmitEmpty();
            break;
            case DEVDRV_CH_1:
                Userdef_RIIC1_WaitTransmitEmpty();
                Userdef_RIIC1_InitTransmitEmpty();
            break;
            case DEVDRV_CH_2:
                Userdef_RIIC2_WaitTransmitEmpty();
                Userdef_RIIC2_InitTransmitEmpty();
            break;
            case DEVDRV_CH_3:
                Userdef_RIIC3_WaitTransmitEmpty();
                Userdef_RIIC3_InitTransmitEmpty();
            break;
            default:
                /* Do not reach here based on the assumption */
            break;
        }

        /* Write data to wire */
        riic->RIICnDRT.UINT8[0] = data;
    }

    return ret;
}

/******************************************************************************
* Function Name: RIIC_TransmitStart
* Description  : Make request to issue start condition fore selected channel
*              : Perform the following sequence of actions
*              : Wait till bus is free
*              : Clear Receive data full flag
*              : Clear transmit end flag
*              : Clear transmit-data-empty flag
*              : Requests to issue a start condition
* Arguments    : uint32_t channel : RIIC channel (0 to 3)
* Return Value : none
******************************************************************************/
static void RIIC_TransmitStart(uint32_t channel)
{
    volatile struct st_riic * riic;


    switch (channel)
    {
        case DEVDRV_CH_0:
            Userdef_RIIC0_WaitBusMastership(RIIC_BUS_MASTERSHIP_WAIT_FREE);

            Userdef_RIIC0_InitReceiveFull();
            Userdef_RIIC0_InitTransmitEmpty();
            Userdef_RIIC0_InitTransmitEnd();
        break;
        case DEVDRV_CH_1:
            Userdef_RIIC1_WaitBusMastership(RIIC_BUS_MASTERSHIP_WAIT_FREE);

            Userdef_RIIC1_InitReceiveFull();
            Userdef_RIIC1_InitTransmitEmpty();
            Userdef_RIIC1_InitTransmitEnd();
        break;
        case DEVDRV_CH_2:
            Userdef_RIIC2_WaitBusMastership(RIIC_BUS_MASTERSHIP_WAIT_FREE);
            Userdef_RIIC2_InitReceiveFull();
            Userdef_RIIC2_InitTransmitEmpty();
            Userdef_RIIC2_InitTransmitEnd();
        break;
        case DEVDRV_CH_3:
            Userdef_RIIC3_WaitBusMastership(RIIC_BUS_MASTERSHIP_WAIT_FREE);
            Userdef_RIIC3_InitReceiveFull();
            Userdef_RIIC3_InitTransmitEmpty();
            Userdef_RIIC3_InitTransmitEnd();
        break;
        default:
            /* Do not reach here based on the assumption */
        break;
    }

    riic = RIIC_GetRegAddr(channel);
    RZA_IO_RegWrite_8(&(riic->RIICnCR2.UINT8[0]),
    		            1,
    		            RIICn_RIICnCR2_ST_SHIFT,
    		            RIICn_RIICnCR2_ST);
}

/******************************************************************************
* Function Name: RIIC_TransmitRestart
* Description  : Make the request to issue a restart condition.
*              : Perform the following sequence of actions
*              : Wait till bus is busy
*              : Clear Receive data full flag
*              : Clear transmit end flag
*              : Clear transmit-data-empty flag
*              : Requests to issue a restart condition
* Arguments    : uint32_t channel : RIIC channel (0 to 3)
* Return Value : none
******************************************************************************/
static void RIIC_TransmitRestart(uint32_t channel)
{
    volatile struct st_riic * riic;

    switch (channel)
    {
        case DEVDRV_CH_0:
            Userdef_RIIC0_WaitBusMastership(RIIC_BUS_MASTERSHIP_WAIT_BUSY);

            Userdef_RIIC0_InitReceiveFull();
            Userdef_RIIC0_InitTransmitEmpty();
            Userdef_RIIC0_InitTransmitEnd();
        break;
        case DEVDRV_CH_1:
            Userdef_RIIC1_WaitBusMastership(RIIC_BUS_MASTERSHIP_WAIT_BUSY);

            Userdef_RIIC1_InitReceiveFull();
            Userdef_RIIC1_InitTransmitEmpty();
            Userdef_RIIC1_InitTransmitEnd();
        break;
        case DEVDRV_CH_2:
            Userdef_RIIC2_WaitBusMastership(RIIC_BUS_MASTERSHIP_WAIT_BUSY);

            Userdef_RIIC2_InitReceiveFull();
            Userdef_RIIC2_InitTransmitEmpty();
            Userdef_RIIC2_InitTransmitEnd();
        break;
        case DEVDRV_CH_3:
            Userdef_RIIC3_WaitBusMastership(RIIC_BUS_MASTERSHIP_WAIT_BUSY);

            Userdef_RIIC3_InitReceiveFull();
            Userdef_RIIC3_InitTransmitEmpty();
            Userdef_RIIC3_InitTransmitEnd();
        break;
        default:
            /* Do not reach here based on the assumption */
        break;
    }

    riic = RIIC_GetRegAddr(channel);
    RZA_IO_RegWrite_8(&(riic->RIICnCR2.UINT8[0]),
    		            1,
    		            RIICn_RIICnCR2_RS_SHIFT,
    		            RIICn_RIICnCR2_RS);
}

/******************************************************************************
* Function Name: RIIC_TransmitStop
* Description  : Make the request to issue a restart condition.
*              : Perform the following sequence of actions
*              : Clear Receive data full flag
*              : Requests to issue a stop condition
* Arguments    : uint32_t channel : RIIC channel (0 to 3)
* Return Value : none
******************************************************************************/
static void RIIC_TransmitStop(uint32_t channel)
{
    volatile struct st_riic * riic;

    riic = RIIC_GetRegAddr(channel);

    /* Stop condition detection flag initialisation */
    RZA_IO_RegWrite_8(&(riic->RIICnSR2.UINT8[0]),
    		            0,
    		            RIICn_RIICnSR2_STOP_SHIFT,
    		            RIICn_RIICnSR2_STOP);

    /* Stop condition issuance request */
    RZA_IO_RegWrite_8(&(riic->RIICnCR2.UINT8[0]),
    		            1,
    		            RIICn_RIICnCR2_SP_SHIFT,
    		            RIICn_RIICnCR2_SP);
}

/******************************************************************************
* Function Name: RIIC_DetectStop
* Description  : Make the request to detect stop.
*              : Perform the following sequence of actions
*              : Detect stop condition
*              : Clear NACK
*              : Clear stop condition
* Arguments    : uint32_t channel : RIIC channel (0 to 3)
* Return Value : none
******************************************************************************/
static void RIIC_DetectStop(uint32_t channel)
{
    volatile struct st_riic * riic;

    switch (channel)
    {
        case DEVDRV_CH_0:
            Userdef_RIIC0_WaitStop();
        break;
        case DEVDRV_CH_1:
            Userdef_RIIC1_WaitStop();
        break;
        case DEVDRV_CH_2:
            Userdef_RIIC2_WaitStop();
        break;
        case DEVDRV_CH_3:
            Userdef_RIIC3_WaitStop();
        break;
        default:
            /* Do not reach here based on the assumption */
        break;
    }

    riic = RIIC_GetRegAddr(channel);

    RIIC_ClearNack(riic);
    RZA_IO_RegWrite_8(&(riic->RIICnSR2.UINT8[0]),
    		           0,
    		           RIICn_RIICnSR2_STOP_SHIFT,
    		           RIICn_RIICnSR2_STOP);
}

/******************************************************************************
* Function Name: RIIC_ClearNack
* Description  : Clear NACK detection flag of RIIC of channel that are
*              : specified in the argument channel (NACKF)
* Arguments    : struct st_riic * riic : specified RIIC channel register
* Return Value : none
******************************************************************************/
static void RIIC_ClearNack(volatile struct st_riic * riic)
{
    RZA_IO_RegWrite_8(&(riic->RIICnSR2.UINT8[0]),
    		            0,
    		            RIICn_RIICnSR2_NACKF_SHIFT,
    		            RIICn_RIICnSR2_NACKF);
}

/******************************************************************************
* Function Name: RIIC_GetRegAddr
* Description  : Convert channel ID into pointer to RIIC channel register
* Arguments    : uint32_t channel : RIIC channel (0 to 3)
* Return Value : struct st_riic * : Start address of RIIC register by channel
******************************************************************************/
volatile struct st_riic * RIIC_GetRegAddr(uint32_t channel)
{
    volatile struct st_riic * riic;

    switch (channel)
    {
        case DEVDRV_CH_0:
            riic = &RIIC0;
        break;
        case DEVDRV_CH_1:
            riic = &RIIC1;
        break;
        case DEVDRV_CH_2:
            riic = &RIIC2;
        break;
        case DEVDRV_CH_3:
            riic = &RIIC3;
        break;
        default:
            /* Do not reach here based on the assumption */
            riic = &RIIC0;
        break;
    }

    return riic;
}


/* End of File */
