#ifndef _HAL_TRAMPOLINE_C_
#define _HAL_TRAMPOLINE_C_

/*- Includes ---------------------------------------------------------------*/
#if defined(HAL_ATMEGA128RFA1)
	#include "hal/atmega128rfa1/hal.c"
	#include "hal/atmega128rfa1/halSleep.c"
	#include "hal/atmega128rfa1/halTimer.c"
#elif defined(HAL_ATMEGA256RFR2)
	#include "hal/atmega256rfr2/hal.c"
	#include "hal/atmega256rfr2/halSleep.c"
	#include "hal/atmega256rfr2/halTimer.c"
#else
	#error "Undefined Hardware Abstraction Layer (HAL)"
#endif

/*- Prototypes -------------------------------------------------------------*/

#endif // _HAL_TRAMPOLINE_C_