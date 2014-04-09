/* @file pal_usb.c
 *
 * @brief PAL USB related functions
 *
 * This file implements SAM4S USB related functions
 * and reception functions.
 *
 * $Id: pal_usb.c 31133 2012-03-05 05:26:08Z mahendran.p $
 *
 * @author    Atmel Corporation: http://www.atmel.com
 * @author    Support email: avr@atmel.com
 */
/*
 * Copyright (c) 2010, Atmel Corporation All rights reserved.
 *
 * Licensed under Atmel's Limited License Agreement --> EULA.txt
 */

#if ((defined USB0))
/* === Includes =========================================================*/
#include <pal_internal.h>
#include "pal_usb.h"
#include <usart.h>

/* === Globals =========================================================*/

/* Usb tempoary Transmission and receive buffers */
static uint8_t usb_0_tx_temp_buf[USB_INT_BUF_MAX_SIZE];
static uint8_t usb_0_rx_temp_buf[USB_INT_BUF_MAX_SIZE];

/* USB communication buffer */
static usb_communication_buffer_t usb_0_buffer;

/* === Prototypes =========================================================*/

/* === Implementation ======================================================= */

/**
 * @brief Initializes USB 0
 *
 * This function initializes the USB0 Channel and the ring buffer parameters.
 *
 */
void sio_usb_0_init(void)
{
    /* Initialize the buffer parameters */
    usb_0_buffer.tx_buf_head = 0;
    usb_0_buffer.tx_buf_tail = 0;
    usb_0_buffer.rx_buf_head = 0;
    usb_0_buffer.rx_buf_tail = 0;
    usb_0_buffer.rx_count = 0;


}

/**
 * @brief Transmit data via USB 0
 *
 * This function transmits data via USB channel 0.
 *
 * @param data Pointer to the buffer where the data to be transmitted is present
 * @param length Number of bytes to be transmitted
 *
 * @return Number of bytes actually transmitted
 */

uint8_t sio_usb_0_tx(uint8_t *data, uint8_t length)
{

    uint8_t bytes_to_be_written = 0;
    uint8_t head = 0;
    uint8_t tail = 0;
    uint8_t size = 0;
    uint8_t back = 0;

    uint8_t tx_length = 0;
    uint8_t i = 0;

    /* Check whether the tranmission is ongoing or not before sending data */
    while (block_tx == true);

    /*
     * Initialize head and tail
     */
    head = usb_0_buffer.tx_buf_head;
    tail = usb_0_buffer.tx_buf_tail;

    size = get_buffer_size( usb_0_buffer.tx_buf_head,
                            usb_0_buffer.tx_buf_tail,
                            USB_TX_BUF_SIZE);
    if (size < length)
    {
        /* Not enough buffer space available. Use the remaining size. */
        bytes_to_be_written = size;
    }
    else
    {
        bytes_to_be_written = length;
    }

    /* Remember the number of bytes transmitted. */
    back = bytes_to_be_written;

    /* The data is copied to the transmit buffer. */
    while (bytes_to_be_written > 0)
    {
        usb_0_buffer.tx_buf[usb_0_buffer.tx_buf_tail] = *data;
        if ((USB_TX_BUF_SIZE - 1) == usb_0_buffer.tx_buf_tail)
        {
            /* Reached the end of buffer, revert back to beginning of buffer. */
            usb_0_buffer.tx_buf_tail = 0;
        }
        else
        {
            /*
             * Increment the index to point the next character to be
             * inserted.
             */
            usb_0_buffer.tx_buf_tail++;
        }

        bytes_to_be_written--;
        data++;
    }

    /*
     * Check whether there is a transmission ongoing. Otherwise write
     * data into the UART data register. Transmission of subsequent
     * bytes / data will be taken care in the ISR.
     */

    /*
     * Prepare a linear buffer for submitting data
    */
    head = usb_0_buffer.tx_buf_head;
    tail = usb_0_buffer.tx_buf_tail;

    while (head != tail)
    {
        usb_0_tx_temp_buf[i] = usb_0_buffer.tx_buf[head];
        /* cannot transmit more than USB_TX_MAX_SIZE at once */
        if (i == USB_TX_BUF_SIZE)
        {
            break;
        }
        if ((head) == (USB_TX_BUF_SIZE - 1))
        {
            head = 0;
        }
        else
        {
            head++;
        }
        i++;
    }

    tx_length = i;

    /* Set the flag to true before writing the data in usb driver*/
    block_tx = true;

    /* Send the data by calling USB driver write and wait for write success */
    while (VCP_WriteUsart(&descp, usb_0_tx_temp_buf, tx_length)
           != -1)  ;
    return back;
}


