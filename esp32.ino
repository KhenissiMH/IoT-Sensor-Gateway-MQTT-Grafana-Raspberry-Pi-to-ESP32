#include <Adafruit_NeoPixel.h>
#define pin 8
#define numpixels 1
Adafruit_NeoPixel pixels(numpixels, pin, NEO_GRB + NEO_KHZ800);
void setup() {
  pixels.begin(); 
}

void loop(){
  pixels.clear(); 
  pixels.setPixelColor(0, pixels.Color(250, 0, 0)); 
  pixels.show();   
  delay(100);
  pixels.setPixelColor(0, pixels.Color(50, 50, 0)); 
  pixels.show();  
  delay(100);
  pixels.setPixelColor(0, pixels.Color(0, 250, 0)); 
  pixels.show();  
  delay(100);
  pixels.setPixelColor(0, pixels.Color(0, 50, 50)); 
  pixels.show();  
  delay(100);
  pixels.setPixelColor(0, pixels.Color(0, 0, 250)); 
  pixels.show();  
  delay(100);
  pixels.setPixelColor(0, pixels.Color(50, 0, 50)); 
  pixels.show();  
  delay(100);

}




