/*
 * BMP085.c
 *
 * Created: 28/04/2012 11:25
 *  Author: Nando
 */ 
#include "BMP085.h"
#include "TWIM_Driver.h"

#define OSS 0	// Oversampling Setting (note: code is not set up to use other OSS values)
bmp085_calibration_param_t calibration_values;

void bmp085_test(void)
{
	calibration_values.ac1 = 408;
	calibration_values.ac2 = -72;
	calibration_values.ac3 = -14383;
	calibration_values.ac4 = 32741;
	calibration_values.ac5 = 32757;
	calibration_values.ac6 = 23153;
	calibration_values.b1 = 6190;
	calibration_values.b2 = 4;
	calibration_values.mb = -32768;
	calibration_values.mc = -8711;
	calibration_values.md = 2868;
}
void bmp085_init(void)
{
	//uint8_t chipid;
	//// get device ID
	//bmp085_writeAddress(BMP085_CHIP_ID_REG);		// First Calibration Offset
	//twim_wait();
	//twim_read(BMP085_DEVICE, 1);	// Read Device ID
	//twim_wait();
	//bmp085_getDatab(&chipid);
	//
	// get Calibrate Values from Sensor
	bmp085_writeAddress(0xAA);		// First Calibration Offset
	twim_wait();
	
	twim_read(BMP085_DEVICE, 2);	// Read AC1
	twim_wait();
	bmp085_getDataw(&calibration_values.ac1);

	bmp085_writeAddress(0xAC);
	twim_wait();

	twim_read(BMP085_DEVICE, 2);	// Read AC2
	twim_wait();
	bmp085_getDataw(&calibration_values.ac2);

	bmp085_writeAddress(0xAE);
	twim_wait();

	twim_read(BMP085_DEVICE, 2);	// Read AC3
	twim_wait();
	bmp085_getDataw(&calibration_values.ac3);

	bmp085_writeAddress(0xB0);
	twim_wait();

	twim_read(BMP085_DEVICE, 2);	// Read AC4
	twim_wait();
	bmp085_getDataw(&calibration_values.ac4);

	bmp085_writeAddress(0xB2);
	twim_wait();

	twim_read(BMP085_DEVICE, 2);	// Read AC5
	twim_wait();
	bmp085_getDataw(&calibration_values.ac5);

	bmp085_writeAddress(0xB4);
	twim_wait();

	twim_read(BMP085_DEVICE, 2);	// Read AC6
	twim_wait();
	bmp085_getDataw(&calibration_values.ac6);
	
	bmp085_writeAddress(0xB6);		// First Calibration Offset
	twim_wait();

	twim_read(BMP085_DEVICE, 2);	// Read B1
	twim_wait();
	bmp085_getDataw(&calibration_values.b1);

	bmp085_writeAddress(0xB8);
	twim_wait();

	twim_read(BMP085_DEVICE, 2);	// Read B2
	twim_wait();
	bmp085_getDataw(&calibration_values.b2);

	bmp085_writeAddress(0xBA);
	twim_wait();

	twim_read(BMP085_DEVICE, 2);	// Read MB
	twim_wait();
	bmp085_getDataw(&calibration_values.mb);

	bmp085_writeAddress(0xBC);
	twim_wait();

	twim_read(BMP085_DEVICE, 2);	// Read MC
	twim_wait();
	bmp085_getDataw(&calibration_values.mc);

	bmp085_writeAddress(0xBE);
	twim_wait();

	twim_read(BMP085_DEVICE, 2);	// Read MD
	twim_wait();
	bmp085_getDataw(&calibration_values.md);
}
bool bmp085_writeAddress(uint8_t address)
{
	uint8_t	addr = address;
	return(twim_writeread(BMP085_DEVICE, &addr, 1, 0));
}
bool bmp085_readAddress(void)
{
	return(twim_read(BMP085_DEVICE, 2));
}
bool bmp085_ready(void)
{
	return(twim_ready());
}
bool bmp085_write(uint8_t address, uint16_t data)
{
	uint8_t	buf[3];

	buf[0]		= address;
	buf[1]		= (data >> 8);
	buf[2]		= (data & 0xFF);

	return(twim_writeread(BMP085_DEVICE, buf, 3, 0));
}
bool bmp085_getDataw(uint16_t* data)
{
	if(twim_getresult() == TWIM_RESULT_OK)
	{
		uint8_t d[2];
		twim_getdata(d, 2);
		
		*data = d[0]<< 8;
		*data += d[1];

		return(true);
	}
	
	return(false);
}
bool bmp085_getDatab(uint8_t* data)
{
	if(twim_getresult() == TWIM_RESULT_OK)
	{
		twim_getdata(data, 1);

		return(true);
	}
	
	return(false);
}

