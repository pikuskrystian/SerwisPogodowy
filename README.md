# SerwisPogodowy
## Pobieranie danych z serwisu OpenWeatherMap
   Nalezy wejść na https://openweathermap.org/, założyc darmowe konto i wygenerowac API key który bedzie nam potrzebny do uzyskiwania danych pogodowych.
   Podłącz moduł WiFi do złącza mikroBUS na płycie prototypowej. 
   ![Algorithm schema](./images/SerwisPogodowySS1.png)
   
   Dodaj swój {API key} do linku w metodzie GET
   ```

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

   
   ```
   Przykładowe dane pogodowe w formacie JSON
   ```
{
"coord":{"lon":20.987,"lat":50.0138},
"weather":[{"id":800,"main":"Clear","description":"clear sky","icon":"01d"}],
"base":"stations",
"main":{"temp":284.82,"feels_like":282.4,"temp_min":284.82,"temp_max":284.82,"pressure":1025,"humidity":51},
"visibility":10000,
"wind":{"speed":1.04,"deg":154},
"clouds":{"all":0},
"dt":1614786092,
"sys":{"type":3,"id":2021607, "country":"PL","sunrise":1614748525,"sunset":1614788452},
"timezone":3600,
"id":757026,
"name":"Tarnów",
"cod":200
}
```
### Technologies:
Project is created with:
* JSON
* Weather API
* C
* 


```
#define AP_SSID "C111"
#define AP_PASSPHRASE "abcdefabcdef987654321"
QCOM_SSID g_ssid = {.ssid = (AP_SSID)};
QCOM_PASSPHRASE g_passphrase = {.passphrase = (AP_PASSPHRASE)};
WLAN_AUTH_MODE g_auth = WLAN_AUTH_WPA2_PSK;
WLAN_CRYPT_TYPE g_cipher = WLAN_CRYPT_AES_CRYPT;
```
![xd](./images/guiserwispogodowy.png)
