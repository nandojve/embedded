/**
 * \file WSNDemo.c
 *
 * \brief WSNDemo application implementation
 *
 * Copyright (C) 2014 Atmel Corporation. All rights reserved.
 *
 * \asf_license_start
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice,
 *    this list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 *
 * 3. The name of Atmel may not be used to endorse or promote products derived
 *    from this software without specific prior written permission.
 *
 * 4. This software may only be redistributed and used in connection with an
 *    Atmel microcontroller product.
 *
 * THIS SOFTWARE IS PROVIDED BY ATMEL "AS IS" AND ANY EXPRESS OR IMPLIED
 * WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NON-INFRINGEMENT ARE
 * EXPRESSLY AND SPECIFICALLY DISCLAIMED. IN NO EVENT SHALL ATMEL BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
 * STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN
 * ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 *
 * \asf_license_stop
 *
 *
 */

/*
 * Copyright (c) 2014, Atmel Corporation All rights reserved.
 *
 * Licensed under Atmel's Limited License Agreement --> EULA.txt
 */

/**
 * \mainpage
 * \section preface Preface
 * This is the reference manual for the WSN Demo Application Application
 * The WSNDemo application implements a typical wireless sensor network
 *scenario,
 * in which one central node collects the data from a network of sensors and
 *passes this data over a serial connection for further processing.
 * In the case of the WSNDemo this processing is performed by the WSNMonitor PC
 *application. The BitCloud® Quick Start Guide  provides a detailed description
 *of the WSNDemo application scenario, and instructions on how to use
 *WSNMonitor.
 *  However since BitCloud is a ZigBee® PRO stack, there are a few differences
 *in the protocol:
 * • Device types (Coordinator, Router and End Device) are simulated on the
 *application level; there is no such separation in Lightweight Mesh on the
 *stack level
 * • The value of the extended address field is set equal to the value of the
 *short address field
 * • For all frames, the LQI and RSSI fields are filled in by the coordinator
 *with the values of LQI and RSSI from the received frame. This means that nodes
 *that are not connected to the coordinator directly will have the same values
 *as the last node on the route to the coordinator
 * • Sensor data values are generated randomly on all platforms
 * • Sending data to the nodes on the network is not implemented and not
 *supported in this demo application
 */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "config.h"
#include "sys.h"
#include "phy.h"
#include "sys.h"
#include "nwk.h"
#include "sysTimer.h"
#if APP_ENDDEVICE
	#include "sleep_mgr.h"
#endif
#include "commands.h"
#if APP_COORDINATOR
	#include "sio2host.h"
#endif
#if SAMD || SAMR21
	#include "system.h"
#else
	#include "sysclk.h"
	#if (LED_COUNT > 0)
		#include "led.h"
	#endif
#endif
#include "board.h"
#include "wsndemo.h"

#include <twi_master.h>
#include <delay.h>
#include "VCNL40xx.h"
#ifdef PLATFORM_WM400
#include "AT30TSE752.h"
#endif
void start_iic(void);

#define HAL_USB_ENABLED 1

#if (HAL_USB_ENABLED == 1)
	#include <udi_cdc.h>
	#include "halUsb.h"
#endif

/*****************************************************************************
*****************************************************************************/

#define APP_CAPTION_SIZE  (sizeof(APP_CAPTION) - 1)

/*- Types ------------------------------------------------------------------*/
COMPILER_PACK_SET(1)
typedef struct  AppMessage_t {
	uint8_t commandId;
	uint8_t nodeType;
	uint64_t extAddr;
	uint16_t shortAddr;
	uint32_t softVersion;
	uint32_t channelMask;
	uint16_t panId;
	uint8_t workingChannel;
	uint16_t parentShortAddr;
	uint8_t lqi;
	int8_t rssi;

	struct {
		uint8_t type;
		uint8_t size;
		int32_t battery;
		int32_t temperature;
		int32_t light;
	} sensors;

	struct {
		uint8_t type;
		uint8_t size;
		char text[APP_CAPTION_SIZE];
	} caption;
} AppMessage_t;

typedef enum AppState_t {
	APP_STATE_INITIAL,
	APP_STATE_SEND,
	APP_STATE_WAIT_CONF,
	APP_STATE_SENDING_DONE,
	APP_STATE_WAIT_SEND_TIMER,
	APP_STATE_WAIT_COMMAND_TIMER,
	APP_STATE_PREPARE_TO_SLEEP,
	APP_STATE_SLEEP,
	APP_STATE_WAKEUP,
	APP_STATE_COLLECT,
} AppState_t;
COMPILER_PACK_RESET()
/*- Variables --------------------------------------------------------------*/
static AppState_t appState = APP_STATE_INITIAL;
static bool temp_collect_ok = false;
static float temp_collected = 0.0;
static bool vcnl_collect_ok = false;
static vcnl40xx_raw_data_t vcnl_collected = { .ambient_light = 0, .proximity = 0 };

