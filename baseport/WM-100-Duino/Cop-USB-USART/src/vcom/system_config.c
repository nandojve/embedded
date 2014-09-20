#include <stdio.h>
#include <avr/eeprom.h>
#include <nvm.h>

#include "system_config.h"

#if (VCOM_ENABLE == 1)

static SystemConfig	systemconfig;
uint8_t serial_number[22];

/* Non-Volatile Parameter Values for EEPROM storage */
static uint8_t	EEMEM EEPROM_PDICLOCK	= DEFAULT_PDICLK;
static uint8_t	EEMEM EEPROM_TPICLOCK	= DEFAULT_TPICLK;
static uint16_t	EEMEM EEPROM_VERSION	= DEFAULT_VERSION;
static uint8_t	EEMEM EEPROM_DRIVER		= DEFAULT_DRIVER;

void system_config_check(void);

void system_config_init(void)
{
	struct nvm_device_serial	device_serial;
	nvm_read_device_serial(&device_serial);

	for(uint8_t x = 0; x < sizeof(device_serial); x++)
		sprintf((char *) serial_number + (2 * x), "%02X", device_serial.byte[x]);

	nvm_eeprom_read_buffer((eeprom_addr_t) &EEPROM_PDICLOCK, &(systemconfig.pdiclock), sizeof(EEPROM_PDICLOCK));
	nvm_eeprom_read_buffer((eeprom_addr_t) &EEPROM_TPICLOCK, &(systemconfig.tpiclock), sizeof(EEPROM_TPICLOCK));
	nvm_eeprom_read_buffer((eeprom_addr_t) &EEPROM_VERSION, &(systemconfig.version), sizeof(EEPROM_VERSION));
	nvm_eeprom_read_buffer((eeprom_addr_t) &EEPROM_DRIVER, &(systemconfig.driver), sizeof(EEPROM_DRIVER));
	
	system_config_check();
}
void system_config_save(char* command)
{
	UNUSED(command);
	
	system_config_check();
	
	nvm_eeprom_erase_and_write_buffer((eeprom_addr_t) &EEPROM_PDICLOCK, &(systemconfig.pdiclock), sizeof(EEPROM_PDICLOCK));
	nvm_eeprom_erase_and_write_buffer((eeprom_addr_t) &EEPROM_TPICLOCK, &(systemconfig.tpiclock), sizeof(EEPROM_TPICLOCK));
	nvm_eeprom_erase_and_write_buffer((eeprom_addr_t) &EEPROM_VERSION, &(systemconfig.version), sizeof(EEPROM_VERSION));
	nvm_eeprom_erase_and_write_buffer((eeprom_addr_t) &EEPROM_DRIVER, &(systemconfig.driver), sizeof(EEPROM_DRIVER));
}
SystemConfig* system_config_get(void)
{
	return(&systemconfig);
}
void system_config_check(void)
{
	if(systemconfig.pdiclock > 6)
	systemconfig.pdiclock			= DEFAULT_PDICLK;
	
	if(systemconfig.tpiclock > 6)
	systemconfig.tpiclock			= DEFAULT_TPICLK;
	
	if(systemconfig.version < DEFAULT_VERSION)
	systemconfig.version			= DEFAULT_VERSION;
	
	if(systemconfig.driver > 1)
	systemconfig.driver				= DEFAULT_DRIVER;
}

#endif