#define pinpir 10

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  pinMode(pinpir, INPUT);
}

void loop() {
  int dtct = digitalRead(pinpir);
  if (dtct == HIGH){
    Serial.println("motion detected");
  }
  else{
    Serial.println("no motion detected");
  }
  delay(500); 
}