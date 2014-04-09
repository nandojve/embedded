/* This file has been prepared for Doxygen automatic documentation generation.*/
/*! \file *********************************************************************
 *
 * \brief
 *      XMEGA USART driver source file.
 *
 *      This file contains the function implementations the XMEGA interrupt
 *      and polled USART driver.
 *
 *      The driver is not intended for size and/or speed critical code, since
 *      most functions are just a few lines of code, and the function call
 *      overhead would decrease code performance. The driver is intended for
 *      rapid prototyping and documentation purposes for getting started with
 *      the XMEGA ADC module.
 *
 *      For size and/or speed critical code, it is recommended to copy the
 *      function contents directly into your application instead of making
 *      a function call.
 *
 *      Some functions use the following construct:
 *          "some_register = ... | (some_parameter ? SOME_BIT_bm : 0) | ..."
 *      Although the use of the ternary operator ( if ? then : else ) is discouraged,
 *      in some occasions the operator makes it possible to write pretty clean and
 *      neat code. In this driver, the construct is used to set or not set a
 *      configuration bit based on a boolean input parameter, such as
 *      the "some_parameter" in the example above.
 *
 * \par Application note:
 *      AVR1307: Using the XMEGA USART
 *
 * \par Documentation
 *      For comprehensive code documentation, supported compilers, compiler
 *      settings and supported devices see readme.html
 *
 * \author
 *      Atmel Corporation: http://www.atmel.com \n
 *      Support email: avr@atmel.com
 *
 * $Revision: 1694 $
 * $Date: 2008-07-29 14:21:58 +0200 (ti, 29 jul 2008) $  \n
 *
 * Copyright (c) 2008, Atmel Corporation All rights reserved.
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
 * 3. The name of ATMEL may not be used to endorse or promote products derived
 * from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY ATMEL "AS IS" AND ANY EXPRESS OR IMPLIED
 * WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE EXPRESSLY AND
 * SPECIFICALLY DISCLAIMED. IN NO EVENT SHALL ATMEL BE LIABLE FOR ANY DIRECT,
 * INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF
 * THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *****************************************************************************/
//#ifdef USART_DRIVER_EXT
#include "usart_driver_ext.h"

/*! \brief Initializes buffer and selects what USART module to use.
 *
 *  Initializes receive and transmit buffer and selects what USART module to use,
 *  and stores the data register empty interrupt level.
 *
 *  \param usart_data           The USART_data_t struct instance.
 *  \param usart                The USART module.
 *  \param rxcIntLevel          Receive complete interrupt level.
 *  \param txcIntLevel          Transmite complete interrupt level.
 *  \param dreIntLevel          Data register empty interrupt level.
 */
void USART_InterruptDriver_Initialize(USART_driver_t* usart_driver,
                                      USART_t* usart,
                                      USART_RXCINTLVL_t rxcIntLevel,
									  USART_TXCINTLVL_t txcIntLevel,
									  USART_DREINTLVL_t dreIntLevel)
{
	usart_driver->usart			= usart;
	usart_driver->rxcIntLevel	= rxcIntLevel;
	usart_driver->txcIntLevel	= txcIntLevel;
	usart_driver->dreIntLevel	= dreIntLevel;
}

/*! \brief Set USART RXC interrupt level.
 *
 *  Set the interrupt level on Receiver interrupt.
 *
 *  \note Changing the RXC interrupt level in the interrupt driver while it is
 *        running will not change the RXC interrupt level in the USART before the
 *        RXC interrupt have been disabled and enabled again.
 *
 *  \param usart_driver       The USART_driver_t struct instance
 *  \param rxcIntLevel        Interrupt level of the RXC interrupt.
 */
void USART_InterruptDriver_RxcInterruptLevel_Set(USART_driver_t* usart_driver, USART_RXCINTLVL_t rxcIntLevel)
{
	usart_driver->rxcIntLevel = rxcIntLevel;
}

/*! \brief Set USART TXC interrupt level.
 *
 *  Set the interrupt level on Transmitter interrupt.
 *
 *  \note Changing the TXC interrupt level in the interrupt driver while it is
 *        running will not change the TXC interrupt level in the USART before the
 *        TXC interrupt have been disabled and enabled again.
 *
 *  \param usart_driver       The USART_driver_t struct instance
 *  \param dreIntLevel        Interrupt level of the TXC interrupt.
 */
void USART_InterruptDriver_TxcInterruptLevel_Set(USART_driver_t* usart_driver, USART_TXCINTLVL_t txcIntLevel)
{
	usart_driver->txcIntLevel = txcIntLevel;
}

/*! \brief Set USART DRE interrupt level.
 *
 *  Set the interrupt level on Data Register interrupt.
 *
 *  \note Changing the DRE interrupt level in the interrupt driver while it is
 *        running will not change the DRE interrupt level in the USART before the
 *        DRE interrupt have been disabled and enabled again.
 *
 *  \param usart_driver       The USART_driver_t struct instance
 *  \param dreIntLevel        Interrupt level of the DRE interrupt.
 */
void USART_InterruptDriver_DreInterruptLevel_Set(USART_driver_t* usart_driver, USART_DREINTLVL_t dreIntLevel)
{
	usart_driver->dreIntLevel = dreIntLevel;
}

USART_RXCINTLVL_t USART_InterruptDriver_RxcInterruptLevel_Get(USART_driver_t* usart_driver)
{
	return(usart_driver->rxcIntLevel);
}
USART_TXCINTLVL_t USART_InterruptDriver_TxcInterruptLevel_Get(USART_driver_t* usart_driver)
{
	return(usart_driver->txcIntLevel);
}
USART_DREINTLVL_t USART_InterruptDriver_DreInterruptLevel_Get(USART_driver_t* usart_driver)
{
	return(usart_driver->dreIntLevel);
}


/*! \brief Put data (9 bit character).
 *
 *  Use the function USART_IsTXDataRegisterEmpty before using this function to
 *  put 9 bit character to the TX register.
 *
 *  \param usart      The USART module.
 *  \param data       The data to send.
 */
void USART_NineBits_PutChar(USART_t * usart, uint16_t data)
{
	if(data & 0x0100) {
		usart->CTRLB |= USART_TXB8_bm;
	}else {
		usart->CTRLB &= ~USART_TXB8_bm;
	}

	usart->DATA = (data & 0x00FF);
}

/*! \brief Get received data (9 bit character).
 *
 *  This function reads out the received 9 bit character (uint16_t).
 *  Use the function USART_IsRXComplete to check if anything is received.
 *
 *  \param usart      The USART module.
 *
 *  \retval           Received data.
 */
uint16_t USART_NineBits_GetChar(USART_t * usart)
{
	if(usart->CTRLB & USART_RXB8_bm) {
		return(0x0100 | usart->DATA);
	}else {
		return(usart->DATA);
	}
}

#endif /* USART_DRIVER_EXT */