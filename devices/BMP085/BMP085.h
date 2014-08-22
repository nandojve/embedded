/*
 * BMP085.h
 *
 * Created: 28/04/2012 11:25
 *  Author: Nando
 */ 


#ifndef BMP085_H_
#define BMP085_H_

#include "avr_compiler.h"

#define		BMP085_DEVICE	0x77

typedef struct {
   int16_t ac1;
   int16_t ac2;
   int16_t ac3;
   uint16_t ac4;
   uint16_t ac5;
   uint16_t ac6;
   int16_t b1;
   int16_t b2;
   int16_t mb;
   int16_t mc;
   int16_t md;

   int32_t b5;	// temp
} bmp085_calibration_param_t;

#define BMP085_PROM_START__ADDR		0xaa
#define BMP085_PROM_DATA__LEN		  22

#define BMP085_CHIP_ID_REG			0xD0
#define BMP085_VERSION_REG			0xD1

#define BMP085_CTRL_MEAS_REG		0xF4
#define BMP085_ADC_OUT_MSB_REG		0xF6
#define BMP085_ADC_OUT_LSB_REG		0xF7

#define BMP085_SOFT_RESET_REG		0xE0

#define BMP085_T_MEASURE        0x2E				// temperature measurent 
#define BMP085_P_MEASURE        0x34				// pressure measurement

#define BMP085_TEMP_CONVERSION_TIME  5				// TO be spec'd by GL or SB

#define SMD500_PARAM_MG      3038        //calibration parameter
#define SMD500_PARAM_MH     -7357        //calibration parameter
#define SMD500_PARAM_MI      3791        //calibration parameter

#define BMP085_ALTITUDE_RATIO		(1.00 / 5255.00)

void		bmp085_init(void);
void		bmp085_test(void);
bool		bmp085_ready(void);
bool		bmp085_writeAddress(uint8_t address);
bool		bmp085_readAddress(void);
bool		bmp085_write(uint8_t address, uint16_t data);
bool		bmp085_getDataw(uint16_t* data);
bool		bmp085_getDatab(uint8_t* data);

bool		bmp085_getTempUncompensated(int32_t* ut);
bool		bmp085_getPressureUncompensated(int32_t* up);

void		bmp085_calcTemp(int32_t* ut, int32_t* temp);
void		bmp085_calcPressure(int32_t* up, int32_t* pressure);
void		bmp085_calcAltitude(int32_t* pressure, float* altitude);

#endif /* BMP085_H_ */