/**
 * @brief Receives data from USB0
 *
 * This function receives data from USB0
 *
 * @param data pointer to the buffer where the received data will be stored
 * @param max_length maximum length of data to be received
 *
 * @return actual number of bytes received
 */

uint8_t sio_usb_0_rx(uint8_t *data, uint8_t max_length)
{
    uint8_t data_received = 0;
    if (usb_0_buffer.rx_count == 0)
    {
        /* usb receive buffer is empty. */
        return 0;
    }

    ENTER_CRITICAL_REGION();

    if (USB_RX_BUF_SIZE <= usb_0_buffer.rx_count)
    {
        /*
         * Bytes between head and tail are overwritten by new data.
         * The oldest data in buffer is the one to which the tail is
         * pointing. So reading operation should start from the tail.
         */
        usb_0_buffer.rx_buf_head = usb_0_buffer.rx_buf_tail;

        /*
         * This is a buffer overflow case. But still only bytes equivalent to
         * full buffer size are useful.
         */
        usb_0_buffer.rx_count = USB_RX_BUF_SIZE;

        /* Bytes received is more than or equal to buffer. */
        if (USB_RX_BUF_SIZE <= max_length)
        {
            /*
             * Requested receive length (max_length) is more than the
             * max size of receive buffer, but at max the full
             * buffer can be read.
             */
            max_length = USB_RX_BUF_SIZE;
        }
    }
    else
    {
        /* Bytes received is less than receive buffer maximum length. */
        if (max_length > usb_0_buffer.rx_count)
        {
            /*
             * Requested receive length (max_length) is more than the data
             * present in receive buffer. Hence only the number of bytes
             * present in receive buffer are read.
             */
            max_length = usb_0_buffer.rx_count;
        }
    }

    data_received = max_length;

    while (max_length > 0)
    {
        /* Start to copy from head. */
        *data = usb_0_buffer.rx_buf[usb_0_buffer.rx_buf_head];
        usb_0_buffer.rx_buf_head++;
        usb_0_buffer.rx_count--;
        data++;
        max_length--;
        if ((USB_RX_BUF_SIZE) == usb_0_buffer.rx_buf_head)
        {
            usb_0_buffer.rx_buf_head = 0;
        }
    }

    LEAVE_CRITICAL_REGION();
    /* Read the buffer by calling VCP_ReadUsart with the tail buff pointer so
       that it will add from the tail buff*/
    VCP_ReadUsart(&descp, &usb_0_buffer.rx_buf[usb_0_buffer.rx_buf_tail],
                  USB_RX_BUF_SIZE);
    return data_received;
}


