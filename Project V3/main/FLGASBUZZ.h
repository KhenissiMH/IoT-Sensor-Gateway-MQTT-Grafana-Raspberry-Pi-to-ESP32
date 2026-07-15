#define GDO 8
#define DO 9
#define AO 11

void FGSU(){
  pinMode(GDO, INPUT);
  pinMode(DO, INPUT);
  pinMode(AO, OUTPUT);
  Serial.println("Warming up the MQ2 sensor");
  delay(20000);
}
void FGL(){
  int GS = digitalRead(GDO);
  int p = digitalRead(DO);
  //float FI = analogRead(AO);

  if (p == LOW || GS == LOW ) {
    if (p == LOW ){
      Serial.println("Flame detected !!");
      //Serial.print("Flame Intensity : ");
      //Serial.println(FI);
    }
    if (GS == LOW ){
      Serial.println("gas concetration detected");
      //float GM = analogRead(GAO);
      //Serial.print("Gas measerment :");
      //Serial.println(GM)  ;
    }
    digitalWrite(AO,HIGH);
  }
  if (p == HIGH && GS == HIGH){
    Serial.println("No flame detected");
    Serial.println("no gas concetration detected");
    digitalWrite(AO,LOW);
  }
}