#if APP_ROUTER || APP_ENDDEVICE
	static NWK_DataReq_t appNwkDataReq;
	static SYS_Timer_t appNetworkStatusTimer;
	static SYS_Timer_t appCommandWaitTimer;
	static bool appNetworkStatus;
#endif

static AppMessage_t appMsg;
static SYS_Timer_t appDataSendingTimer;
#define APP_COMMAND_PENDING 0x01

#if (HAL_USB_ENABLED == 1)
	static uint8_t sendbuffer[512];
	void UartBytesReceived(void);
#else
	#if APP_COORDINATOR
		static uint8_t rx_data[APP_RX_BUF_SIZE];
	#endif

	void UartBytesReceived(uint16_t bytes, uint8_t *byte );
#endif

/*- Implementations --------------------------------------------------------*/

#if APP_COORDINATOR

/*****************************************************************************
*****************************************************************************/
#if (HAL_USB_ENABLED == 1)
void UartBytesReceived(void)
{
	while(udi_cdc_is_rx_ready())
	{
		APP_CommandsByteReceived(udi_cdc_multi_getc(0));
	}
}
#else
void UartBytesReceived(uint16_t bytes, uint8_t *byte )
{
	for (uint16_t i = 0; i < bytes; i++) {
		APP_CommandsByteReceived(byte[i]);
	}
}
#endif

static void appUartSendMessage(uint8_t *data, uint8_t size)
{
	uint8_t cs = 0;

#if (HAL_USB_ENABLED == 1)
	uint16_t					count	= 0;
	sendbuffer[count++]					= 0x10;
	sendbuffer[count++]					= 0x02;

	for(uint8_t i = 0; i < size; i++)
	{
		if(data[i] == 0x10)
		{
			sendbuffer[count++]			= 0x10;
			cs							+= 0x10;
		}
	
		sendbuffer[count++]				= data[i];
		cs								+= data[i];
	}
	sendbuffer[count++]					= 0x10;
	sendbuffer[count++]					= 0x03;
	cs									+= 0x10 + 0x02 + 0x10 + 0x03;
	sendbuffer[count++]					= cs;

	if(udi_cdc_multi_is_tx_ready(0))
	{
		udi_cdc_multi_write_buf(0, sendbuffer, count);
	}
#else
	sio2host_putchar(0x10);
	sio2host_putchar(0x02);

	for (uint8_t i = 0; i < size; i++) {
		if (data[i] == 0x10) {
			sio2host_putchar(0x10);
			cs += 0x10;
		}

		sio2host_putchar(data[i]);
		cs += data[i];
	}

	sio2host_putchar(0x10);
	sio2host_putchar(0x03);
	cs += 0x10 + 0x02 + 0x10 + 0x03;

	sio2host_putchar(cs);
#endif
}

#endif

/*****************************************************************************
*****************************************************************************/
static bool appDataInd(NWK_DataInd_t *ind)
{
	AppMessage_t *msg = (AppMessage_t *)ind->data;
#if (LED_COUNT > 0)
	LED_Toggle(LED_DATA);
#endif
	msg->lqi = ind->lqi;
	msg->rssi = ind->rssi;
#if APP_COORDINATOR
	appUartSendMessage(ind->data, ind->size);

	if (APP_CommandsPending(ind->srcAddr)) {
		NWK_SetAckControl(APP_COMMAND_PENDING);
	}
#endif
	return true;
}

/*****************************************************************************
*****************************************************************************/
static void appDataSendingTimerHandler(SYS_Timer_t *timer)
{
	if (APP_STATE_WAIT_SEND_TIMER == appState) {
		appState = APP_STATE_COLLECT;
	} else {
		SYS_TimerStart(&appDataSendingTimer);
	}

	(void)timer;
}

#if APP_ROUTER || APP_ENDDEVICE

/*****************************************************************************
*****************************************************************************/
static void appNetworkStatusTimerHandler(SYS_Timer_t *timer)
{
#if (LED_COUNT > 0)
	LED_Toggle(LED_NETWORK);
#endif
	(void)timer;
}

/*************************************************************************//**
*****************************************************************************/
static void appCommandWaitTimerHandler(SYS_Timer_t *timer)
{
	appState = APP_STATE_SENDING_DONE;
	(void)timer;
}

#endif

