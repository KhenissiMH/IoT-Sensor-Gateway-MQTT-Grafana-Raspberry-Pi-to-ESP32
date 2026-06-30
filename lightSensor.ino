#include <Wire.h>
#include <BH1750.h>

BH1750 lightMeter(0x23);

#define SDA_PIN 6
#define SCL_PIN 7

void setup() {
  Serial.begin(115200);
  while (!Serial) {}

  Wire.begin(SDA_PIN, SCL_PIN);
  delay(100); 

  if (lightMeter.begin(BH1750::CONTINUOUS_HIGH_RES_MODE)) {
    Serial.println("BH1750 initialized successfully");
  } else {
    Serial.println("Error initializing BH1750 - check wiring");
  }
}

void loop() {
  float lux = lightMeter.readLightLevel();
  Serial.print("Light: ");
  Serial.print(lux);
  Serial.println(" lx");
  delay(2000);
}