/*
 * Cop_Sensors.c
 *
 * Created: 04/04/2014 21:18:49
 *  Author: nando
 */ 

#include <string.h>
#include <board.h>
#include <sysclk.h>
#include <delay.h>
#include <stdio_usb.h>
#include <ioport.h>
#include <sleepmgr.h>
#include "Terminal.h"
#include "AT24MAC602.h"
#include "VCNL40xx.h"
#include "MPL115A2.h"
#include "CC2D33S.h"
#include "conf_twim.h"

void start_iic(void);
void myterminal_process_command(TerminalState* state, char* command);
void app_task(TerminalState* state);

#define DEVICE_EEPROM		0x57

#define TS_TSE_LOOP			(TS_USER + 1)

int main(void)
{
	/* Initialize basic board support features.
	 * - Initialize system clock sources according to device-specific
	 *   configuration parameters supplied in a conf_clock.h file.
	 * - Set up GPIO and board-specific features using additional configuration
	 *   parameters, if any, specified in a conf_board.h file.
	 */
	sysclk_init();
	sleepmgr_init();
	board_init();
	
	// Initialize interrupt vector table support.
	irq_initialize_vectors();

	// Enable interrupts
	cpu_irq_enable();

	/* Call a local utility routine to initialize C-Library Standard I/O over
	 * a USB CDC protocol. Tunable parameters in a conf_usb.h file must be
	 * supplied to configure the USB device correctly.
	 */
	stdio_usb_init();

	start_iic();
	
	ioport_set_pin_level(SENSORS_MPL115A2_EN, IOPORT_PIN_LEVEL_LOW);	// Force MPL115A2 to save energy
	delay_ms(10);														// Wait wake-up and ready
	ioport_set_pin_level(SENSORS_MPL115A2_EN, IOPORT_PIN_LEVEL_HIGH);	// Force MPL115A2 to save energy
	mpl115a2_init();													// Get Coefficients to evaluate data
	ioport_set_pin_level(SENSORS_MPL115A2_EN, IOPORT_PIN_LEVEL_LOW);	// Force MPL115A2 to save energy

	vcnl40xx_init();
	
	delay_init(sysclk_get_cpu_hz());
	
	TerminalState*				state	= terminal_init(myterminal_process_command);

	LED_On(LED0);

	while(true)
	{
		terminal_task();
		app_task(state);
	}
}

void start_iic(void)
{
	static twi_options_t options		=
	{
		.speed							= CONF_TWIM_MASTER_SPEED,
		
	};

	twi_master_setup(CONF_TWIM_PORT, &options);
}

