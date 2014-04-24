/*
 * USART_BBBS.c
 *
 * Created: 22/04/2014 22:20:12
 *  Author: nando
 */ 

#include <compiler.h>
#include <delay.h>
#include <board.h>
#include <ioport.h>
#include <led.h>
#include <sleepmgr.h>
#include <sysclk.h>
#include <serial.h>

/**
 * \brief Application entry point.
 *
 * \return Unused (ANSI-C compatibility).
 */
int main(void)
{
	//static uint8_t received_byte;
	uint8_t buffer[] = "Teste USART com BBBS\n";
	const usart_serial_options_t uart_serial_options =
	{
		.baudrate						= CONF_UART_BAUDRATE,
#ifdef CONF_UART_CHAR_LENGTH
		.charlength						= CONF_UART_CHAR_LENGTH,
#endif
		.paritytype						= CONF_UART_PARITY,
#ifdef CONF_UART_STOP_BITS
		.stopbits						= CONF_UART_STOP_BITS,
#endif
	};

 	/* Initialize the SAM system */
 	sysclk_init();
 	board_init();

	// Initialize usart driver in RS232 mode
	usart_serial_init(CONF_UART, &uart_serial_options);

	LED_On(LED0);

	for(;;)
	{
		// Get and echo a character forever
		//usart_serial_getchar(CONF_UART, &received_byte);
		LED_On(LED1);
		//usart_serial_putchar(CONF_UART, received_byte);
		for(int i = 0; i < sizeof(buffer); ++i)
			usart_serial_putchar(CONF_UART, buffer[i]);
		delay_ms(50);
		LED_Off(LED1);
		delay_ms(1000);
	}
}