/*
* @brief This function is called when the USB transfer from the
* device to the host is finished.
*
* @param pArg     Pointer to callback arguments.
* @param status   USBD status.
* @param transferred number of bytes transferred
* @param remaining number of bytes remaining
*
*/
void usb0_tx_complete_handler(void *pArg , uint8_t   status,
                              uint32_t transferred, uint32_t remaining)
{
    const unsigned int maxPackedSize = MIN(CHIP_USB_ENDPOINTS_MAXPACKETSIZE(RECEIVE_PIPE),
                                           USBEndpointDescriptor_MAXBULKSIZE_FS);
    bool needZeroFrameCompletion = (maxPackedSize == transferred) && (0 == remaining);
    ENTER_CRITICAL_REGION();
    while (transferred--)
    {
        if ((USB_TX_BUF_SIZE - 1) == usb_0_buffer.tx_buf_head)
        {
            /* Reached the end of buffer, revert back to beginning of buffer. */
            usb_0_buffer.tx_buf_head = 0;
        }
        else
        {
            /*
             * * Increment the index to point the next character to be
             * * transmitted.
             * */
            usb_0_buffer.tx_buf_head++;
        }
    }

    if (usb_0_buffer.tx_buf_head != usb_0_buffer.tx_buf_tail)
    {
        /*
         * Prepare a linear buffer for submitting data
         * remaining data in the buffer
         */
        uint8_t tx_length = 0;
        uint8_t i = 0;
        uint8_t head = 0;
        uint8_t tail = 0;
        head = usb_0_buffer.tx_buf_head;
        tail = usb_0_buffer.tx_buf_tail;
        while (head != tail)
        {
            usb_0_tx_temp_buf[i] = usb_0_buffer.tx_buf[head];
            /* cannot transmit more than USB_TX_MAX_SIZE at once */
            if (i == (USB_TX_BUF_SIZE - 1))
            {
                break;
            }
            if ((head) == (USB_TX_BUF_SIZE - 1))
            {
                head = 0;
            }
            else
            {
                head++;
            }
            i++;
        }
        tx_length = i;
        /* Set the flag to true before writing the data in usb driver*/
        block_tx = true;
        /* Send the data by calling USB driver write and wait for success */
        while (VCP_WriteUsart(&descp, usb_0_tx_temp_buf, tx_length) != -1)  ;
    }
    else if ( needZeroFrameCompletion )
    {
        block_tx = true;
        /* Send Zero Length Packet */
        HAL_UsbWrite(TRANSMIT_PIPE, NULL, 0, (TransferCallback_t)usb0_tx_complete_handler, NULL);
    }
    else
    {
        /* No more data for transmission */
        block_tx = false;
    }
    LEAVE_CRITICAL_REGION();
    /* Keep compilar happy*/
    pArg = pArg;
    status = status;
    remaining = remaining;
}


/*
 * @brief This function is called when the USB transfer from the host to the
 * device is finished
 *
 * @param pArg     Pointer to callback arguments.
 * @param status   USBD status.
 * @param transferred number of bytes transferred
 * @param remaining number of bytes remaining
 */

void usb0_rx_complete_handler(void *pArg , uint8_t   status,
                              uint32_t received, uint32_t remaining)
{
    uint8_t tail = usb_0_buffer.rx_buf_tail;
    uint8_t i = 0;
    ENTER_CRITICAL_REGION();
    if (received == 0)
    {
        VCP_ReadUsart(&descp, &usb_0_buffer.rx_buf[usb_0_buffer.rx_buf_tail],
                      USB_RX_BUF_SIZE);
    }
    while (received--)
    {
        /* Count of bytes received through USB 0 channel is incremented. */
        usb_0_buffer.rx_count++;
        usb_0_buffer.rx_buf[tail] = usb_0_rx_temp_buf[i];
        i++;
        if ((USB_RX_BUF_SIZE - 1) == usb_0_buffer.rx_buf_tail)
        {
            /* Revert back to beginning of buffer after reaching end of buffer. */
            usb_0_buffer.rx_buf_tail = 0;
            tail = 0;
        }
        else
        {
            usb_0_buffer.rx_buf_tail++;
            tail++;
        }
    }
    LEAVE_CRITICAL_REGION();
    /* Keep compilar happy*/
    pArg = pArg;
    status = status;
    remaining = remaining;
}
#endif  /* ((defined USB0)) */
/* EOF */
