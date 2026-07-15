#ifndef BROKERCONNECTION_H
#define BROKERCONNECTION_H

#include <Arduino.h>
#include <WiFi.h>
#include <PubSubClient.h>

extern const char* ssid;
extern const char* password;

extern const char* mqtt_server;
extern const int   mqtt_port;
extern const char* device_id;

extern WiFiClient espClient;
extern PubSubClient client;
extern String clientId;

void ConnectWifi();
void reconnect();

#endif