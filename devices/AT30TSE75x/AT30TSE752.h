/*
 * AT30TSE752.h
 *
 * Created: 01/03/2014 09:58
 *  Author: Nando
 *  Device: AT30TSE752
 *
 *	Note 1:	Software Write Protect WILL NOT work because is hardware dependent and isn't implemented.
 *	Note 2:	Interrupt must be configured to interrupt alarm work properly.
 *	Note 3: This device is compliance with SMBus commands. So, interrupt is implemented with pin share.
 *		see, SMBus Alert or ARA command for interrupt.
 *		see, SMBus BUS Timeout and
 *		see SMBus general call
 */ 

#ifndef _AT30TSE752_H_
#define _AT30TSE752_H_

#include "AT24C0x.h"

typedef enum at30tse75x_registers_tag
{
				AT30TSE52X_TEMP_REG							= 0x00,
				AT30TSE52X_CONF_REG							= 0x01,
				AT30TSE52X_Tlow_REG							= 0x02,
				AT30TSE52X_Thigh_REG						= 0x03,
				AT30TSE52X_NVM_CONF_REG						= 0x11,
				AT30TSE52X_NVM_Tlow_REG						= 0x12,
				AT30TSE52X_NVM_Thigh_REG					= 0x13,
				AT30TSE52X_UNKNOWN_REG						= 0xFF,
} at30tse75x_registers_t;

typedef enum at30tse75x_command_tag
{
				AT30TSE52X_COPY_VOLATILE_REG_TO_NVM_REG		= 0x48,
				AT30TSE52X_COPY_NVM_REG_TO_VOLATILE_REG		= 0xB8,
} at30tse75x_command_t;

typedef enum at30tse75x_conversion_resolution_tag
{
				AT30TSE52X_9_BITS							= 0x00,		// Default	// 0.5°C	25ms conversion time
				AT30TSE52X_10_BITS							= 0x01,					// 0.25°C	50ms
				AT30TSE52X_11_BITS							= 0x02,					// 0.125°C	100ms
				AT30TSE52X_12_BITS							= 0x03,					// 0.0625°C	200ms
} at30tse75x_conversion_resolution_t;

typedef enum at30tse75x_fault_tolerance_queue_tag
{
				AT30TSE52X_ALARM_AFTER_1_FAULT				= 0x00,		// Default
				AT30TSE52X_ALARM_AFTER_2_CONSECUTIVE_FAULTS	= 0x01,
				AT30TSE52X_ALARM_AFTER_4_CONSECUTIVE_FAULTS	= 0x02,
				AT30TSE52X_ALARM_AFTER_6_CONSECUTIVE_FAULTS	= 0x03,
} at30tse75x_fault_tolerance_queue_t;

typedef struct at30tse75x_config_tag						// default is: 0x0000
{
	uint16_t	nonvolatile_registers_busy					: 1;
	uint16_t	reserved									: 7;
	uint16_t	shutdown_mode								: 1;
	uint16_t	alarm_thermostat_mode						: 1;
	uint16_t	alert_pin_polarity							: 1;	
	uint16_t	fault_tolerance_queue						: 2;
	uint16_t	conversion_resolution						: 2;
	uint16_t	on_shot_mode								: 1;
} at30tse75x_config_t;

typedef struct at30tse75x_nonvolatile_config_tag			// default is: 0x0000
{
	uint16_t	reserved_1									: 1;
	uint16_t	register_lock								: 1;
	uint16_t	register_lockdown							: 1;
	uint16_t	reserved_2									: 5;
	uint16_t	shutdown_mode								: 1;
	uint16_t	alarm_thermostat_mode						: 1;
	uint16_t	alert_pin_polarity							: 1;
	uint16_t	fault_tolerance_queue						: 2;
	uint16_t	conversion_resolution						: 2;
	uint16_t	reserved_3									: 1;
} at30tse75x_nonvolatile_config_t;

#define			at30tse752_read_byte(data)					at24c0x_read_byte(AT30TSE752_MEM_DEVICE, data)
#define			at30tse752_read_page(address, data)			at24c0x_read_page(AT30TSE752_MEM_DEVICE, address, data)
#define			at30tse752_write_byte(address, data)		at24c0x_write_byte(AT30TSE752_MEM_DEVICE, address, data)
#define			at30tse752_write_page(address, data)		at24c0x_write_page(AT30TSE752_MEM_DEVICE, address, data)

status_code_t	at30tse752_read_register(at30tse75x_registers_t reg, uint16_t* data);
status_code_t	at30tse752_write_register(at30tse75x_registers_t reg, uint16_t data);

float			at30tse752_itemp_to_ftemp(int16_t itemp);
bool			at30tse752_getConfig(at30tse75x_config_t* config);
bool			at30tse752_setConfig(at30tse75x_config_t* config);
bool			at30tse752_getNVMConfig(at30tse75x_nonvolatile_config_t* config);
bool			at30tse752_setNVMConfig(at30tse75x_nonvolatile_config_t* config);
bool			at30tse752_exec_cmd(at30tse75x_command_t cmd);
bool			at30tse752_read_limit_temp(bool isHigh, uint16_t* data);
bool			at30tse752_write_limit_temp(bool isHigh, uint16_t* data);

#endif /* _AT30TSE752_H_ */