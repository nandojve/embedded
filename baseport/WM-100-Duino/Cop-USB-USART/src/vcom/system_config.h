#ifndef SYSTEM_CONFIG_H_
#define SYSTEM_CONFIG_H_

#include <inttypes.h>

#define				DEFAULT_PDICLK		0x02		// 2MHz / 4 == 500Khz
#define				DEFAULT_TPICLK		0x00		// 125KHz
#define				DEFAULT_VERSION		0x0120
#define				DEFAULT_DRIVER		0x00		// JUNGO Driver

typedef struct SystemConfig_tag
{
	uint8_t			pdiclock;
	uint8_t			tpiclock;
	uint16_t		version;
	uint8_t			driver;
} SystemConfig;

void				system_config_init(void);
uint8_t				system_config_load(void);
void				system_config_save(char* command);
SystemConfig*		system_config_get(void);

#endif