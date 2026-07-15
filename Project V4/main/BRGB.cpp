#include "BRGB.h"

bool lastState = LOW;

void BRSU(){
  pinMode(R, OUTPUT);
  pinMode(P, INPUT_PULLDOWN);
}

void BRL(){
  bool I = digitalRead(P);
  if (I == HIGH){
    lastState = !lastState;
    if (lastState == HIGH){
      digitalWrite(R, HIGH);
    } else {
      digitalWrite(R, LOW);
    }
  }
}