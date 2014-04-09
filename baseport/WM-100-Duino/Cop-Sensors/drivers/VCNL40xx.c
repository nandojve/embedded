/*
 * VCNL4xxx.c
 *
 * Created: 17/11/2011 12:21:46
 *  Author: Nando
 */

#include <sysclk.h>
#include <ioport.h>
#include "VCNL40xx.h"
#include "conf_twim.h"

#ifndef VCNL40XX_DEVICE
	#define			VCNL40XX_DEVICE		0x13
#endif

static twi_package_t package			=
{
	.chip								= VCNL40XX_DEVICE,
	.addr[0]							= 0,
	.addr[1]							= 0,
	.addr[2]							= 0,
	.addr_length						= 1,
};

static vcnl40xx_raw_data_t				local_raw_data;

status_code_t vcnl40xx_init(void)
{
	uint8_t			data;
	package.length						= 1;
	package.buffer						= &data;

	// Proximity
//	package.addr[0]						= REGISTER_PROX_CURRENT;
//	data								= 0x0A;				// 100mA LED
//	twi_master_write(CONF_TWIM_PORT, &package);

//	package.addr[0]						= REGISTER_PROX_TIMING;
//	data								= 0x02;				// DEFAULT 781.25kHz
//	twi_master_write(CONF_TWIM_PORT, &package);

//	package.addr[0]						= REGISTER_PROX_RATE;
//	data								= 0x02;				// 7.8125 measurements
//	twi_master_write(CONF_TWIM_PORT, &package);
	
	// Ambient Light
	package.addr[0]						= REGISTER_AMBI_PARAMETER;
	data								= 0xA6;				// Continuous Conversion + DEFAULT
	twi_master_write(CONF_TWIM_PORT, &package);
	
	// Enable All
	package.addr[0]						= REGISTER_COMMAND;
	data								= 0x1F;
	return(twi_master_write(CONF_TWIM_PORT, &package));
}
status_code_t vcnl40xx_probe(uint8_t* identification)
{
	package.addr[0]						= REGISTER_ID;
	package.length						= 1;
	package.buffer						= identification;
	
	return(twi_master_read(CONF_TWIM_PORT, &package));
}
status_code_t vcnl40xx_start_convertion(void)
{
	uint8_t			data				= 0x18;				// Starts a single on-demand simultaneously measurement of ambient light and proximity

	package.addr[0]						= REGISTER_COMMAND;
	package.buffer						= &data;
	package.length						= 1;

	return(twi_master_write(CONF_TWIM_PORT, &package));
}
status_code_t vcnl40xx_read_raw_data(void)
{
	vcnl40xx_raw_data_t					raw_big16;
	
	//package.addr[0]						= REGISTER_AMBI_VALUE;
	//package.buffer						= &raw_big16.ambient_light;
	//package.length						= sizeof(raw_big16.ambient_light);
//
	//status_code_t			ret			= twi_master_read(CONF_TWIM_PORT, &package);
	//
	//package.addr[0]						= REGISTER_PROX_VALUE;
	//package.buffer						= &raw_big16.proximity;
	//package.length						= sizeof(raw_big16.proximity);
//
	//ret									= twi_master_read(CONF_TWIM_PORT, &package);
//
	package.addr[0]						= REGISTER_AMBI_VALUE;
	package.buffer						= &raw_big16;
	package.length						= sizeof(vcnl40xx_raw_data_t);
	
	status_code_t			ret			= twi_master_read(CONF_TWIM_PORT, &package);

	if(ret == STATUS_OK)
	{
		local_raw_data.ambient_light	= BE16(raw_big16.ambient_light);
		local_raw_data.proximity		= BE16(raw_big16.proximity);
	}

	return(ret);
}
void vcnl40xx_get_raw_data(vcnl40xx_raw_data_t* data)
{
	*data								= local_raw_data;
}
void vcnl40xx_set_raw_data(vcnl40xx_raw_data_t* data)
{
	local_raw_data						= *data;
}
