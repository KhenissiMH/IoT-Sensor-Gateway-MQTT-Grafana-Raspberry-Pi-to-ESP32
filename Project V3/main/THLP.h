#include <Wire.h>
#include <BH1750.h>

#include <DHT.h>


BH1750 lightMeter(0x23);
#define SDA_PIN 6
#define SCL_PIN 7

#define DHTPIN 10
#define DHTTYPE DHT11
DHT dht(DHTPIN, DHTTYPE);
char payload[8];


void SSU(){
  dht.begin();
  Wire.begin(SDA_PIN, SCL_PIN);
  delay(500); 
  if (lightMeter.begin(BH1750::CONTINUOUS_HIGH_RES_MODE)) {
    Serial.println("BH1750 works");
  }
  else {
    Serial.println("Error  (BH1750 - check wiring)");

  }
}