bool bmp085_getTempUncompensated(int32_t* ut)
{
	uint8_t buf[2];
	buf[0] = 0xF4;
	buf[1] = 0x2E;
	twim_writeread(BMP085_DEVICE, buf, 2, 0);
	twim_wait();

	_delay_ms(5);

	bmp085_writeAddress(0xF6);
	twim_wait();
	
	twim_read(BMP085_DEVICE, 2);
	twim_wait();
	
	if(!twim_getdata(buf, 2))
		return(false);

	*ut = (int32_t) buf[0] << 8;
	*ut += (int32_t) buf[1];
	
	return(true);
}
bool bmp085_getPressureUncompensated(int32_t* up)
{
	uint8_t buf[3];
	buf[0] = 0xF4;
	buf[1] = 0x34 + (OSS << 6);
	twim_writeread(BMP085_DEVICE, buf, 2, 0);
	twim_wait();

	_delay_ms(2 + (3 << (OSS)));

	bmp085_writeAddress(0xF6);
	twim_wait();
	
	twim_read(BMP085_DEVICE, 3);
	twim_wait();
	
	if(!twim_getdata(buf, 3))
		return(false);

	*up = (((uint32_t) buf[0] << 16) | ((uint32_t) buf[1] << 8) | (uint32_t) buf[2]) >> (8 - OSS);
	
	return(true);	
}

void bmp085_calcTemp(int32_t* ut, int32_t* temp)
{
	int32_t x1,x2;

	x1 = (((int32_t) *ut - (int32_t) calibration_values.ac6) * (int32_t) calibration_values.ac5) >> 15;
	x2 = ((int32_t) calibration_values.mc << 11) / (x1 + calibration_values.md);
	calibration_values.b5 = x1 + x2;
	*temp = ((calibration_values.b5 + 8) >> 4);		// temperature in 0.1°C
}
void bmp085_calcPressure(int32_t* up, int32_t* pressure)
{
	int32_t x1,x2,x3,b3,b6;
	uint32_t b4, b7;
   
	b6 = calibration_values.b5 - 4000;
	//*****calculate B3************
	x1 = (b6*b6) >> 12;
	x1 *= calibration_values.b2;
	x1 >>=11;

	x2 = (calibration_values.ac2*b6);
	x2 >>=11;

	x3 = x1 +x2;

	b3 = (((((int32_t) calibration_values.ac1 )*4 + x3) <<OSS) + 2) >> 2;

	//*****calculate B4************
	x1 = (calibration_values.ac3* b6) >> 13;
	x2 = (calibration_values.b1 * ((b6*b6) >> 12) ) >> 16;
	x3 = ((x1 + x2) + 2) >> 2;
	b4 = (calibration_values.ac4 * (uint32_t) (x3 + 32768)) >> 15;
     
	b7 = ((uint32_t)(*up - b3) * (50000>>OSS));   
	if(b7 < 0x80000000)
		*pressure = (b7 << 1) / b4;
	else
		*pressure = (b7 / b4) << 1;
   
	x1 = *pressure >> 8;
	x1 *= x1;
	x1 = (x1 * SMD500_PARAM_MG) >> 16;
	x2 = (*pressure * SMD500_PARAM_MH) >> 16;
	*pressure += (x1 + x2 + SMD500_PARAM_MI) >> 4;	// pressure in Pa  
}

void bmp085_calcAltitude(int32_t* pressure, float* altitude)
{
	float	pression_ratio	= ((float) *pressure / 10132500);
	float	power			= pow(pression_ratio, (float) BMP085_ALTITUDE_RATIO);
	*altitude				= 44330.00 * (1.00 - power);
}