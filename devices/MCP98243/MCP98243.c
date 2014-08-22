/*
 * MCP98243.c
 *
 * Created: 16/11/2011 23:59:42
 *  Author: Nando
 */ 

#include <compiler.h>
#include <twi_master.h>

#include "MCP98243.h"

#ifndef MCP98243_TEMP_DEVICE
	#define MCP98243_TEMP_DEVICE		(0x0C)
#else
	#define MCP98243_MEM_DEVICE			(0x28)
#endif

static twi_package_t package								=
{
	.chip													= MPL115A2_DEVICE,
	.addr[0]												= 0,
	.addr[1]												= 0,
	.addr[2]												= 0,
	.addr_length											= 1,
};

void mcp98243_init(void)
{
	// Set default config...
}
bool mcp98243_ioctl(uint8_t command)
{
	package.addr[0]						= MPL115A2_A0_COEFFICIENT;
	package.buffer						= &coefficient_big16;
	package.length						= sizeof(mpl115a2_coeffient_data_t);

	status_code_t				ret		= twi_master_read(TWIM_MASTER_PORT, &package);
	if(ret == STATUS_OK)

	return(twim_writeread(MCP98243_DEVICE, &command, 1, 2));
}
bool mcp89243_writeAddress(uint8_t address)
{
	uint8_t	addr = address;
	return(twim_writeread(MCP98243_DEVICE, &addr, 1, 0));
}
bool mcp89243_readAddress(void)
{
	return(twim_read(MCP98243_DEVICE, 2));
}
bool mcp98243_eeprom_addressw(uint8_t address)
{
	uint8_t	addr = address;
	return(twim_writeread(MCP98243_DEVICE_EEPROM, &addr, 1, 0));
}
bool mcp98243_eeprom_datar(void)
{
	return(twim_read(MCP98243_DEVICE_EEPROM, 1));
}
bool mcp98243_eeprom_dataw(uint8_t address, uint8_t data)
{
	uint8_t buf[2];
	buf[0] = address;
	buf[1] = data;
	return(twim_writeread(MCP98243_DEVICE_EEPROM, &buf, 2, 0));
}
bool mcp89243_ready(void)
{
	return(twim_ready());
}
bool mcp98243_getDataw(uint16_t* data)
{
	if(twim_getresult() == TWIM_RESULT_OK)
	{
		twim_getdata((uint8_t *) data, 2);

		return(true);
	}
	
	return(false);
}
bool mcp98243_getDatab(uint8_t* data)
{
	if(twim_getresult() == TWIM_RESULT_OK)
	{
		twim_getdata(data, 1);

		return(true);
	}
	
	return(false);
}
bool mcp98243_getTemp(float* temp)
{
	uint8_t				address = 0x05;
	twim_writeread(MCP98243_DEVICE, &address, 1, 0);
	twim_wait();
	
	twim_read(MCP98243_DEVICE, 2);
	twim_wait();
	
	uint8_t				buf[2];
	if(!twim_getdata(buf, 2))
		return(false);

	float				ltemp = 0;	

	if(buf[0] & 0x08)
		ltemp			+= 128;
	if(buf[0] & 0x04)
		ltemp			+= 64;
	if(buf[0] & 0x02)
		ltemp			+= 32;
	if(buf[0] & 0x01)
		ltemp			+= 16;
	if(buf[1] & 0x80)
		ltemp			+= 8;
	if(buf[1] & 0x40)
		ltemp			+= 4;
	if(buf[1] & 0x20)
		ltemp			+= 2;
	if(buf[1] & 0x10)
		ltemp			+= 1;
	if(buf[1] & 0x08)
		ltemp			+= 0.5;
	if(buf[1] & 0x04)
		ltemp			+= 0.25;
	if(buf[1] & 0x02)
		ltemp			+= 0.125;
	if(buf[1] & 0x01)
		ltemp			+= 0.0625;

	if(buf[0] & 0x10)
		ltemp			*= -1;
		
	*temp				= ltemp;

	return(true);
}