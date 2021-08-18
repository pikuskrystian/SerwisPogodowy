# SerwisPogodowy
## Pobieranie danych z serwisu OpenWeatherMap
   Nalezy wejść na https://openweathermap.org/, założyc darmowe konto i wygenerowac API key który bedzie nam potrzebny do uzyskiwania danych pogodowych.
   Podłącz moduł WiFi do złącza mikroBUS na płycie prototypowej. 
   ![Algorithm schema](./images/SerwisPogodowySS1.png)
   
   Dodaj swój {API key} do linku w metodzie GET
   ```
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
   
   ```
### Technologies:
Project is created with:
* JSON
* Weather API
* C


```
#define AP_SSID "C111"
#define AP_PASSPHRASE "abcdefabcdef987654321"
QCOM_SSID g_ssid = {.ssid = (AP_SSID)};
QCOM_PASSPHRASE g_passphrase = {.passphrase = (AP_PASSPHRASE)};
WLAN_AUTH_MODE g_auth = WLAN_AUTH_WPA2_PSK;
WLAN_CRYPT_TYPE g_cipher = WLAN_CRYPT_AES_CRYPT;
```
