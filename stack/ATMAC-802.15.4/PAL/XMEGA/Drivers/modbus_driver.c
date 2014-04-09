//#ifdef MODBUS_DRIVER

/* ----------------------- Driver includes ----------------------------------*/
#include "modbus_driver.h"
#include "ring_buffer_single_driver.h"
#include "usart_driver_ext.h"

/* ----------------------- Modbus includes ----------------------------------*/
#include "mb.h"
#include "mbport.h"

/* ----------------------- Defines ------------------------------------------*/

#define MODBUS_UART_BAUD_CALC(UART_BAUD_RATE, F_OSC)		((F_OSC) / ((UART_BAUD_RATE) * 16UL) - 1)
#define MODBUS_SERIAL_BUFFER_SIZE		256

/* ----------------------- Static variables ---------------------------------*/

volatile uint8_t						modbusRxTxBuffer[MODBUS_SERIAL_BUFFER_SIZE];
Ring_Buffer_Single_t					modbusRingBuffer;
USART_driver_t							modbusUsart;

static USHORT							usTimerOCRADelta;
static eMBEventType						eQueuedEvent;
static BOOL								xEventInQueue;

/* ----------------------- Modbus Serial ------------------------------------*/

BOOL xMBPortSerialInit(UCHAR ucPORT, ULONG ulBaudRate, UCHAR ucDataBits, eMBParity eParity)
{
	if(RING_buffer_single_Initialize(&modbusRingBuffer, &modbusRxTxBuffer, MODBUS_SERIAL_BUFFER_SIZE))
	{
		return(FALSE);
	}

	USART_InterruptDriver_Initialize(&modbusUsart, &MODBUS_SERIAL_PORT, MODBUS_SERIAL_RXCINT, MODBUS_SERIAL_TXCINT, MODBUS_SERIAL_DREINT);

    UCHAR ucUCSRC						= 0;

    /* prevent compiler warning. */
    (void) ucPORT;

    UBRR								= UART_BAUD_CALC(ulBaudRate, F_CPU);

	switch(eParity)
	{
		case MB_PAR_EVEN:
			ucUCSRC						|= USART_PMODE_EVEN_gc;
			break;
		case MB_PAR_ODD:
			ucUCSRC						|= USART_PMODE_ODD_gc;
			break;
		case MB_PAR_NONE:
			ucUCSRC						|= USART_PMODE_DISABLED_gc;
			ucUCSRC						|= USART_SBMODE_bm;	// Needs Two Stop bits if parity is none
			break;
	}

	switch(ucDataBits)
	{
		case 8:
			ucUCSRC						|= USART_CHSIZE_8BIT_gc
			break;
		case 7:
			ucUCSRC						|= USART_CHSIZE_7BIT_gc
			break;
	}

	MODBUS_SERIAL_PORT.CTRLC			= ucUCSRC;

	vMBPortSerialEnable(FALSE, FALSE);

#ifdef RTS_ENABLE
		RTS_INIT;
#endif

	return(TRUE);
}

void vMBPortSerialEnable(BOOL xRxEnable, BOOL xTxEnable)
{
    if(xRxEnable)
    {
		USART_Rx_Enable(MODBUS_SERIAL_PORT);
		USART_RxdInterruptLevel_Set(&MODBUS_SERIAL_PORT, MODBUS_SERIAL_RXCINT);
    }
    else
    {
		USART_Rx_Disable(MODBUS_SERIAL_PORT);
		USART_RxdInterruptLevel_Set(&MODBUS_SERIAL_PORT, USART_RXCINTLVL_OFF_gc);
    }

    if(xTxEnable)
    {
		USART_Tx_Enable(MODBUS_SERIAL_PORT);
		USART_DreInterruptLevel_Set(&MODBUS_SERIAL_PORT, MODBUS_SERIAL_DREINT);
#ifdef RTS_ENABLE
		USART_TxdInterruptLevel_Set(&MODBUS_SERIAL_PORT, MODBUS_SERIAL_TXCINT);
        RTS_HIGH;
#endif
    }
    else
    {
		USART_Tx_Disable(MODBUS_SERIAL_PORT);
		USART_DreInterruptLevel_Set(&MODBUS_SERIAL_PORT, USART_DREINTLVL_OFF_gc);
#ifdef RTS_ENABLE
		USART_TxdInterruptLevel_Set(&MODBUS_SERIAL_PORT, USART_TXCINTLVL_OFF_gc);
#endif
    }
}

