

## Innehållsförteckning
1. [Introduktion](#Introduktion)
   1. [Programbibliotek](#programbibliotek)
   2. [Hårdvara](#hårdvara)
3. [Azure SDK exempel](#azure-sdk-exempel)
   1. [Utmatning](#utmatning)
   2. [Analys av koden](#analys-av-koden)
3. [WifiManager](#wifimanager)
4. [Slutlig kod](#slutlig-kod)



## Introduktion

Installera stöd för esp32-kontrollern i Arduino IDE

File > preferences > additional boards manager URLs:
`https://raw.githubusercontent.com/espressif/arduino-esp32/gh-pages/package_esp32_index.json`

Board manager > esp32 by Espressif Systems > Install

Select board: DOIT ESP32 DEVKIT V1

Välj också rätt COM-port.

### Programbibliotek


Installera de behövliga programbiblioteken:
1. Azure_SDK_for_C
2. textparser
3. WiFiManager

Dessa kan också direkt kopieras från mappen [här](../prod_code/ESP32/Lib).

### Hårdvara

Mer information om pinnar som kan användas på ESP32-mikrokontrollern kan ses [här](https://lastminuteengineers.com/esp32-pinout-reference/).

![](https://lastminuteengineers.b-cdn.net/wp-content/uploads/iot/ESP32-Pinout.png)



## Azure SDK exempel

Mappen för exempelkoden kan hittas [här](../examples/ESP32/Azure_IoT_Hub_ESP32_example/).


För att komma igång behöver inställningar ändras i filen
[iot_configs.h](../examples/ESP32/Azure_IoT_Hub_ESP32_example/iot_configs.h).
Konfigurera i likhet med detta:
``` h
// Wifi
#define IOT_CONFIG_WIFI_SSID "ASUS_2498A_5G" // your SSID
#define IOT_CONFIG_WIFI_PASSWORD "mySecretPassword" // your PASSWORD


#define IOT_CONFIG_IOTHUB_FQDN "BatteryCharger.azure-devices.net" // your Azure IoT host name
#define IOT_CONFIG_DEVICE_ID "ESP32" // your device ID

#define IOT_CONFIG_DEVICE_KEY "51b36rt34fgh35fjR83nFVD51GWkzl35Df26eG=" // your real device Key
```

Sedan är det bara att klicka på knappen Upload i Arduino IDE.
Det finns flera sätt att se meddelanden som skickas från ESP32:n. Till detta rekommenderas att använda tillägget Azure IoT Hub inuti Visual Studio code. Man behöver logga in på samma Microsoft-konto som är värd för Azure IoT Hub. Därifrån går det att ta emot och skicka data till ESP32-enheten.

Läs mer om tillägget för Visual Studio Code [här](https://github.com/Microsoft/vscode-azure-iot-toolkit/wiki).


### Utmatning

Vid rätt ändrade inställningar skickas dessa meddelanden från ESP32 till molnet.

``` shell
[IoTHubMonitor] [4:20:44 PM] Message received from [ESP32]:
{
  "msgCount": 0
}
[IoTHubMonitor] [4:20:46 PM] Message received from [ESP32]:
{
  "msgCount": 1
}
[IoTHubMonitor] [4:20:48 PM] Message received from [ESP32]:
{
  "msgCount": 2
}
```

### Analys av koden

Meddelanden kommer från denna del i koden inuti filen Azure_IoT_Hub_ESP32.ino.
``` c
static void getTelemetryPayload(az_span payload, az_span* out_payload)
{
  az_span original_payload = payload;

  payload = az_span_copy(payload, AZ_SPAN_FROM_STR("{ \"msgCount\": "));
  (void)az_span_u32toa(payload, telemetry_send_count++, &payload);
  payload = az_span_copy(payload, AZ_SPAN_FROM_STR(" }"));
  payload = az_span_copy_u8(payload, '\0');

  *out_payload = az_span_slice(
      original_payload, 0, az_span_size(original_payload) - az_span_size(payload) - 1);
}
```
Mer data som ska skickas kan läggas till på detta ställe i Json-format. Exempel hur detta gjorts kan hittas i ESP32-koden i mappen för produktion.
Stödda datatyper kan hittas [här](https://azuresdkdocs.blob.core.windows.net/$web/c/az_core/1.0.0-preview.5/az__span_8h.html#a025d7ace73f19807e6d7ad5e07931d54).

## WifiManager

Kom igång med en [instruktionsvideo](https://youtu.be/VnfX9YJbaU8).

.ino filen

``` C
  WiFi.mode(WIFI_STA); // explicitly set mode 
```

Ansluter till det sparade nätverket. Ifall inget nätverk konfigurerats kommer en access point skapas där man kan ansluta till ett önskat nätverk. Detta kommer sparas och behöver då endast ställas in en gång.

``` C
  while (WiFi.status() != WL_CONNECTED) // Not connected to wifi
  {
    wm.autoConnect("OnDemandAP"); 
    delay(500);
    Serial.print(".");
  }
```


Används endast för att återställa sparade nätverk i testningssyfte.
``` C
wm.resetSettings(); // For TESTING only
```


För att ändra nätverk startas OnDemandAP

``` C
void setupWifiConnection()  // open OnDemandAP
{
  WiFiManager wm;    

  Serial.println("Setting up access point");
  //reset settings - for testing
  //wm.resetSettings();

  // set configportal timeout
  wm.setConfigPortalTimeout(APtimeout);

  if (!wm.startConfigPortal("OnDemandAP")) {
    Serial.println("failed to connect and hit timeout");
    delay(3000);
    //reset and try again, or maybe put it to deep sleep
    ESP.restart();
    delay(5000);
    }
}
```


## Slutlig kod
- [Källkod](../prod_code/ESP32/)
- [Funktionsgraf](../instructions/pdf/ESP32_kod.pdf)
