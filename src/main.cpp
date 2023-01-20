/*
* SMART SENSE
* Autor do projeto: Gabriel Luiz B. Neto
* 
*/
#include <WiFiMQTTManager.h>
#include <soc/rtc_wdt.h>
#include <definitions.h>
#include <Pindefinitions.h>
#include <Setup.h>
#include <SO.h>
#include <DHT.h>




void setup() {

  Setup_Init();
  //FitaLed_Init();
  Wifi_init();
  Analisador_Init();
  SO_Init();
   
}


void loop() {
  // check for reset button push, and reconnect to MQTT if necessary 
}
