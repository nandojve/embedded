/* This file has been prepared for Doxygen automatic documentation generation.*/
/*! \file *********************************************************************
 *
 * \brief
 *      Double Ring Buffer driver source file.
 *
 *      This file contains the function implementations the Double Ring Buffer
 *      driver.
 *
 *      For size and/or speed critical code, it is recommended to copy the
 *      function contents directly into your application instead of making
 *      a function call.
 *
 * \par Documentation
 *      For comprehensive code documentation, supported compilers, compiler
 *      settings and supported devices see readme.html
 *
 * \author
 *      Gerson Fernando Budke:  \n
 *      Support email: none
 *
 * Copyright (c) 2011, Gerson Fernando Budke All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice,
 * this list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 * this list of conditions and the following disclaimer in the documentation
 * and/or other materials provided with the distribution.
 *
 * 3. The name of AUTOR may not be used to endorse or promote products derived
 * from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY AUTOR "AS IS" AND ANY EXPRESS OR IMPLIED
 * WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE EXPRESSLY AND
 * SPECIFICALLY DISCLAIMED. IN NO EVENT SHALL AUTOR BE LIABLE FOR ANY DIRECT,
 * INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF
 * THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *****************************************************************************/
#ifdef RING_BUFFER_DOUBLE_DRIVER
#include "ring_buffer_double_driver.h"

/*! \brief Initializes ring buffer.
 *
 *  Initializes receive and transmit buffer.
 *
 *  \param buffer				The Ring_Buffer_Double_t struct instance.
 *  \param pRxBuffer            Pointer to be Ring Buffer of receive.
 *  \param RxSize	            Receive buffer size.
 *  \param pTxBuffer            Pointer to be Ring Buffer of transmitter.
 *  \param TxSize	            Transmitter buffer size.
 */
bool RING_buffer_double_Initialize(Ring_Buffer_Double_t* buffer, uint8_t* pRxBuffer, uint8_t RxSize, uint8_t* pTxBuffer, uint8_t TxSize)
{
	if(RxSize == 0 || TxSize == 0)
	{
		return(false);
	}

	buffer->RxMask											= RxSize - 1;
	if(buffer->RxMask & RxSize)
	{
		return(false);
	}

	buffer->TxMask											= TxSize - 1;
	if(buffer->TxMask & TxSize)
	{
		return(false);
	}

	buffer->pRX												= pRxBuffer;
	buffer->pTX												= pTxBuffer;
	buffer->RX_Tail											= 0;
	buffer->RX_Head											= 0;
	buffer->TX_Tail											= 0;
	buffer->TX_Head											= 0;

	return(true);
}

/*! \brief Test if there is data in the transmitter buffer 
 *
 *  This function can be used to test if there is free space in the transmitter
 *  buffer
 *
 *  \param buffer				The Ring_Buffer_Double_t struct instance.
 *
 *  \retval true				There is data in the receive 
 *  \retval false				The receive buffer is empty.
 */
bool RING_buffer_double_FreeSpace(Ring_Buffer_Double_t* buffer)
{
	/* Make copies to make sure that volatile access is specified. */
	uint8_t						tempHead					= (buffer->TX_Head + 1) & USART_TX_BUFFER_MASK;
	uint8_t						tempTail					= buffer->TX_Tail;

	/* There are data left in the buffer unless Head and Tail are equal. */
	return(tempHead != tempTail);
}

/*! \brief Put data.
 *
 *  Stores data byte in TX buffer.
 *
 *  \param buffer				The Ring_Buffer_Double_t struct instance.
 *  \param data					The data to send.
 */
bool RING_buffer_double_PutByte(Ring_Buffer_Double_t* buffer, uint8_t data)
{
	bool						TXBuffer_FreeSpace			= USART_TXBuffer_FreeSpace(buffer);

	if(TXBuffer_FreeSpace)
	{
		uint8_t					tempTX_Head					= buffer->TX_Head;
	  	buffer->pTX[tempTX_Head]							= data;

		/* Advance buffer head. */
		buffer->TX_Head										= (tempTX_Head + 1) & USART_TX_BUFFER_MASK;
	}

	return(TXBuffer_FreeSpace);
}

/*! \brief Test if there is data in the receive buffer
 *
 *  This function can be used to test if there is data in the receive buffer
 *  
 *
 *  \param buffer	The Ring_Buffer_Double_t struct instance
 *
 *  \retval true				There is data in the receive 
 *  \retval false				The receive buffer is empty.
 */
bool RING_buffer_double_Available(Ring_Buffer_Double_t* buffer)
{
	/* Make copies to make sure that volatile access is specified. */
	uint8_t						tempHead					= buffer->RX_Head;
	uint8_t						tempTail					= buffer->RX_Tail;

	/* There are data left in the buffer unless Head and Tail are equal. */
	return(tempHead != tempTail);
}

/*! \brief Get buffer data.
 *
 *  The function USART_RXBufferData_Available should be used before this
 *  function is used to ensure that data is available.
 *
 *  Returns data from RX buffer
 *
 *  \param buffer				The Ring_Buffer_Double_t struct instance.
 *
 *  \return						Received data.
 */
uint8_t RING_buffer_double_GetByte(Ring_Buffer_Double_t* buffer)
{
	uint8_t						ans							= (buffer->pRX[buffer->RX_Tail]);

	/* Advance buffer tail. */
	buffer->RX_Tail											= (buffer->RX_Tail + 1) & USART_RX_BUFFER_MASK;

	return(ans);
}

/*! \brief RX Interrupt Service Routine.
 *
 *  RX Interrupt Service Routine.
 *  Stores received data in RX buffer. This method must be used in ISR routine.
 *
 *  \param buffer				The Ring_Buffer_Double_t struct instance.
 *  \param data					The data to send.
 */
bool RING_buffer_double_RxISR(Ring_Buffer_Double_t* buffer, uint8_t data)
{
	bool						ans;

	/* Advance buffer head. */
	uint8_t						tempRX_Head					= (buffer->RX_Head + 1) & USART_RX_BUFFER_MASK;

	/* Check for overflow. */
	uint8_t						tempRX_Tail					= buffer->RX_Tail;

	if(tempRX_Head == tempRX_Tail)
	{
		ans													= false;
	}
	else
	{
		ans													= true;
		
		buffer->pRX[buffer->RX_Head]						= data;
		buffer->RX_Head										= tempRX_Head;
	}

	return(ans);
}

/*! \brief Tx Interrupt Service Routine.
 *
 *  Tx Interrupt Service Routine.
 *  Get one byte from buffer. Argument is pointer to Buffer (Ring_Buffer_Double_t).
 *
 *  \param buffer				The Ring_Buffer_Double_t struct instance.
 *  \param *data				Pointer to get data.
 *
 *  \retval true				The data was copy from transmite buffer to *data.
 *  \retval false				The transmite buffer is empty.
 */
bool RING_buffer_double_TxISR(Ring_Buffer_Double_t* buffer, uint8_t* data)
{
	/* Check if all data is transmitted. */
	uint8_t						tempTX_Tail					= buffer->TX_Tail;
	bool						ans;

	if(bufPtr->TX_Head == tempTX_Tail)
	{
		ans													= false;
	}
	else
	{
		ans													= true;
		*data												= buffer->pTX[buffer->TX_Tail];

		/* Advance buffer tail. */
		buffer->TX_Tail										= (buffer->TX_Tail + 1) & USART_TX_BUFFER_MASK;
	}

	return(ans);
}

#endif /* RING_BUFFER_DOUBLE_DRIVER */