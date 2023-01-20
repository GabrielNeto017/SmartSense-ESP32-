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


//-------Sensor de °C e Umidade-----------
#define DHTPIN 25
#define DHTTYPE DHT11 
DHT dht(DHTPIN, DHTTYPE);


//------Definições Tasks----------------
static uint8_t TaskCoreZero = 0;
static uint8_t TaskCoreOne = 1;



void setup() {

  Setup_Init();
  //FitaLed_Init();
  Wifi_init();
  dht.begin();
  Analisador_Init();
  SO_Init();
   
}


void loop() {
  // check for reset button push, and reconnect to MQTT if necessary 
}
