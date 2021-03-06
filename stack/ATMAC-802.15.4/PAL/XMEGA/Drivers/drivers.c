//_____  I N C L U D E S ___________________________________________________
#include "drivers.h"

//_____ M A C R O S ________________________________________________________


//_____ P R I V A T E    D E C L A R A T I O N _____________________________


//_____ D E F I N I T I O N ________________________________________________


//_____ D E C L A R A T I O N ______________________________________________
void driver_init(void)
{
#ifdef ADC_DRIVER
	adc_init();
#endif
#ifdef DMA_DRIVER
	dma_init();
#endif
#ifdef EVENT_DRIVER
	event_init();
#endif
#ifdef TWI_MASTER_DRIVER
	twim_init();
#endif
}