BOOL xMBPortSerialPutByte(CHAR ucByte)
{
	USART_PutChar(&MODBUS_SERIAL_PORT, ucByte);

    return(TRUE);
}

BOOL xMBPortSerialGetByte(CHAR * pucByte)
{
    *pucByte					= USART_GetChar(&MODBUS_SERIAL_PORT);

    return(TRUE);
}

ISR(MODBUS_DRE_IRQ)
{
    pxMBFrameCBTransmitterEmpty();
}

ISR(MODBUS_RXC_IRQ)
{
    pxMBFrameCBByteReceived();
}

#ifdef RTS_ENABLE
ISR(MODBUS_TXC_IRQ)
{
    RTS_LOW;
}
#endif

/* ----------------------- Modbus Timers ------------------------------------*/

BOOL xMBPortTimersInit(USHORT usTim1Timerout50us)
{
    /* Calculate overflow counter an OCR values for Timer. */
    usTimerOCRADelta			= (MB_TIMER_TICKS * usTim1Timerout50us) / (MB_50US_TICKS);

	MODBUS_TIMER.CTRLA			= TC_CLKSEL_OFF_gc;
	MODBUS_TIMER.CTRLB			= 0x00;
	MODBUS_TIMER.CTRLC			= 0x00;
	MODBUS_TIMER.CTRLD			= 0x00;
	MODBUS_TIMER.CTRLE			= 0x00;

    vMBPortTimersDisable();

    return(TRUE);
}

inline void vMBPortTimersEnable()
{
	MODBUS_TIMER.CNT			= 0x0000;

	if(usTimerOCRADelta > 0)
    {
		MODBUS_TIMER.INTCTRLB	= (MODBUS_TIMER.INTCTRLB & ~TC0_CCAINTLVL_gm) | MODBUS_TIMER_INTLVL;
		MODBUS_TIMER.CCA		= usTimerOCRADelta;
    }

	MODBUS_TIMER.CTRLA			= MODBUS_TIMER_PRESCALER;
}

inline void vMBPortTimersDisable()
{
    /* Disable the timer. */
	MODBUS_TIMER.CTRLA			= TC_CLKSEL_OFF_gc;

    /* Disable the output compare interrupts for channel A/B. */
	MODBUS_TIMER.INTCTRLB		= (MODBUS_TIMER.INTCTRLB & ~TC0_CCAINTLVL_gm) | TC_CCAINTLVL_OFF_gc;

	/* Clear output compare flags for channel A/B. */
	MODBUS_TIMER.INTFLAGS		= TC0_CCAIF_bm;
}

ISR(MODBUS_TIMER_INTR)
{
	(void) pxMBPortCBTimerExpired();
}

/* ----------------------- Modbus Events ------------------------------------*/

BOOL xMBPortEventInit(void)
{
    xEventInQueue				= FALSE;
    
	return(TRUE);
}

BOOL xMBPortEventPost(eMBEventType eEvent)
{
    xEventInQueue				= TRUE;
    eQueuedEvent				= eEvent;
    
	return(TRUE);
}

BOOL xMBPortEventGet(eMBEventType *eEvent)
{
    BOOL xEventHappened			= FALSE;

    if(xEventInQueue)
    {
        *eEvent					= eQueuedEvent;
        xEventInQueue			= FALSE;
        xEventHappened			= TRUE;
    }

    return(xEventHappened);
}

#endif /* MODBUS_DRIVER */