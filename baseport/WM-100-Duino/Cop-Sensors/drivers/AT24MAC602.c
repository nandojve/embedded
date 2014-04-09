/*
 * AT24MAC602.c
 *
 * Created: 01/03/2014 09:58
 *  Author: Nando
 *  Device: AT24MAC602
 */

#include <twi_master.h>
#include "AT24MAC602.h"
#include "conf_twim.h"

static twi_package_t package	=
{
	.chip						= 0,
	.addr[0]					= 0,
	.addr[1]					= 0,
	.addr[2]					= 0,
	.addr_length				= 0,
//	.no_wait					= false
};

status_code_t at24mac602_read_sn(uint8_t* data)
{
	package.chip				= AT24MAC602_EUI_DEVICE;
	package.addr[0]				= 0x80;
	package.addr_length			= 1;
	package.buffer				= data;
	package.length				= 16;

	return(twi_master_read(CONF_TWIM_PORT, &package));
}
status_code_t at24mac602_read_eui(uint8_t* data)
{
	package.chip				= AT24MAC602_EUI_DEVICE;
	package.addr[0]				= 0x98;
	package.addr_length			= 1;
	package.buffer				= data;
	package.length				= 8;

	return(twi_master_read(CONF_TWIM_PORT, &package));
}