void myterminal_process_command(TerminalState* state, char* command)
{
	if(!strncmp(command, "read", 4))
	{
		if(!strncmp(GetCLParmStr(command, 1), "page", 4))
		{
			uint8_t				address	= atoi(GetCLParmStr(command, 2));
			uint8_t				buffer[16];
			
			for(uint8_t i = 0; i < 16; ++i)
				buffer[i]		= 0;
			
			if(at24c0x_read_page(DEVICE_EEPROM, address, buffer) == STATUS_OK)
			{
				for(uint8_t i = 0; i < 16; ++i)
					printf("%2X ", buffer[i]);

				printf("\n");
			}
		}
		else if(!strncmp(GetCLParmStr(command, 1), "byte", 4))
		{
			uint8_t				data;
			status_code_t		ret		= ERR_INVALID_ARG;
			
			//if(!strncmp(GetCLParmStr(command, 2), "mac", 3))
				//ret				= at24mac602_read_byte(&data);
			//else if(!strncmp(GetCLParmStr(command, 2), "tse", 3))
				//ret				= at30tse752_read_byte(&data);
			//else if(!strncmp(GetCLParmStr(command, 2), "c256", 4))
				//ret				= at24c0x_read_byte(0x54, &data);
			//else
				ret				= at24c0x_read_byte(DEVICE_EEPROM, &data);

			if(ret == STATUS_OK)
			{
				//printf("%s: 0x%02X\n", GetCLParmStr(command, 2), data);
				printf("0x%02X\n", data);
			}
		}
		else if(!strncmp(GetCLParmStr(command, 1), "mso2012", 7))
		{
			uint8_t				buffer[16];

			//if(at24c0x_read_page(0x0000, buffer))
			//{
				//printf("0x0000\n");
				//for(uint8_t i = 0; i < 16; ++i)
				//printf("%2X ", buffer[i]);
//
				//printf("\n");
			//}
			//if(at24c0x_read_page(0x0100, buffer))
			//{
				//printf("0x0100\n");
				//for(uint8_t i = 0; i < 16; ++i)
				//printf("%2X ", buffer[i]);
//
				//printf("\n");
			//}
			//if(at24c0x_read_page(0x0200, buffer))
			//{
				//printf("0x0200\n");
				//for(uint8_t i = 0; i < 16; ++i)
				//printf("%2X ", buffer[i]);
//
				//printf("\n");
			//}
			//if(at24c0x_read_page(0x0300, buffer))
			//{
				//printf("0x0300\n");
				//for(uint8_t i = 0; i < 16; ++i)
				//printf("%2X ", buffer[i]);
//
				//printf("\n");
			//}
		}
		else if(!strncmp(GetCLParmStr(command, 1), "sn", 2))
		{
			uint8_t				buffer[16];
			
			for(uint8_t i = 0; i < 16; ++i)
				buffer[i]		= 0;
			
			if(at24mac602_read_sn(buffer) == STATUS_OK)
			{
				for(uint8_t i = 0; i < 16; ++i)
				printf("%2X ", buffer[i]);

				printf("\n");
			}
		}
		else if(!strncmp(GetCLParmStr(command, 1), "eui", 3))
		{
			uint8_t				buffer[8];
			
			for(uint8_t i = 0; i < 8; ++i)
				buffer[i]		= 0;
			
			if(at24mac602_read_eui(buffer) == STATUS_OK)
			{
				for(uint8_t i = 0; i < 8; ++i)
					printf("%2X ", buffer[i]);

				printf("\n");
			}
		}
		else if(!strncmp(GetCLParmStr(command, 1), "temp", 4))
		{
			float				temp;
			
			//if(at30tse752_temperature(&temp) == STATUS_OK)
			//{
				//printf("%f\n", temp);
			//}
		}
		
		*state				= TS_INIT;
	}
	else if(!strncmp(command, "write", 5))
	{
/*		if(!strncmp_flash(GetCLParmStr(command, 1), "page", 3))
		{
			uint8_t	addr = atoi(GetCLParmStr(command, 2));
			uint8_t	data = atoi(GetCLParmStr(command, 3));
			if(mcp98243_eeprom_dataw(addr, data))
			{
				*state		= TS_INIT;
					
				return;
			}
		}
		else
*/
		if(!strncmp(GetCLParmStr(command, 1), "byte", 4))
		{
			uint16_t			addr	= atoi(GetCLParmStr(command, 2));
			uint8_t				data	= atoi(GetCLParmStr(command, 3));
			
			at24c0x_write_byte(DEVICE_EEPROM, addr, data);
		}
		else if(!strncmp(GetCLParmStr(command, 1), "mso2012", 7))
		{
			uint8_t				v1[16]		= { 0xFF, 0xFF, 0xFF, 0xFF, 'D', 'P', 'O', '2', 'C', 'O', 'M', 'P', '\0', 0xFF, 0xFF, 0xFF };
			uint8_t				v2[16]		= { 0xFF, 0xFF, 0xFF, 0xFF, 'D', 'P', 'O', '2', 'E', 'M', 'B', 'D', '\0', 0xFF, 0xFF, 0xFF };
			uint8_t				v3[16]		= { 0xFF, 0xFF, 0xFF, 0xFF, 'D', 'P', 'O', '2', 'A', 'U', 'T', 'O', '\0', 0xFF, 0xFF, 0xFF };
			uint8_t				v4[16]		= { 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF };

//			at24c0x_write_page(0x0000, v1);
			delay_ms(100);
//			at24c0x_write_page(0x0100, v2);
			delay_ms(100);
//			at24c0x_write_page(0x0200, v3);
			delay_ms(100);
//			at24c0x_write_page(0x0300, v4);
			delay_ms(100);
		}
		
		*state							= TS_INIT;
	}
	else if(!strncmp(command, "probe", 5))
	{
		if(!strncmp(GetCLParmStr(command, 1), "vcnl", 4))
		{
			uint8_t				data;
			if(vcnl40xx_probe(&data) == STATUS_OK)
			{
				if(data == 0x11)
					printf("VCNL4000\n");
				else if(data == 0x21)
					printf("VCNL4010\n");
				else
					printf("VCNLxxxx: 0x%02X\n", data);
			}
			else
			{
				printf("probe failed!\n");
			}
		}
		else if(!strncmp(GetCLParmStr(command, 1), "cc2", 3))
		{
			if(cc2d33s_start_convertion() == STATUS_OK)
			{
				while(cc2d33s_ready() != STATUS_OK)
					delay_ms(1);
					
				if(cc2d33s_read_raw_data() == STATUS_OK)
				{
					cc2d33s_conpensate_data_t	data;
					cc2d33s_evaluate(&data);
					printf("RH: %f\%, Temp.: %f°C\n", data.humidity, data.temperature);
				}
				else
				{
					printf("read data failed!\n");
				}
			}
			else
			{
				printf("probe failed!\n");
			}
		}
		
		*state							= TS_INIT;
	}
	else if(!strncmp(command, "do", 2))
	{
		*state							= TS_TSE_LOOP;
	}
}

