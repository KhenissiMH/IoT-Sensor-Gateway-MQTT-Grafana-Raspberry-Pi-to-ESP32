#ifndef FRTOS_H
#define FRTOS_H

#include <Arduino.h>
#include "THLP.h"
#include "brokerconnection.h"

extern SemaphoreHandle_t mqttMutex;
extern TaskHandle_t Task1Handle;
extern TaskHandle_t Task2Handle;

void publishSensorTemperatureHumidity(void *parameter);
void publishSensorLight(void *parameter);
void publish();
void PL();

#endif