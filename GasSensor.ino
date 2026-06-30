
#define GDO 10
#define GAO 11
void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  pinMode(GDO, INPUT);
  pinMode(GAO, INPUT);

  Serial.println("Warming up the MQ2 sensor");
  delay(20000); 
}

void loop() {
  int GS = digitalRead(GDO);
  if (GS == HIGH){
    Serial.println("no gas concetration detected");
  }
  else{
    Serial.println("gas concetration detected");
  }
  float GM = analogRead(GAO);
  Serial.print("Gas measerment :");
  Serial.println(GM)  ;
  
  delay(2000);
}
