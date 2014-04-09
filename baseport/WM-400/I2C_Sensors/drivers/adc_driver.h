/*
 * adc_driver.h
 *
 * Created: 22/03/2014 11:21:38
 *  Author: nando
 */ 

#ifndef ADC_DRIVER_H_
#define ADC_DRIVER_H_

#include <compiler.h>
#include <adcife.h>

status_code_t adc_init(void);
status_code_t adc_start(void);
status_code_t adc_getRawValue(uint16_t* data);
float adc_evaluate(uint16_t data);

#endif /* ADC_DRIVER_H_ */