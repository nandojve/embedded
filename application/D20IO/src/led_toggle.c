#include <board.h>
#include <port.h>
#include <extint.h>
#include <extint_callback.h>
#include <system_interrupt.h>
#include <sleepmgr.h>
#include <usart.h>
#include <usart_interrupt.h>

/** \name LED0 definitions
 *  @{ */
#define LED1_PIN                  PIN_PA05
#define LED1_ACTIVE               false
#define LED1_INACTIVE             !LED1_ACTIVE
/** @} */
#define LED_1_NAME                "LED1 (yellow)"
#define LED_1_PIN                 LED1_PIN
#define LED_1_ACTIVE              LED1_ACTIVE
#define LED_1_INACTIVE            LED1_INACTIVE

/** \name SW0 definitions
 *  @{ */
#define SW1_PIN                   PIN_PA04
#define SW1_ACTIVE                false
#define SW1_INACTIVE              !SW1_ACTIVE
#define SW1_EIC_PIN               PIN_PA04A_EIC_EXTINT4
#define SW1_EIC_MUX               MUX_PA04A_EIC_EXTINT4
#define SW1_EIC_PINMUX            PINMUX_PA04A_EIC_EXTINT4
#define SW1_EIC_LINE              4

/**
 * \name Button #0 definitions
 *
 * Wrapper macros for SW0, to ensure common naming across all Xplained Pro
 * boards.
 *
 *  @{ */
#define BUTTON_1_NAME             "SW1"
#define BUTTON_1_PIN              SW1_PIN
#define BUTTON_1_ACTIVE           SW1_ACTIVE
#define BUTTON_1_INACTIVE         SW1_INACTIVE
#define BUTTON_1_EIC_PIN          SW1_EIC_PIN
#define BUTTON_1_EIC_MUX          SW1_EIC_MUX
#define BUTTON_1_EIC_PINMUX       SW1_EIC_PINMUX
#define BUTTON_1_EIC_LINE         SW1_EIC_LINE


/** If \true, interrupts are used to alter the board state, when \false polling
 *  is used.
 */
#define USE_INTERRUPTS   true

/** If \true, the External Interrupt Controller module is used to check when the
 *   button state changes, when \false the PORT module is used.
 */
#define USE_EIC          true

void usart_read_callback(const struct usart_module *const usart_module);
void usart_write_callback(const struct usart_module *const usart_module);

void configure_usart(void);
void configure_usart_callbacks(void);

struct usart_module usart_instance;
#define MAX_RX_BUFFER_LENGTH   32
volatile uint8_t rx_buffer[MAX_RX_BUFFER_LENGTH];


/** Updates the board LED to the current button state. */
static void update_led_state(void)
{
	bool pin_state = port_pin_get_input_level(BUTTON_1_PIN);
	port_pin_set_output_level(LED_1_PIN, pin_state);
}

#if USE_INTERRUPTS == true
#  if USE_EIC == true
/** Callback function for the EXTINT driver, called when an external interrupt
 *  detection occurs.
 *
 *  \param[in] channel  External Interrupt channel that has changed state
 */
static void extint_callback(uint32_t channel)
{
	update_led_state();
}

/** Configures and registers the External Interrupt callback function with the
 *  driver.
 */
static void configure_eic_callback(void)
{
	extint_register_callback(extint_callback, EXTINT_CALLBACK_TYPE_DETECT);
	extint_chan_enable_callback(BUTTON_1_EIC_LINE, EXTINT_CALLBACK_TYPE_DETECT);
}
#  else
/** Handler for the device SysTick module, called when the SysTick counter
 *  reaches the set period.
 *
 *  \note As this is a raw device interrupt, the function name is significant
 *        and must not be altered to ensure it is hooked into the device's
 *        vector table.
 */
void SysTick_Handler(void)
{
	update_led_state();
}

/** Configures the SysTick module to fire a SysTick interrupt every 999 system
 *  clock source cycles.
 */
static void configure_systick_handler(void)
{
	SysTick->CTRL = 0;
	SysTick->LOAD = 999;
	SysTick->VAL  = 0;
	SysTick->CTRL = SysTick_CTRL_CLKSOURCE_Msk | SysTick_CTRL_TICKINT_Msk | SysTick_CTRL_ENABLE_Msk;
}
#  endif
#endif

