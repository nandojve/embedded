/*
 * MPL115A2.c
 *
 * Created: 01/03/2014 13:45
 *  Author: Nando
 */

#include <sysclk.h>
#include <ioport.h>
#include <math.h>
#include "MPL115A2.h"
#include "conf_twim.h"

static twi_package_t package								=
{
	.chip													= MPL115A2_DEVICE,
	.addr[0]												= 0,
	.addr[1]												= 0,
	.addr[2]												= 0,
	.addr_length											= 1,
};

static mpl115a2_coeffient_data_t		local_coefficient;
static mpl115a2_raw_data_t				local_raw_data;
static float							fa0					= 0.0;
static float							fb1					= 0.0;
static float							fb2					= 0.0;
static float							fc12				= 0.0;
static float							local_altitude		= 261.00;				// Florianópolis Pressure

status_code_t mpl115a2_init(void)
{
	mpl115a2_coeffient_data_t			coefficient_big16;

	package.addr[0]						= MPL115A2_A0_COEFFICIENT;
	package.buffer						= &coefficient_big16;
	package.length						= sizeof(mpl115a2_coeffient_data_t);

	status_code_t				ret		= twi_master_read(CONF_TWIM_PORT, &package);
	if(ret == STATUS_OK)
	{
		local_coefficient.a0			= BE16(coefficient_big16.a0);
		local_coefficient.b1			= BE16(coefficient_big16.b1);
		local_coefficient.b2			= BE16(coefficient_big16.b2);
		local_coefficient.c12			= BE16(coefficient_big16.c12);

		fa0								= local_coefficient.a0 / 8.0;
		fb1								= local_coefficient.b1 / 8192.0;
		fb2								= local_coefficient.b2 / 16384.0;
		fc12							= local_coefficient.c12 / 16777216.0;
	}

	return(ret);
}
status_code_t mpl115a2_start_convertion(void)
{
	package.addr[0]						= MPL115A2_START_CONVERTION;

	return(twi_master_write(CONF_TWIM_PORT, &package));
}
status_code_t mpl115a2_read_raw_data(void)
{
	mpl115a2_raw_data_t					raw_big16;
	
	package.addr[0]						= MPL115A2_PRESSURE;
	package.buffer						= &raw_big16;
	package.length						= sizeof(mpl115a2_raw_data_t);

	status_code_t				ret		= twi_master_read(CONF_TWIM_PORT, &package);
	if(ret == STATUS_OK)
	{
		local_raw_data.pressure			= (BE16(raw_big16.pressure) >> 6);
		local_raw_data.temperature		= (BE16(raw_big16.temperature) >> 6);
	}

	return(ret);
}
void mpl115a2_evaluate(mpl115a2_conpensate_data_t* data)
{
	float	c12x2						= fc12 * local_raw_data.temperature;
	float	a1							= fb1 + c12x2;
	float	a1x1						= a1 * local_raw_data.pressure;
	float	y1							= fa0 + a1x1;
	float	a2x2						= fb2 * local_raw_data.temperature;
	float	pcomp						= y1 + a2x2;

	data->pressure						= (pcomp * ((115.0 - 55.0) / 1023.0)) + 50.0;
	data->temperature					= ((c12x2 - a2x2) * ((105.0 - -40.0) / 1023.0)) + -40.0;
}
void mpl115a2_get_coefficient(mpl115a2_coeffient_data_t* data)
{
	*data								= local_coefficient;
}
void mpl115a2_set_coefficient(mpl115a2_coeffient_data_t* data)
{
	local_coefficient					= *data;

	fa0									= local_coefficient.a0 / 8.0;
	fb1									= local_coefficient.b1 / 8192.0;
	fb2									= local_coefficient.b2 / 16384.0;
	fc12								= local_coefficient.c12 / 16777216.0;
}
void mpl115a2_get_raw_data(mpl115a2_raw_data_t* data)
{
	*data								= local_raw_data;
}
void mpl115a2_set_raw_data(mpl115a2_raw_data_t* data)
{
	local_raw_data						= *data;
}
void mpl115a2_evaluate_weather_simple(mpl115a2_weather_data_t* data)
{
	mpl115a2_conpensate_data_t			barometric_status;

	mpl115a2_evaluate(&barometric_status);
	
	data->pressure						= barometric_status.pressure;
	data->temperature					= barometric_status.temperature;
	
	// Please, see International Barometric Formula to understand the constants
	float				ratio			= (barometric_status.pressure / SEA_LEVEL_PRESSURE);
	float				power			= pow(ratio, ALTITUDE_RATIO);
	data->altitude						= 44330.00 * (1.00 - power);
	
	// Please, see Height (m) to Pressure (kPa) exponential equation to understand the constants
	float				weather			= SEA_LEVEL_PRESSURE * exp(data->altitude / -7990.0);
	float				diff			= data->pressure - weather;
	
	if(diff < -0.25)
		data->type						= MPL115A2_WEATHER_TYPE_RAIN;
	else
	{
		data->type						= MPL115A2_WEATHER_TYPE_SUN;

		if(diff < 0.25)
			data->type					|= MPL115A2_WEATHER_TYPE_CLOUD;
	}
}
void mpl115a2_weather_set_local_altitude(float altitude)
{
	local_altitude						= altitude;
}
void mpl115a2_weather_get_local_altitude(float* altitude)
{
	*altitude							= local_altitude;
}