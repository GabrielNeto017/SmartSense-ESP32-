
#ifndef _DEFINITIONS_H
#define _DEFINITIONS_H


#include <Arduino.h>

void subscribeTo();
void FeedWatchdog(void * pvParameters);
void DHT11e(void * pvParameters);
void FitaLed(void * pvParameters);
void EntradasAnalogicas(void * pvParameters);
void Analisador(void * pvParameters);
void subs(void * pvParameters);
void subscriptionCallback(char* topic, byte* message, unsigned int length);

#endif
