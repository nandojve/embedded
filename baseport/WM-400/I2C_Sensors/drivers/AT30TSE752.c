/*
 * AT30TSE752.c
 *
 * Created: 01/03/2014 09:58
 *  Author: Nando
 *  Device: AT30TSE752
 */

#include <twi_master.h>
#include <math.h>
#include "AT30TSE752.h"
#include "conf_twim.h"

static twi_package_t package			=
{
	.chip								= AT30TSE752_TEMP_DEVICE,
	.addr[0]							= 0,
	.addr[1]							= 0,
	.addr[2]							= 0,
	.addr_length						= 0,
//	.no_wait							= false
};

static at30tse75x_registers_t at30tse52x_register			= AT30TSE52X_UNKNOWN_REG;

status_code_t at30tse752_set_register(at30tse75x_registers_t reg)
{
	uint8_t					pointer		= (uint8_t) reg;

	package.addr_length					= 0;
	package.buffer						= &pointer;
	package.length						= 1;

	status_code_t			ret			= twi_master_write(CONF_TWIM_PORT, &package);

	if(ret == STATUS_OK)
	{
		at30tse52x_register				= reg;
	}

	return(ret);
}
status_code_t at30tse752_read_register(at30tse75x_registers_t reg, uint16_t* data)
{
	status_code_t			ret;
	
	if(reg != at30tse52x_register)
	{
		ret								= at30tse752_set_register(reg);
		if(ret != STATUS_OK)
		{
			return(ret);
		}
	}
	
	uint16_t				n_data;
	
	package.addr_length					= 0;
	package.buffer						= &n_data;
	package.length						= 2;
	
	ret									= twi_master_read(CONF_TWIM_PORT, &package);
	
	if(ret == STATUS_OK)
	{
		*data							= BE16(n_data);
	}
	
	return(ret);
}
status_code_t at30tse752_write_register(at30tse75x_registers_t reg, uint16_t data)
{
	at30tse52x_register					= reg;
	package.addr[0]						= reg;
	package.addr_length					= 1;
	uint16_t				n_data		= BE16(data);

	package.buffer						= &n_data;
	package.length						= 2;

	return(twi_master_write(CONF_TWIM_PORT, &package));
}
float at30tse752_itemp_to_ftemp(int16_t itemp)
{
	float				ftemp			= 0.0625 * ((itemp & 0xFF) >> 4);
	ftemp								= ((itemp >> 8) & 0x7F) + ftemp;
	ftemp								*= itemp < 0 ? -1 : 1;

	return(ftemp);
}
bool at30tse752_getConfig(at30tse75x_config_t* config)
{
	return(at30tse752_read_register(AT30TSE52X_CONF_REG, (uint16_t *) config) == STATUS_OK);
}
bool at30tse752_setConfig(at30tse75x_config_t* config)
{
	return(at30tse752_write_register(AT30TSE52X_CONF_REG, *((uint16_t *) config)) == STATUS_OK);
}
bool at30tse752_getNVMConfig(at30tse75x_nonvolatile_config_t* config)
{
	return(at30tse752_read_register(AT30TSE52X_NVM_CONF_REG, (uint16_t *) config) == STATUS_OK);
}
bool at30tse752_setNVMConfig(at30tse75x_nonvolatile_config_t* config)
{
	return(at30tse752_write_register(AT30TSE52X_NVM_CONF_REG, *((uint16_t *) config)) == STATUS_OK);
}
bool at30tse752_exec_cmd(at30tse75x_command_t cmd)
{
	uint8_t					data		= (uint8_t) cmd;

	package.addr_length					= 0;
	package.buffer						= &data;
	package.length						= 1;

	status_code_t			ret			= twi_master_write(CONF_TWIM_PORT, &package);

	return(ret == STATUS_OK);
}
bool at30tse752_read_limit_temp(bool isHigh, uint16_t* data)
{
	return(at30tse752_read_register((isHigh ? AT30TSE52X_Thigh_REG : AT30TSE52X_Tlow_REG), data));
}
bool at30tse752_write_limit_temp(bool isHigh, uint16_t* data)
{
	return(at30tse752_write_register((isHigh ? AT30TSE52X_Thigh_REG : AT30TSE52X_Tlow_REG), *data));
}
