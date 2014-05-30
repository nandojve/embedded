/*
 * SimpleRemote.c
 *
 * Created: 28/05/2014 10:44:26
 *  Author: nando
 */ 

#ifdef __cplusplus
extern "C" {
#endif

#include "config.h"
#include "hal.h"
#include "phy.h"
#include "sys.h"
#include "nwk.h"
#include "nwkDataReq.h"
#include "halGpio.h"
#include "sysTimer.h"

/*- Definitions ------------------------------------------------------------*/
HAL_GPIO_PIN(LED0, E, 3);
HAL_GPIO_PIN(LED1, E, 4);
HAL_GPIO_PIN(LED2, E, 5);
HAL_GPIO_PIN(BUTTON, B, 6);

/*- Types ------------------------------------------------------------------*/
typedef enum AppState_t
{
	APP_STATE_INITIAL,
	APP_STATE_IDLE,
	APP_STATE_WAIT_CONF,
} AppState_t;

typedef struct AppMessage_t
{
	uint8_t buttonState;
} AppMessage_t;

/*- Variables --------------------------------------------------------------*/
static AppState_t appState = APP_STATE_INITIAL;
static AppMessage_t appMessage;
static NWK_DataReq_t appNwkDataReq;
static bool appButtonState = false;
static SYS_Timer_t blinkled;

static void blinkLedHandler(SYS_Timer_t *timer)
{
  HAL_GPIO_LED2_toggle();

  (void)timer;
}
void appDataConf(NWK_DataReq_t *req)
{
	appState = APP_STATE_IDLE;
	(void)req;
}
void appSendMessage(uint8_t state)
{
	appMessage.buttonState = state;

	appNwkDataReq.dstAddr = 1;
	appNwkDataReq.dstEndpoint = 1;
	appNwkDataReq.srcEndpoint = 1;
	appNwkDataReq.options = NWK_OPT_ACK_REQUEST;
	appNwkDataReq.data = (uint8_t *)&appMessage;
	appNwkDataReq.size = sizeof(appMessage);
	appNwkDataReq.confirm = appDataConf;

	NWK_DataReq(&appNwkDataReq);

	appState = APP_STATE_WAIT_CONF;
}
bool rx_frame(NWK_DataInd_t *ind)
{
	AppMessage_t *msg = (AppMessage_t *)ind->data;
		
	if (msg->buttonState)
		HAL_GPIO_LED1_set();
	else
		HAL_GPIO_LED1_clr();
		
	return true;
}
void app_task(void)
{
	switch (appState)
	{
		case APP_STATE_INITIAL:
		{
			// Hardware initialization
			HAL_GPIO_BUTTON_in();
			HAL_GPIO_BUTTON_pullup();
				
			HAL_GPIO_LED0_out();
			HAL_GPIO_LED0_clr();

			HAL_GPIO_LED1_out();
			HAL_GPIO_LED1_set();

			HAL_GPIO_LED2_out();
			HAL_GPIO_LED2_set();

			// Timer initialization
			blinkled.interval = 500;
			blinkled.mode = SYS_TIMER_PERIODIC_MODE;
			blinkled.handler = blinkLedHandler;
			SYS_TimerStart(&blinkled);
			
			appState = APP_STATE_IDLE;
			break;
		}
		case APP_STATE_IDLE:
		{
			if (appButtonState != HAL_GPIO_BUTTON_read())
			{
				appButtonState = HAL_GPIO_BUTTON_read();
				appSendMessage(appButtonState);
			}
			break;
		}
		case APP_STATE_WAIT_CONF:
		{
			break;
		}
	}
}

#ifdef __cplusplus
}
#endif
