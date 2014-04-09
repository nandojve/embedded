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
#ifndef RING_BUFFER_SINGLE_DRIVER_H
#define RING_BUFFER_SINGLE_H

#include "avr_compiler.h"

/* \brief Generic assincronous single ring buffer. */
typedef struct Ring_Buffer_Single
{
	/* \brief Buffer pointer. */
	volatile uint8_t* pBuffer;
	
	/* \brief Buffer head. */
	volatile uint8_t Head;
	/* \brief Buffer tail. */
	volatile uint8_t Tail;
	
	/* \brief Buffer mask. */
	volatile uint8_t Mask;
} Ring_Buffer_Single_t;

bool	RING_buffer_single_Initialize(Ring_Buffer_Single_t*	buffer, uint8_t* pBuffer, uint8_t size);
bool	RING_buffer_single_FreeSpace(Ring_Buffer_Single_t*	buffer);
bool	RING_buffer_single_PutByte(Ring_Buffer_Single_t*	buffer, uint8_t data);
bool	RING_buffer_single_Available(Ring_Buffer_Single_t*	buffer);
uint8_t	RING_buffer_single_GetByte(Ring_Buffer_Single_t*	buffer);
bool	RING_buffer_single_RxISR(Ring_Buffer_Single_t*		buffer, uint8_t data);
bool	RING_buffer_single_TxISR(Ring_Buffer_Single_t*		buffer, uint8_t* data)

#endif /* RING_BUFFER_SINGLE_DRIVER_H */