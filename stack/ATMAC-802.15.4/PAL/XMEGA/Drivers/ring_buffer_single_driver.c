/* This file has been prepared for Doxygen automatic documentation generation.*/
/*! \file *********************************************************************
 *
 * \brief
 *      Single Ring Buffer driver source file.
 *
 *      This file contains the function implementations the Single Ring Buffer
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
//#ifdef RING_BUFFER_SINGLE_DRIVER
#include "ring_buffer_single_driver.h"

/*! \brief Initializes ring buffer.
 *
 *  Initializes ring buffer.
 *
 *  \param Buffer				The Ring_Buffer_Single_t struct instance.
 *  \param pBuffer				Pointer to be Ring Buffer.
 *  \param Size					Buffer size.
 */
bool RING_buffer_single_Initialize(Ring_Buffer_Single_t* buffer, uint8_t* pBuffer, uint8_t size)
{
	if(size == 0)
	{
		return(true);
	}

	buffer->Mask											= size - 1;
	if(buffer->Mask & size)
	{
		return(true);
	}

	buffer->pBuffer											= buffer;
	buffer->Tail											= 0;
	buffer->Head											= 0;

	return(false);
}

/*! \brief Test if there is data in the transmitter buffer 
 *
 *  This function can be used to test if there is free space in the transmitter
 *  buffer
 *
 *  \param buffer				The Ring_Buffer_Single_t struct instance.
 *
 *  \retval true				There is data in the receive 
 *  \retval false				The receive buffer is empty.
 */
bool RING_buffer_single_FreeSpace(Ring_Buffer_Single_t* buffer)
{
	/* Make copies to make sure that volatile access is specified. */
	uint8_t						tempHead					= (buffer->Head + 1) & buffer->Mask;
	uint8_t						tempTail					= buffer->Tail;

	/* There are data left in the buffer unless Head and Tail are equal. */
	return(tempHead != tempTail);
}

/*! \brief Put data.
 *
 *  Stores data byte in TX buffer.
 *
 *  \param buffer				The Ring_Buffer_Single_t struct instance.
 *  \param data					The data to send.
 */
bool RING_buffer_single_PutByte(Ring_Buffer_Single_t* buffer, uint8_t data)
{
	bool						Buffer_FreeSpace			= RING_buffer_single_FreeSpace(buffer);

	if(Buffer_FreeSpace)
	{
		uint8_t					temp_Head					= buffer->Head;
	  	buffer->pBuffer[temp_Head]							= data;

		/* Advance buffer head. */
		buffer->Head										= (temp_Head + 1) & buffer->Mask;
	}

	return(Buffer_FreeSpace);
}

/*! \brief Test if there is data in the receive buffer
 *
 *  This function can be used to test if there is data in the receive buffer
 *  
 *
 *  \param buffer	The Ring_Buffer_Single_t struct instance
 *
 *  \retval true				There is data in the receive 
 *  \retval false				The receive buffer is empty.
 */
bool RING_buffer_single_Available(Ring_Buffer_Single_t* buffer)
{
	/* Make copies to make sure that volatile access is specified. */
	uint8_t						tempHead					= buffer->Head;
	uint8_t						tempTail					= buffer->Tail;

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
 *  \param buffer				The Ring_Buffer_Single_t struct instance.
 *
 *  \return						Received data.
 */
uint8_t RING_buffer_single_GetByte(Ring_Buffer_Single_t* buffer)
{
	uint8_t						ans							= (buffer->pBuffer[buffer->Tail]);

	/* Advance buffer tail. */
	buffer->Tail											= (buffer->Tail + 1) & buffer->Mask;

	return(ans);
}

/*! \brief RX Interrupt Service Routine.
 *
 *  RX Interrupt Service Routine.
 *  Stores received data in RX buffer. This method must be used in ISR routine.
 *
 *  \param buffer				The Ring_Buffer_Single_t struct instance.
 *  \param data					The data to send.
 */
bool RING_buffer_single_RxISR(Ring_Buffer_Single_t* buffer, uint8_t data)
{
	bool						ans;

	/* Advance buffer head. */
	uint8_t						temp_Head					= (buffer->Head + 1) & buffer->Mask;

	/* Check for overflow. */
	uint8_t						temp_Tail					= buffer->Tail;

	if(temp_Head == temp_Tail)
	{
		ans													= false;
	}
	else
	{
		ans													= true;
		
		buffer->pBuffer[buffer->Head]						= data;
		buffer->Head										= temp_Head;
	}

	return(ans);
}

/*! \brief Tx Interrupt Service Routine.
 *
 *  Tx Interrupt Service Routine.
 *  Get one byte from buffer. Argument is pointer to Buffer (Ring_Buffer_Single_t).
 *
 *  \param buffer				The Ring_Buffer_Single_t struct instance.
 *  \param *data				Pointer to get data.
 *
 *  \retval true				The data was copy from transmite buffer to *data.
 *  \retval false				The transmite buffer is empty.
 */
bool RING_buffer_single_TxISR(Ring_Buffer_Single_t* buffer, uint8_t* data)
{
	/* Check if all data is transmitted. */
	uint8_t						temp_Tail					= buffer->Tail;
	bool						ans;

	if(buffer->Head == temp_Tail)
	{
		ans													= false;
	}
	else
	{
		ans													= true;
		*data												= buffer->pBuffer[buffer->Tail];

		/* Advance buffer tail. */
		buffer->Tail										= (buffer->Tail + 1) & buffer->Mask;
	}

	return(ans);
}

#endif /* RING_BUFFER_SINGLE_DRIVER */