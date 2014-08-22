/*
 * CC2D33S.h
 *
 * Created: 17/11/2011 12:21:41
 *  Author: Nando
 *
 *	Note 1:	Device will accept enter in command mode ONLY at Power Up AND if t < 3ms if fast start is enable
 * else, t < 10ms, where t is pass time after Power Up.
 *	Note 2: Alarm pin was not connected, alarm will not work.
 *	Note 3: Ready pin was not connected, device must be checked by polling.
 */ 

#ifndef CC2D33S_H_
#define CC2D33S_H_

#define				ONE_BY_TWO_FOURTEEN	0.00006103515625	// 1.0 / 2 ^ 14

typedef enum cc2d33s_measure_mode_tag
{
	CC2D33S_SLEEP_OR_SINGLE_SHOT		= 0x00,				// Sleep Mode
	CC2D33S_AUTO_UPDATE					= 0x01,				// Auto Update/Conversion Mode
} cc2d33s_measure_mode_t;

typedef enum cc2d33s_status_tag
{
	CC2D33S_STATUS_VALID_DATA			= 0x00,				// Valid data: Data that has not been fetched since the last measurement cycle.
	CC2D33S_STATUS_STALED_DATA			= 0x01,				// Stale data: Data that has already been fetched since the last measurement cycle.
	CC2D33S_STATUS_COMMAND_MODE			= 0x10,				// Command Mode: The ChipCap 2 is in Command Mode.
	
	// Mask
	CC2D33S_STATUS_VALID_DATA_Mask		= 0xC0,
} cc2d33s_status_t;

typedef enum cc2d33s_command_tag
{
	// Basic
	CC2D33S_DATA_FETCH					= 0x00,				// Used to fetch data in any digital mode.
	CC2D33S_MEASUREMENT_REQUEST			= 0x01,				// Used to start measurements in Sleep Mode.
	// Advanced (Command Mode Only)
	CC2D33S_READ_EEPROM					= 0x16,
	CC2D33S_WRITE_EEPROM				= 0x56,
	CC2D33S_START_NORMAL_MODE			= 0x80,
	// Ultra Advanced (Must be send at most 3ms after Power Up in fast start, 10ms otherwise)
	CC2D33S_START_COMMAND_MODE			= 0x0A,
} cc2d33s_command_t;

typedef struct cc2d33s_raw_data_tag
{
	uint16_t					humidity;
	uint16_t					temperature;
} cc2d33s_raw_data_t;

typedef struct cc2d33s_conpensate_data_tag
{
	float						humidity;
	float						temperature;
} cc2d33s_conpensate_data_t;

typedef struct cc2d33s_eeprom_data_tag						// Defined (INTERNAL USE ONLY)
{
	uint16_t					pdm_clip_high;
	uint16_t					pdm_clip_low;
	uint16_t					alarm_high_on;
	uint16_t					alarm_high_off;
	uint16_t					alarm_low_on;
	uint16_t					alarm_low_off;
	uint16_t					custom_config;				// MUST NOT BE CHANGED (RISK OF LOSS DEVICE FOREVER)
	uint16_t					reserved;					// MUST NOT BE CHANGED (RISK OF LOSS DEVICE FOREVER)
	uint16_t					customer_id2;				// Do Not Use
	uint16_t					customer_id3;				// Do Not Use
} cc2d33s_eeprom_data_t;

status_code_t					cc2d33s_start_convertion(void);
status_code_t					cc2d33s_ready(void);
status_code_t					cc2d33s_read_raw_data(void);
void							cc2d33s_evaluate(cc2d33s_conpensate_data_t* data);
void							cc2d33s_get_raw_data(cc2d33s_raw_data_t* data);
void							cc2d33s_set_raw_data(cc2d33s_raw_data_t* data);

#endif /* CC2D33S_H_ */