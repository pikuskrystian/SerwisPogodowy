/*
 * Copyright 2016-2020 NXP
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 *
 * o Redistributions of source code must retain the above copyright notice, this list
 *   of conditions and the following disclaimer.
 *
 * o Redistributions in binary form must reproduce the above copyright notice, this
 *   list of conditions and the following disclaimer in the documentation and/or
 *   other materials provided with the distribution.
 *
 * o Neither the name of NXP Semiconductor, Inc. nor the names of its
 *   contributors may be used to endorse or promote products derived from this
 *   software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
 * ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

/**
 * @file    IoT_Lab.c
 * @brief   Application entry point.
 */
#include <stdio.h>
#include "board.h"
#include "peripherals.h"
#include "pin_mux.h"
#include "clock_config.h"
#include "LPC55S69_cm33_core0.h"
#include "fsl_debug_console.h"

#include "qcom_api.h"
#include "wlan_qcom.h"

#include "lcd.h"


// 0 is the highest priority and priority 15 is the lowest priority
const int TASK_MAIN_PRIO       = configMAX_PRIORITIES - 3;
const int TASK_MAIN_STACK_SIZE = 800;

portSTACK_TYPE *task_main_stack = NULL;
TaskHandle_t task_main_task_handler;

// Hardwired SSID, passphrase, auth and cipher algo of AP to connect to
#define AP_SSID       "C111"
#define AP_PASSPHRASE "abcdefabcdef987654321"

QCOM_SSID g_ssid             = {.ssid = (AP_SSID)};
QCOM_PASSPHRASE g_passphrase = {.passphrase = (AP_PASSPHRASE)};

WLAN_AUTH_MODE g_auth    = WLAN_AUTH_WPA2_PSK;
WLAN_CRYPT_TYPE g_cipher = WLAN_CRYPT_AES_CRYPT;

char recvData[2048]={0}; // HTTP Recive Buffer
char sbuff[256];

void http_head_parser(char *headData, char *parseVal, char* keyVal)
{
	char *pParse=0;
	int i;

	pParse=strstr(headData, keyVal);
	if(pParse){

		pParse+=strlen(keyVal);
		for(i=0; (pParse[i] != '\r') && (pParse[i] != '\n') ; i++)
			parseVal[i]=pParse[i];
		parseVal[i]=0;
	}
}

void task_main(void *param)
{
	int32_t result = 0;
	(void)result;

	/* Initialize WIFI shield */
	result = WIFISHIELD_Init();
	assert(A_OK == result);

	/* Initialize the WIFI driver (thus starting the driver task) */
	result = wlan_driver_start();
	assert(A_OK == result);

	LCD_Puts(10, 30, "Connecting to WiFi...", 0xFF00);
	LCD_GramRefresh();

	apScan();
	apConnect(&g_ssid, &g_passphrase, g_auth, g_cipher);
	getDhcp();

	LCD_Clear(0x0000);
	sprintf(sbuff, "WIFi: %s", AP_SSID);
	LCD_Puts(10, 30, sbuff, 0xFF00);
	LCD_GramRefresh();

	char vbuff[30]={0};

	while (1)
	{
		httpGet("api.openweathermap.org/data/2.5/weather?q=Tarnow,pl&APPID={APIKEY}", recvData);

		char *contentData=recvData;
		int recvLen=strlen(recvData);

		http_head_parser(recvData, vbuff, "Content-Length:");
		int contLen=atoi(vbuff);
		contentData=recvData+(recvLen-contLen);

		PRINTF("%s\r\n\r\n", contentData);
		vTaskDelay(MSEC_TO_TICK(5000));
	}
}
/*
 * @brief   Application entry point.
 */
int main(void) {

	BaseType_t result = 0;
	(void)result;

	/* Init board hardware. */
	BOARD_InitBootPins();
	BOARD_InitBootClocks();
	BOARD_InitBootPeripherals();
#ifndef BOARD_INIT_DEBUG_CONSOLE_PERIPHERAL
	/* Init FSL debug console. */
	BOARD_InitDebugConsole();
#endif

	LCD_Init(FLEXCOMM3_PERIPHERAL);
	LCD_Clear(0x0000);
	LCD_GramRefresh();

	result = xTaskCreate(task_main, "main", TASK_MAIN_STACK_SIZE, task_main_stack, TASK_MAIN_PRIO, &task_main_task_handler);
	assert(pdPASS == result);
	vTaskStartScheduler();

	while(1) {

	}
	return 0 ;
}
