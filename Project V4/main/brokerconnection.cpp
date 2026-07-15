#include "brokerconnection.h"

const char* ssid = "Amine";
const char* password = "@aA123456789";

const char* mqtt_server = "192.168.1.156";
const int   mqtt_port   = 1883;
const char* device_id   = "esp32c6-gateway";

WiFiClient espClient;
PubSubClient client(espClient);
String clientId = String(device_id) + "-" + String(WiFi.macAddress());

void ConnectWifi(){
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(5000);
    Serial.println("failed connection");
  }

  Serial.print("Connected to ");
  Serial.println(ssid);
  Serial.print("LocalIP :");
  Serial.println(WiFi.localIP());
}

void reconnect(){
  if (WiFi.status() == WL_CONNECTED){
    while(!client.connected()){
      Serial.print("Attempting MQTT connection...");
      if (client.connect(clientId.c_str())) {
        Serial.println("connected");
      } else {
        Serial.println("failed");
        delay(5000);
      }
    }
  }
}