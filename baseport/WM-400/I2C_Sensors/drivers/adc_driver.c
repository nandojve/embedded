/*
 * TEMT6000.c
 *
 * Created: 22/03/2014 11:21:48
 *  Author: nando
 */ 

#include "adc_driver.h"

volatile bool					g_uc_condone_flag			= false;			// Conversion done flag
uint16_t						g_adc_sample_data			= 0;				// Conversion value
struct adc_dev_inst				g_adc_inst					= NULL;				// ADC instance

static void adc_adcife_read_conv_result(void)
{
	if((adc_get_status(&g_adc_inst) & ADCIFE_SR_SEOC) == ADCIFE_SR_SEOC)
	{
		g_adc_sample_data									= adc_get_last_conv_value(&g_adc_inst);
		g_uc_condone_flag									= 1;

		adc_clear_status(&g_adc_inst, ADCIFE_SCR_SEOC);
	}
}

status_code_t adc_init(void)
{
	struct adc_config			adc_cfg						=
	{
		.prescal											= ADC_PRESCAL_DIV16,				// System clock division factor is 16
		.clksel												= ADC_CLKSEL_APBCLK,				// The APB clock is used
		.speed												= ADC_SPEED_75K,					// Max speed is 75K
		.refsel												= ADC_REFSEL_1,						// ADC Reference Voltage 0.625 * VCC
		.start_up											= CONFIG_ADC_STARTUP				// Enables the Startup time
	};
	struct adc_seq_config		adc_seq_cfg					=
	{
		.zoomrange											= ADC_ZOOMRANGE_0,					// Select Vref for shift cycle
		.muxneg												= ADC_MUXNEG_0,						// Pad Ground
		.muxpos												= ADC_MUXPOS_6,						// AD6 Channel
		.internal											= ADC_INTERNAL_0,					// Enables the primary/secondary voltage sources
		.gcomp												= ADC_GCOMP_DIS,					// Disables the ADC gain error reduction
		.hwla												= ADC_HWLA_DIS,						// Disables the HWLA mode
		.res												= ADC_RES_12_BIT,					// 12-bits resolution
		.bipolar											= ADC_BIPOLAR_SINGLEENDED			// Enables the single-ended mode
	};
	struct adc_ch_config		adc_ch_cfg					=
	{
		.seq_cfg											= &adc_seq_cfg,
		.internal_timer_max_count							= 60,								// Internal Timer Max Counter
		.window_mode										= 0,								// Window monitor mode is off
		.low_threshold										= 0,
		.high_threshold										= 0,
	};

	status_code_t				ret							= adc_init(&g_adc_inst, ADCIFE, &adc_cfg);

	if(ret != STATUS_OK)
	{
		return(ret);																			// ADC Init Fail!
	}
	
	ret														= adc_enable(&g_adc_inst);

	if(ret != STATUS_OK)
	{
		return(ret);																			// ADC Enable Fail!
	}

	adc_ch_set_config(&g_adc_inst, &adc_ch_cfg);
	adc_set_callback(&g_adc_inst, ADC_SEQ_SEOC, adc_adcife_read_conv_result, ADCIFE_IRQn, 1);

	adc_configure_trigger(&g_adc_inst, ADC_TRIG_SW);											// Configure trigger mode and start convention.
	adc_configure_gain(&g_adc_inst, ADC_GAIN_1X);												// Configure 1X GAIN
	
	return(STATUS_OK);
}
status_code_t adc_start(void)
{
	g_uc_condone_flag										= false;
	adc_start_software_conversion(&g_adc_inst);
	
	return(STATUS_OK);
}
status_code_t adc_getRawValue(uint16_t* data);
{
	status_code_t						ret					= g_uc_condone_flag ? STATUS_OK : OPERATION_IN_PROGRESS;

	g_uc_condone_flag										= false;
	*data													= g_adc_sample_data;

	return(ret);
}
float adc_evaluate(uint16_t data)
{
	return(0.0);
}