#define DO 10
#define AO 11

void setup() {
  Serial.begin(115200);
  pinMode(DO, INPUT);
  pinMode(AO, INPUT);
}

void loop() {
  int p = digitalRead(DO);
  float FI = analogRead(AO);

  if (p == LOW) {
    Serial.println("Flame detected !!");
    Serial.print("Flame Intensity : ");
    Serial.println(FI);
  }
  else {
    Serial.println("No flame detected");
  }

  delay(2000);
}