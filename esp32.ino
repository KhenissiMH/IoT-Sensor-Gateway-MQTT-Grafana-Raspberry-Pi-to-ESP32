#include <WiFi.h>
#include <Adafruit_NeoPixel.h>
#define pin 8
#define numpixels 1 
const char *ssid = "wifi ssid";
const char *password = "wifipassword";

NetworkServer server(80);
Adafruit_NeoPixel pixels(numpixels, pin, NEO_GRB + NEO_KHZ800);

// State tracking
String ledMode = "";
int rgbStep = 0;
unsigned long lastRGBChange = 0;
const int RGB_INTERVAL = 500;

void setup() {
  Serial.begin(115200);
  pixels.begin();

  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nWiFi connected. IP: ");
  Serial.println(WiFi.localIP());
  server.begin();
}

void loop() {
  // Handle RGB cycling non-blocking
  if (ledMode == "RGB") {
    unsigned long now = millis();
    if (now - lastRGBChange >= RGB_INTERVAL) {
      lastRGBChange = now;
      if (rgbStep == 0)      pixels.setPixelColor(0, pixels.Color(250, 0, 0));
      else if (rgbStep == 1) pixels.setPixelColor(0, pixels.Color(0, 250, 0));
      else if (rgbStep == 2) pixels.setPixelColor(0, pixels.Color(0, 0, 250));
      pixels.show();
      rgbStep = (rgbStep + 1) % 3;
    }
  }

  NetworkClient client = server.accept();
  if (client) {
    Serial.println("New Client.");
    String currentLine = "";
    while (client.connected()) {
      if (client.available()) {
        char c = client.read();
        Serial.write(c);
        if (c == '\n') {
          if (currentLine.length() == 0) {
            client.println("HTTP/1.1 200 OK");
            client.println("Content-type:text/html");
            client.println();
            client.print("Click <a href=\"/R\">here</a> to turn the LED red.<br>");
            client.print("Click <a href=\"/G\">here</a> to turn the LED green.<br>");
            client.print("Click <a href=\"/B\">here</a> to turn the LED blue.<br>");
            client.print("Click <a href=\"/RGB\">here</a> to turn the LED RGB.<br>");
            client.println();
            break;
          } else {
            currentLine = "";
          }
        } else if (c != '\r') {
          currentLine += c;
        }

        if (currentLine.endsWith("GET /R")) {
          ledMode = "R";
          pixels.setPixelColor(0, pixels.Color(250, 0, 0));
          pixels.show();
        }
        if (currentLine.endsWith("GET /G")) {
          ledMode = "G";
          pixels.setPixelColor(0, pixels.Color(0, 250, 0));
          pixels.show();
        }
        if (currentLine.endsWith("GET /B")) {
          ledMode = "B";
          pixels.setPixelColor(0, pixels.Color(0, 0, 250));
          pixels.show();
        }
        if (currentLine.endsWith("GET /RGB")) {
          ledMode = "RGB";
          rgbStep = 0;
          lastRGBChange = millis();
        }
      }
    }
    client.stop();
    Serial.println("Client Disconnected.");
  }
}
