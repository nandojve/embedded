/*
 * CC2D33S.c
 *
 * Created: 01/03/2014 13:45
 *  Author: Nando
 */

#include <sysclk.h>
#include <ioport.h>
#include <compiler.h>
#include <twi_master.h>
#include "CC2D33S.h"

#ifndef CC2D33S_DEVICE
	#error "Must be defined CC2D33S_DEVICE constant"
#endif

static twi_package_t package			=
{
	.chip								= CC2D33S_DEVICE,
	.addr[0]							= 0,
	.addr[1]							= 0,
	.addr[2]							= 0,
	.addr_length						= 0,
};

static cc2d33s_raw_data_t				local_raw_data;

status_code_t cc2d33s_start_convertion(void)
{
	uint8_t					status		= 0;				// Dummy byte
	package.buffer						= &status;
	package.length						= sizeof(status);

	return(twi_master_write(CONF_TWIM_PORT, &package));
}
status_code_t cc2d33s_ready(void)
{
	uint8_t					status;
	package.buffer						= &status;
	package.length						= sizeof(status);

	status_code_t			ret			= twi_master_read(CONF_TWIM_PORT, &package);
	if(ret == STATUS_OK)
	{
		if((status & CC2D33S_STATUS_VALID_DATA_Mask) != CC2D33S_STATUS_VALID_DATA)
		{
			ret							= TWI_RECEIVE_NACK;
		}
	}

	return(ret);
}
status_code_t cc2d33s_read_raw_data(void)
{
	cc2d33s_raw_data_t					raw_big16;
	
	package.buffer						= &raw_big16;
	package.length						= sizeof(cc2d33s_raw_data_t);

	status_code_t			ret			= twi_master_read(CONF_TWIM_PORT, &package);
	if(ret == STATUS_OK)
	{
		local_raw_data.humidity			= (BE16(raw_big16.humidity) & 0x3FFF);
		local_raw_data.temperature		= (BE16(raw_big16.temperature) >> 2);
	}

	return(ret);
}
void cc2d33s_evaluate(cc2d33s_conpensate_data_t* data)
{
	// Humidity Output (%RH) (RH_High [5:0] x 256 + RH_Low [7:0])/ 2^14 x 100
	// Temperature Output (°C) (Temp_High [7:0] x 64 + Temp_Low [7:2]/ 4)/ 2^14 x 165 - 40
	
	data->humidity						= (local_raw_data.humidity * ONE_BY_TWO_FOURTEEN * 100.0);
	data->temperature					= (local_raw_data.temperature * ONE_BY_TWO_FOURTEEN * 165.0) - 40.0;
}
void cc2d33s_get_raw_data(cc2d33s_raw_data_t* data)
{
	*data								= local_raw_data;
}
void cc2d33s_set_raw_data(cc2d33s_raw_data_t* data)
{
	local_raw_data						= *data;
}
