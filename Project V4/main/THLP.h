#ifndef THLP_H
#define THLP_H

#include <Arduino.h>
#include <Wire.h>
#include <BH1750.h>
#include <DHT.h>

#define SDA_PIN 6
#define SCL_PIN 7
#define DHTPIN 10
#define DHTTYPE DHT11

extern BH1750 lightMeter;
extern DHT dht;
extern char payload[8];

void SSU();

#endif