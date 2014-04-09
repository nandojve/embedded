/* Prevent double inclusion */
#ifndef AVR_DRIVERS_H
#define AVR_DRIVERS_H

#define		TRUE	1
#define		FALSE	0

//_____  I N C L U D E S ___________________________________________________
#include "ac_driver.h"
#include "adc_driver.h"
#include "aes_driver.h"
#include "awex_driver.h"
#include "clksys_driver.h"
#ifdef DAC_DRIVER
#include "dac_driver.h"
#endif
#include "des_driver.h"
#include "dma_driver.h"
//#include "ebi_driver.h"
//#include "eeprom_driver.h"
#include "event_system_driver.h"
#include "hires_driver.h"
#include "ir_driver.h"
#include "pmic_driver.h"
#ifdef PORT_DRIVER
#include "port_driver.h"
#endif
#include "qdec_driver.h"
//#include "rtc32_driver.h"
#include "rtc_driver.h"
//#include "sp_driver.h"
#include "spi_driver.h"
#include "tc_driver.h"
#include "twi_master_driver.h"
#include "twi_slave_driver.h"
#include "usart_driver.h"
#include "vbat.h"
#include "wdt_driver.h"

//_____ M A C R O S ________________________________________________________


//_____ P R I V A T E    D E C L A R A T I O N _____________________________


//_____ D E F I N I T I O N ________________________________________________


//_____ D E C L A R A T I O N ______________________________________________
#ifdef __cplusplus
extern "C" {
#endif

void driver_init(void);

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif  /* AVR_DRIVERS_H */
/* EOF */