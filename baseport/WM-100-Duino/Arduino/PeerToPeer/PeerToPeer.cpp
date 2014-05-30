#include <Arduino.h>

#ifdef __cplusplus
extern "C" {
#endif

/*- Includes ---------------------------------------------------------------*/
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "config.h"
#include "hal.h"
#include "phy.h"
#include "sys.h"
#include "nwk.h"
#include "sysTimer.h"
#include <LwMesh.h>

/*- Definitions ------------------------------------------------------------*/

#define APP_FLUSH_TIMER_INTERVAL 20
#define APP_BUFFER_SIZE NWK_MAX_PAYLOAD_SIZE

/*- Types ------------------------------------------------------------------*/
typedef enum AppState_t
{
	APP_STATE_INITIAL,
	APP_STATE_IDLE,
	APP_STATE_SERIAL,
} AppState_t;

/*- Prototypes -------------------------------------------------------------*/
static void appSendData(void);

/*- Variables --------------------------------------------------------------*/
static AppState_t appState = APP_STATE_INITIAL;
static SYS_Timer_t appTimer;
static NWK_DataReq_t appDataReq;
static bool appDataReqBusy = false;
static uint8_t appDataReqBuffer[APP_BUFFER_SIZE];
static uint8_t appUartBuffer[APP_BUFFER_SIZE];
static uint8_t appUartBufferPtr = 0;

/*- Implementations --------------------------------------------------------*/

static void appDataConf(NWK_DataReq_t *req)
{
	appDataReqBusy = false;
	(void)req;
}
static void appSendData(void)
{
	if (appDataReqBusy || 0 == appUartBufferPtr)
	return;

	memcpy(appDataReqBuffer, appUartBuffer, appUartBufferPtr);

	appDataReq.dstAddr = 1;
	appDataReq.dstEndpoint = LwMeshDefaultEndPoint;
	appDataReq.srcEndpoint = LwMeshDefaultEndPoint;
	appDataReq.options = NWK_OPT_ACK_REQUEST;
	appDataReq.data = appDataReqBuffer;
	appDataReq.size = appUartBufferPtr;
	appDataReq.confirm = appDataConf;
	NWK_DataReq(&appDataReq);

	appUartBufferPtr = 0;
	appDataReqBusy = true;
}
static void appTimerHandler(SYS_Timer_t *timer)
{
	appSendData();
	(void)timer;
}
bool rx_frame(NWK_DataInd_t *ind)
{
	for (uint8_t i = 0; i < ind->size; i++)
	Serial.write(ind->data[i]);

	return true;
}
void app_task(void)
{
	switch (appState)
	{
		case APP_STATE_INITIAL:
		{
			appTimer.interval = APP_FLUSH_TIMER_INTERVAL;
			appTimer.mode = SYS_TIMER_INTERVAL_MODE;
			appTimer.handler = appTimerHandler;
				
			Serial.begin(115200);

			appState = APP_STATE_IDLE;
			break;
		}
		case APP_STATE_IDLE:
		{
			if(Serial.available())
				appState = APP_STATE_SERIAL;	
			break;
		}
		case APP_STATE_SERIAL:
		{
			while(Serial.available())
			{
				uint8_t byte = Serial.read();
					
				if (appUartBufferPtr == sizeof(appUartBuffer))
				appSendData();
					
				if (appUartBufferPtr < sizeof(appUartBuffer))
				appUartBuffer[appUartBufferPtr++] = byte;
			}
				
			SYS_TimerStop(&appTimer);
			SYS_TimerStart(&appTimer);
			
			appState = APP_STATE_IDLE;

			break;
		}
		default:
		break;
	}
}

#ifdef __cplusplus
}
#endif