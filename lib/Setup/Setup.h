
#ifndef _SETUP_H
#define _SETUP_H

#include <Arduino.h>
#include <Pindefinitions.h>
#include <WiFiMQTTManager.h>
#include <WiFiManager.h>


void Setup_Init();
void Wifi_init();
void Analisador_Init();
void subscribeTo();
void subscriptionCallback(char* topic, byte* message, unsigned int length);


#endif