#ifndef _PHY_TRAMPOLINE_C_
#define _PHY_TRAMPOLINE_C_

/*- Includes ---------------------------------------------------------------*/
#if defined(HAL_ATMEGA128RFA1)
	#include "phy/atmegarfa1/phy.c"
#elif defined(HAL_ATMEGA256RFR2)
	#include "phy/atmegarfr2/phy.c"
#else
	#error "Undefined Hardware Abstraction Layer (HAL)"
#endif

/*- Prototypes -------------------------------------------------------------*/

#endif // _PHY_TRAMPOLINE_C_