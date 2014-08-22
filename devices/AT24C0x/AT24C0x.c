/*
 * AT24C0x.h
 *
 * Created: 16/02/2014 18:14
 *  Author: Nando
 */

#include <compiler.h>
#include <twi_master.h>
#include "AT24C0x.h"

static twi_package_t package	=
{
	.chip						= 0,
	.addr[0]					= 0,
	.addr[1]					= 0,
	.addr[2]					= 0,
	.addr_length				= 1,
//	.no_wait					= false
};

status_code_t at24c0x_read_byte(uint8_t device, uint8_t* data)
{
	package.chip				= device;
	package.addr_length			= 0;
	package.buffer				= data;
	package.length				= 1;

	return(twi_master_read(TWIM_MASTER_PORT, &package));
}
status_code_t at24c0x_read_page(uint8_t device, uint16_t address, uint8_t *data)
{
	uint8_t			page		= ((address >> 8) & 0x03);
	uint8_t			addr		= (address & 0xF0);

	package.chip				= device | page;
	package.addr[0]				= addr;
	package.addr_length			= 1;
	package.buffer				= data;
	package.length				= 16;

	return(twi_master_read(TWIM_MASTER_PORT, &package));
}
status_code_t at24c0x_write_byte(uint8_t device, uint16_t address, uint8_t data)
{
	uint8_t			page		= ((address >> 8) & 0x03);
	uint8_t			addr		= (address & 0xFF);
	
	package.chip				= device | page;
	package.addr[0]				= addr;
	package.addr_length			= 1;
	package.buffer				= &data;
	package.length				= 1;

	return(twi_master_write(TWIM_MASTER_PORT, &package));
}
status_code_t at24c0x_write_page(uint8_t device, uint16_t address, uint8_t *data)
{
	uint8_t			page		= ((address >> 8) & 0x03);
	uint8_t			addr		= (address & 0xF0);
	
	package.chip				= device | page;
	package.addr[0]				= addr;
	package.addr_length			= 1;
	package.buffer				= data;
	package.length				= 16;

	return(twi_master_write(TWIM_MASTER_PORT, &package));
}