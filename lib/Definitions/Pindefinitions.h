
#ifndef _PINDEFINITIONS_H
#define _PINDEFINITIONS_H


#include <Arduino.h>

//-------Sensor de °C e Umidade-----------
#define DHTPIN 25
#define DHTTYPE DHT11 
DHT dht(DHTPIN, DHTTYPE);

//------------ Fita de Led----------------                                                                                               
#define DI0 34
#define DI1 14
#define DI2 15
#define DI3 35

#define DO0 4
#define DO1 16
#define DO2 17

//-------------- LEDS ---------------------
#define LED1 0
#define LED2 12
#define LED3 13

//------------Entradas Analógicas---------
#define AI0 36
#define AI1 39

//------------Botões Frontais-------------
#define SW1 2  //action_button (Modo led)
#define SW2 32 //reset_button (Button that will put device into Access Point mode to allow for re-entering WiFi and MQTT settings)

//--------------Analisador---------------
#define MCP_CS 5
#define MCP_SCK 18
#define MCP_MISO 19
#define MCP_MOSI 23

#define MCLR 13
#define G0_gain 26
#define G1_gain 27

#define QNT_AMOSTRAS        1920

//-------------Wifi Manager--------------
#define AP_PASSWORD "Smart4.0"

#endif