/*****************************************************************************
*****************************************************************************/
#if APP_ROUTER || APP_ENDDEVICE
static void appDataConf(NWK_DataReq_t *req)
{
#if (LED_COUNT > 0)
	LED_Off(LED_DATA);
#endif

	if (NWK_SUCCESS_STATUS == req->status) {
		if (!appNetworkStatus) {
#if (LED_COUNT > 0)
			LED_On(LED_NETWORK);
#endif
			SYS_TimerStop(&appNetworkStatusTimer);
			appNetworkStatus = true;
		}
	} else {
		if (appNetworkStatus) {
#if (LED_COUNT > 0)
			LED_Off(LED_NETWORK);
#endif
			SYS_TimerStart(&appNetworkStatusTimer);
			appNetworkStatus = false;
		}
	}

	if (APP_COMMAND_PENDING == req->control) {
		SYS_TimerStart(&appCommandWaitTimer);
#if (LED_COUNT > 0)
		LED_Toggle(LED_NETWORK);
#endif
		appState = APP_STATE_WAIT_COMMAND_TIMER;
	} else {
		appState = APP_STATE_SENDING_DONE;
	}
}

#endif

/*****************************************************************************
*****************************************************************************/
static void appSendData(void)
{
#ifdef NWK_ENABLE_ROUTING
	appMsg.parentShortAddr = NWK_RouteNextHop(0, 0);
#else
	appMsg.parentShortAddr = 0;
#endif

  appMsg.sensors.battery     = rand() & 0xffff;
  appMsg.sensors.light       = rand() & 0xffff;

  if(temp_collect_ok)
	appMsg.sensors.temperature = temp_collected * 1000;
  else
	appMsg.sensors.temperature = 0;

  if(vcnl_collect_ok)
	appMsg.sensors.light = vcnl_collected.ambient_light * 250;	// (amb * 1000 * 0.25)  0.25 lux/count
  else
	  appMsg.sensors.light = 0;

#if APP_COORDINATOR
	appUartSendMessage((uint8_t *)&appMsg, sizeof(appMsg));
	SYS_TimerStart(&appDataSendingTimer);
	appState = APP_STATE_WAIT_SEND_TIMER;
#else
	appNwkDataReq.dstAddr = 0;
	appNwkDataReq.dstEndpoint = APP_ENDPOINT;
	appNwkDataReq.srcEndpoint = APP_ENDPOINT;
	appNwkDataReq.options = NWK_OPT_ACK_REQUEST | NWK_OPT_ENABLE_SECURITY;
	appNwkDataReq.data = (uint8_t *)&appMsg;
	appNwkDataReq.size = sizeof(appMsg);
	appNwkDataReq.confirm = appDataConf;
#if (LED_COUNT > 0)
	LED_On(LED_DATA);
#endif
	NWK_DataReq(&appNwkDataReq);

	appState = APP_STATE_WAIT_CONF;
#endif
}

/*************************************************************************//**
*****************************************************************************/
static void appInit(void)
{
	appMsg.commandId            = APP_COMMAND_ID_NETWORK_INFO;
	appMsg.nodeType             = APP_NODE_TYPE;
	appMsg.extAddr              = APP_ADDR;
	appMsg.shortAddr            = APP_ADDR;
	appMsg.softVersion          = 0x01010100;
	appMsg.channelMask          = (1L << APP_CHANNEL);
	appMsg.panId                = APP_PANID;
	appMsg.workingChannel       = APP_CHANNEL;
	appMsg.parentShortAddr      = 0;
	appMsg.lqi                  = 0;
	appMsg.rssi                 = 0;

	appMsg.sensors.type        = 1;
	appMsg.sensors.size        = sizeof(int32_t) * 3;
	appMsg.sensors.battery     = 0;
	appMsg.sensors.temperature = 0;
	appMsg.sensors.light       = 0;

	appMsg.caption.type         = 32;
	appMsg.caption.size         = APP_CAPTION_SIZE;
	memcpy(appMsg.caption.text, APP_CAPTION, APP_CAPTION_SIZE);

	NWK_SetAddr(APP_ADDR);
	NWK_SetPanId(APP_PANID);
	PHY_SetChannel(APP_CHANNEL);
#if (defined(PHY_AT86RF212B) || defined(PHY_AT86RF212))
	PHY_SetBand(APP_BAND);
	PHY_SetModulation(APP_MODULATION);
#endif
	PHY_SetRxState(true);

#ifdef NWK_ENABLE_SECURITY
	NWK_SetSecurityKey((uint8_t *)APP_SECURITY_KEY);
#endif

	NWK_OpenEndpoint(APP_ENDPOINT, appDataInd);

	appDataSendingTimer.interval = APP_SENDING_INTERVAL;
	appDataSendingTimer.mode = SYS_TIMER_INTERVAL_MODE;
	appDataSendingTimer.handler = appDataSendingTimerHandler;

#if APP_ROUTER || APP_ENDDEVICE
	appNetworkStatus = false;
	appNetworkStatusTimer.interval = 500;
	appNetworkStatusTimer.mode = SYS_TIMER_PERIODIC_MODE;
	appNetworkStatusTimer.handler = appNetworkStatusTimerHandler;
	SYS_TimerStart(&appNetworkStatusTimer);

	appCommandWaitTimer.interval = NWK_ACK_WAIT_TIME;
	appCommandWaitTimer.mode = SYS_TIMER_INTERVAL_MODE;
	appCommandWaitTimer.handler = appCommandWaitTimerHandler;
#else
#if (LED_COUNT > 0)
	LED_On(LED_NETWORK);
#endif
#endif

#ifdef PHY_ENABLE_RANDOM_NUMBER_GENERATOR
	srand(PHY_RandomReq());
#endif

	APP_CommandsInit();

	appState = APP_STATE_COLLECT;
}

