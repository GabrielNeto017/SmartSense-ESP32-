
#ifndef _SO_H
#define _SO_H


#include <Arduino.h>
#include <definitions.h>
#include <soc/rtc_wdt.h>
#include <Pindefinitions.h>
#include <DHT.h>
#include <WiFiMQTTManager.h>
#include <WiFiManager.h>

void Wifi_init();
void FitaLed_Init();
void SO_Init();

void subscribeTo();
void subscriptionCallback(char* topic, byte* message, unsigned int length);

#endif