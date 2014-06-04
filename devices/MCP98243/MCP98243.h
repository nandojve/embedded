/*
 * MCP98243.h
 *
 * Created: 16/11/2011 23:59:25
 *  Author: Nando
 */ 


#ifndef MCP98243_H_
#define MCP98243_H_

typedef enum MCP98243_SENSOR_COMMAND_enum
{
	MCP98243_SENSOR_CMD_CAPABILITY		= 0x00,
	MCP98243_SENSOR_CMD_CONFIGURATION	= 0x01,
	MCP98243_SENSOR_CMD_EVENT_UPPERTRIP	= 0x02,
	MCP98243_SENSOR_CMD_EVENT_LOWERTRIP	= 0x03,
	MCP98243_SENSOR_CMD_CRITICAL_TRIP	= 0x04,
	MCP98243_SENSOR_CMD_TEMPERATURE		= 0x05,
	MCP98243_SENSOR_CMD_MANUFACTURERID	= 0x06,
	MCP98243_SENSOR_CMD_DEVICEID_REV	= 0x07,
	MCP98243_SENSOR_CMD_RESOLUTIN		= 0x08,
} MCP98243_SENSOR_COMMAND_t;

void		mcp98243_init(void);
bool		mcp98243_ioctl(uint8_t command);
bool		mcp89243_ready(void);
bool		mcp89243_writeAddress(uint8_t address);
bool		mcp89243_readAddress(void);
bool		mcp98243_getDataw(uint16_t* data);
bool		mcp98243_getDatab(uint8_t* data);

bool		mcp98243_eeprom_addressw(uint8_t address);
bool		mcp98243_eeprom_datar(void);
bool		mcp98243_eeprom_dataw(uint8_t address, uint8_t data);

bool		mcp98243_getTemp(float* temp);

#endif /* MCP98243_H_ */