/*************************************************************************//**
*****************************************************************************/
static void APP_TaskHandler(void)
{
	switch (appState) {
	case APP_STATE_INITIAL:
	{
		appInit();
	}
	break;

	case APP_STATE_SEND:
	{
		appSendData();
	}
	break;

	case APP_STATE_SENDING_DONE:
	{
#if APP_ENDDEVICE
		appState = APP_STATE_PREPARE_TO_SLEEP;
#else
		SYS_TimerStart(&appDataSendingTimer);
		appState = APP_STATE_WAIT_SEND_TIMER;
#endif
	}
	break;

#if APP_ENDDEVICE
	case APP_STATE_PREPARE_TO_SLEEP:
	{
		if (!NWK_Busy()) {
			NWK_SleepReq();
			appState = APP_STATE_SLEEP;
		}
	}
	break;

	case APP_STATE_SLEEP:
	{
		sm_sleep(APP_SENDING_INTERVAL / 1000);
		appState = APP_STATE_WAKEUP;
	}
	break;

	case APP_STATE_WAKEUP:
	{
		NWK_WakeupReq();

		/*
		 * #if (LED_COUNT > 0)
		 *    LED_On(LED_NETWORK);
		 #endif*/

		appState = APP_STATE_COLLECT;
	}
#endif

	case APP_STATE_COLLECT:
	{
		vcnl_collect_ok					= false;
		temp_collect_ok					= false;
		status_code_t		ret			= vcnl40xx_start_convertion();
		if(ret == STATUS_OK)
		{
			delay_ms(1);
			ret							= vcnl40xx_read_raw_data();
			if(ret == STATUS_OK)
			{
				vcnl40xx_get_raw_data(&vcnl_collected);
				vcnl_collect_ok			= true;
			}
		}		
		
#ifdef PLATFORM_WM400
		int16_t				itemp		= 0;
		ret								= at30tse752_read_register(AT30TSE52X_TEMP_REG, (uint16_t *) &itemp);

		if(ret == STATUS_OK)
		{
			temp_collected				= at30tse752_itemp_to_ftemp(itemp);
			temp_collect_ok				= true;
		}
#endif

		appState = APP_STATE_SEND;
	}
	break;

	default:
		break;
	}

#if (APP_COORDINATOR)
	#if (HAL_USB_ENABLED == 1)
		UartBytesReceived();
	#else
		uint16_t bytes;

		if ((bytes = sio2host_rx(rx_data, APP_RX_BUF_SIZE)) > 0)
		{
			UartBytesReceived(bytes, (uint8_t *)&rx_data);
		}
	#endif
#endif
}
/*****************************************************************************
*****************************************************************************/
void start_iic(void)
{
	static twi_options_t options		=
	{
		.speed							= TWIM_MASTER_SPEED,
	};

	twi_master_setup(TWIM_MASTER_PORT, &options);

#ifdef PLATFORM_WM400
	{
		at30tse75x_config_t	config;
		at30tse752_getConfig(&config);
			
		config.conversion_resolution = AT30TSE52X_12_BITS;
			
		at30tse752_setConfig(&config);
	}
#endif

	vcnl40xx_init();
}


/*****************************************************************************
*****************************************************************************/
int wsndemo_main(void)
{
	SYS_Init();
	start_iic();
#if APP_ENDDEVICE
	sm_init();
#endif
#if APP_COORDINATOR
	sio2host_init();
#endif
	cpu_irq_enable();
	while (1) {
		SYS_TaskHandler();
		APP_TaskHandler();
	}
}
