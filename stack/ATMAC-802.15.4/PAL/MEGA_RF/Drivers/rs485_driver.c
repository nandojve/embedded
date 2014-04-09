/*
 * rs485_driver.c
 *
 * Created: 30/05/2012 22:32
 *  Author: Nando
 */ 
#ifdef RS485_DRIVER

#include "pal.h"
#include "rs485_driver.h"
#include "pal_uart.h"

void rs485_init(void)
{
	pal_sio_init(SIO_RS485);
	RS485_PORTDDR	|=	(1 << RS485_PORT_RE) | (1 << RS485_PORT_DE);
	rs485_channel_mode(RS485_CHANNEL_MODE_READ);
}
void rs485_channel_mode(rs485_channel_mode_t mode)
{
	if(mode == RS485_CHANNEL_MODE_READ)
	{
		RS485_PORT	&=	~((1 << RS485_PORT_RE) | (1 << RS485_PORT_DE));
	}
	else if(mode == RS485_CHANNEL_MODE_WRITE)
	{
		RS485_PORT	|=	(1 << RS485_PORT_RE) | (1 << RS485_PORT_DE);
	}
	else if(mode == RS485_CHANNEL_MODE_READWRITE)
	{
		RS485_PORT	|=	(1 << RS485_PORT_DE);
		RS485_PORT	&=	~(1 << RS485_PORT_RE);
	}
	else
	{
		RS485_PORT	|=	(1 << RS485_PORT_RE);
		RS485_PORT	&=	~(1 << RS485_PORT_DE);
	}
}
void rs485_purge(void)
{
	pal_sio_purge(SIO_RS485);
}
uint8_t rs485_rx_length(void)
{
	pal_sio_rx_length(SIO_RS485);
}	
uint8_t rs485_read(uint8_t* data_buffer, uint8_t size)
{
	return(pal_sio_rx(SIO_RS485, data_buffer, size));
}
uint8_t rs485_write(uint8_t* data_buffer, uint8_t size)
{
	rs485_channel_mode(RS485_CHANNEL_MODE_WRITE);
	uint8_t	written = pal_sio_tx(SIO_RS485, data_buffer, size);
	while(!pal_sio_tx_empty(SIO_RS485));
	rs485_channel_mode(RS485_CHANNEL_MODE_READ);
	return(written);
}

#endif /* RS485_DRIVER */