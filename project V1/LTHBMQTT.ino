#include <Wire.h>
#include <BH1750.h>

#include <DHT.h>

#include <WiFi.h>
#include <PubSubClient.h>

const char* ssid = "TOPNET_64B7";        // must be 2.4GHz
const char* password = "9aJXAMkMz7";

const char* mqtt_server = "192.168.1.12";   // replace with your ipconfig IPv4
const int   mqtt_port   = 1883;
const char* device_id   = "esp32c6-gateway";

WiFiClient espClient;
PubSubClient client(espClient);

unsigned long lastPublish = 0;
const unsigned long publishInterval = 2000;



BH1750 lightMeter(0x23);

#define SDA_PIN 6
#define SCL_PIN 7

#define DHTPIN 10
#define DHTTYPE DHT11


DHT dht(DHTPIN, DHTTYPE);

void setup_wifi() {
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  Serial.print("Connecting to WiFi");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nWiFi connected: " + WiFi.localIP().toString());
}

void reconnect() {
  while (!client.connected()) {
    String clientId = String(device_id) + "-" + String(WiFi.macAddress());
    Serial.print("Connecting to MQTT...");
    if (client.connect(clientId.c_str())) {
      Serial.println("connected");
    } else {
      Serial.print("failed, rc=");
      Serial.println(client.state());
      delay(5000);
    }
  }
}

void setup() {

  Serial.begin(115200);

  dht.begin();

  while (!Serial) {}

  Wire.begin(SDA_PIN, SCL_PIN);
  delay(100); 

  if (lightMeter.begin(BH1750::CONTINUOUS_HIGH_RES_MODE)) {
    Serial.println("BH1750 initialized successfully");
  } else {
    Serial.println("Error initializing BH1750 - check wiring");
  }
  
  setup_wifi();
  client.setServer(mqtt_server, mqtt_port);
}



void loop() {

  if (!client.connected()) reconnect();
  client.loop();

  unsigned long now = millis();
  if (now - lastPublish >= publishInterval) {
    lastPublish = now;
  
    float lux = lightMeter.readLightLevel();
  

    float h = dht.readHumidity();
    float t = dht.readTemperature();

    if (isnan(h) || isnan(t)) {
      Serial.println(F("Failed to read from DHT sensor!"));
      return;
    }

    String payload = String(lux,2);
    client.publish("sensors/esp32c6/light", payload.c_str());
    Serial.println("Published Light lvl : " + payload + " Lux");

    payload = String(t,2);
    client.publish("sensors/esp32c6/temp", payload.c_str());
    Serial.println("Published temprature : " + payload + " °C");

    payload = String(h,2);
    client.publish("sensors/esp32c6/humidity", payload.c_str());
    Serial.println("Published humidity : " + payload + " %");
  }

}