void app_task(TerminalState* state)
{
	switch(*state)
	{
		case TS_TSE_LOOP:
		{
			LED_On(LED1);
			//if(terminal_wait_any_key())
			//{
				status_code_t		ret			= cc2d33s_start_convertion();
				
				if(ret == STATUS_OK)
				{
					while(cc2d33s_ready() != STATUS_OK)
						delay_ms(1);

					ret							= cc2d33s_read_raw_data();
					if(ret == STATUS_OK)
					{
						cc2d33s_conpensate_data_t	data;
						cc2d33s_evaluate(&data);
						printf("CC2D33S Relative Humidity: %f %%, Temp.: %f °C\n", data.humidity, data.temperature);
					}
				}

				if(ret != STATUS_OK)
					printf("CC2D33S fail!\n");
				
				ioport_set_pin_level(SENSORS_MPL115A2_EN, IOPORT_PIN_LEVEL_HIGH);	// Enable MPL115A2
				delay_ms(5);											// Wait wake-up and ready

				ret								= mpl115a2_start_convertion();

				if(ret == STATUS_OK)
				{
					delay_ms(5);										// Wait conversion

					ret							= mpl115a2_read_raw_data();
//					ioport_set_pin_level(PIN_PA06, IOPORT_PIN_LEVEL_HIGH);	// Disable MPL115A2 until next round!
					
					if(ret == STATUS_OK)
					{
						mpl115a2_weather_data_t	data;
						//mpl115a2_conpensate_data_tag data;
						// REAL PROVE
						//mpl115a2_raw_data_t raw;
						//raw.pression			= 410;
						//raw.temperature			= 507;
						//
						//mpl115a2_set_raw_data(&raw);
						//
						//mpl115a2_coeffient_data_t coef;
						//
						//coef.a0					= 0x3ECE;
						//coef.b1					= 0xB3F9;
						//coef.b2					= 0xC517;
						//coef.c12				= 0x33C8;
						//
						//mpl115a2_set_coefficient(&coef);
						//
						// mpl115a2_evaluate(&data);
						
						mpl115a2_evaluate_weather_simple(&data);
						char* weather_type = "NaN";
						if(data.type & MPL115A2_WEATHER_TYPE_RAIN)
							weather_type = "Rain";
						else
						{
							if(data.type & MPL115A2_WEATHER_TYPE_CLOUD)
								weather_type = "Sun and Cloud";
							else
								weather_type = "Sun";
						}
						printf("MPL115A2 Pression: %f kPA, Altitude: %f m, Temp: %f °C, Weather: %s\n", data.pressure, data.altitude, data.temperature, weather_type);
					}
				}

				if(ret != STATUS_OK)
					printf("MPL115A2 fail!\n");
					
				ret								= vcnl40xx_start_convertion();
				if(ret == STATUS_OK)
				{
					delay_ms(1);
					ret							= vcnl40xx_read_raw_data();
					if(ret == STATUS_OK)
					{
						vcnl40xx_raw_data_t		data;
						
						vcnl40xx_get_raw_data(&data);

						printf("VCNL4010 Ambient Light: %f lux, Proximity: %d counts\n", ((float) data.ambient_light) * 0.25, data.proximity);
					}
				}
				
				if(ret != STATUS_OK)
					printf("VCNL4010 fail!\n");

				printf("\n");
				
				LED_Off(LED1);
				delay_ms(1000);
			//}
			//else
			//{
				//*state					= TS_INIT;
			//}
			//
			break;
		}
	}
}