#if USE_EIC == true
/** Configures the External Interrupt Controller to detect changes in the board
 *  button state.
 */
static void configure_extint(void)
{
	struct extint_chan_conf eint_chan_conf;
	extint_chan_get_config_defaults(&eint_chan_conf);

	eint_chan_conf.gpio_pin           = BUTTON_1_EIC_PIN;
	eint_chan_conf.gpio_pin_mux       = BUTTON_1_EIC_MUX;
	eint_chan_conf.detection_criteria = EXTINT_DETECT_BOTH;
	extint_chan_set_config(BUTTON_1_EIC_LINE, &eint_chan_conf);

	extint_enable();
}
#endif

void configure_usart(void);

struct usart_module usart_instance;

void configure_usart(void)
{
	struct usart_config config_usart;

	usart_get_config_defaults(&config_usart);

	config_usart.baudrate    = 115200;
	config_usart.mux_setting = EDBG_CDC_SERCOM_MUX_SETTING;
	config_usart.pinmux_pad0 = EDBG_CDC_SERCOM_PINMUX_PAD0;
	config_usart.pinmux_pad1 = EDBG_CDC_SERCOM_PINMUX_PAD1;
	config_usart.pinmux_pad2 = EDBG_CDC_SERCOM_PINMUX_PAD2;
	config_usart.pinmux_pad3 = EDBG_CDC_SERCOM_PINMUX_PAD3;

	while (usart_init(&usart_instance, EDBG_CDC_MODULE, &config_usart) != STATUS_OK);

	usart_enable(&usart_instance);
}
int main(void)
{
	system_init();
	configure_usart();
	configure_usart_callbacks();
	
	struct port_config pin_conf;
	port_get_config_defaults(&pin_conf);

	/* Configure LEDs as outputs, turn them off */
	pin_conf.direction  = PORT_PIN_DIR_OUTPUT;
	port_pin_set_config(LED_1_PIN, &pin_conf);
	port_pin_set_output_level(LED_1_PIN, LED_1_INACTIVE);

	/* Set buttons as inputs */
	pin_conf.direction  = PORT_PIN_DIR_INPUT;
	pin_conf.input_pull = PORT_PIN_PULL_UP;
	port_pin_set_config(BUTTON_1_PIN, &pin_conf);


#if USE_EIC == true
	configure_extint();
#endif

#if USE_INTERRUPTS == true
#  if USE_EIC == false
	configure_systick_handler();
#  else
	configure_eic_callback();
#  endif

	system_interrupt_enable_global();

	uint16_t temp;
	while (true) {
		/* Do nothing - use interrupts */
		//if (usart_read_wait(&usart_instance, &temp) == STATUS_OK)
		//{
			//while (usart_write_wait(&usart_instance, temp) != STATUS_OK);
		//}
		usart_read_buffer_job(&usart_instance, (uint8_t *)rx_buffer, 1);
		//sleepmgr_sleep(SLEEPMGR_STANDBY);
	}
#else
#  if USE_EIC == false
	while (true) {
		update_led_state();
	}
#  else
	while (true) {
		if (extint_chan_is_detected(BUTTON_1_EIC_LINE)) {
			extint_chan_clear_detected(BUTTON_1_EIC_LINE);

			update_led_state();
		}
	}
#  endif
#endif
}


void configure_usart_callbacks(void)
{
	usart_register_callback(&usart_instance, usart_write_callback, USART_CALLBACK_BUFFER_TRANSMITTED);
	usart_register_callback(&usart_instance, usart_read_callback, USART_CALLBACK_BUFFER_RECEIVED);

	usart_enable_callback(&usart_instance, USART_CALLBACK_BUFFER_TRANSMITTED);
	usart_enable_callback(&usart_instance, USART_CALLBACK_BUFFER_RECEIVED);
}

void usart_read_callback(const struct usart_module *const usart_module)
{
	//usart_write_buffer_job(&usart_instance, (uint8_t *) rx_buffer, MAX_RX_BUFFER_LENGTH);
	usart_write_buffer_job(&usart_instance, (uint8_t *) rx_buffer, 1);
}

void usart_write_callback(const struct usart_module *const usart_module)
{
	port_pin_toggle_output_level(LED_0_PIN);
}
