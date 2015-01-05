#include <twi_master.h>
#include "conf_board.h"
#include "conf_clock.h"
#include "conf_usb.h"
#include "conf_twim.h"

void start_iic(void);

/**
 * \brief main function
 */
int main (void)
{
	/* Initialize basic board support features.
	 * - Initialize system clock sources according to device-specific
	 *   configuration parameters supplied in a conf_clock.h file.
	 * - Set up GPIO and board-specific features using additional configuration
	 *   parameters, if any, specified in a conf_board.h file.
	 */
	sysclk_init();
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

	for(;;)
	{
		
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
