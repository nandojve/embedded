/*
 * rs485_driver.c
 *
 * Created: 30/05/2012 22:32
 *  Author: Nando
 */ 

#ifndef RS485_DRIVER_H_
#define RS485_DRIVER_H_

#include "avr_compiler.h"

#define	RS485_PORT				PORTD
#define	RS485_PORTDDR			DDRD
#define	RS485_PORT_DE			DDD5
#define	RS485_PORT_RE			DDD6
#define	SIO_RS485				SIO_1

typedef enum rs485_channel_mode_tag
{
	RS485_CHANNEL_MODE_STANDBY = 0,		// Deactivate Transceiver
	RS485_CHANNEL_MODE_READ,				// Activate Half Duplex - Read Only
	RS485_CHANNEL_MODE_WRITE,				// Activate Half Duplex - Write Only
	RS485_CHANNEL_MODE_READWRITE,			// Activate Full Duplex Mode
} rs485_channel_mode_t;

void		rs485_init(void);
void		rs485_channel_mode(rs485_channel_mode_t mode);
uint8_t		rs485_rx_length(void);
void		rs485_purge(void);
uint8_t		rs485_read(uint8_t* data_buffer, uint8_t size);
uint8_t		rs485_write(uint8_t* data_buffer, uint8_t size);

#endif /* RS485_DRIVER_H_ */