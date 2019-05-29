#include <SPI.h>

#define eyered 14
#define eyeblue 15

#define centerRed 16
#define centerblue 17

#define plug 18

int vol;

void setup() {
  pinMode(plug, INPUT_PULLUP);
  Serial.begin(9600);
  Serial.println("\r\nStart");
  delay(100);
  boot();
}

void loop() {
  basicBlink();
  eb();
//confuse();
}

void boot() {
  delay(5000);
  digitalWrite(eyeblue, HIGH);
  delay(100);
  digitalWrite(eyeblue, LOW);
  delay(100);
  digitalWrite(eyeblue, HIGH);
  delay(100);
  digitalWrite(eyeblue, LOW);
  delay(500);
}

void basicBlink() {
  digitalWrite(eyeblue, HIGH);
  digitalWrite(centerblue, HIGH);
  delay(40);
  vol = analogRead(7);
  if (vol > 550) {
    Serial.println(vol);
    digitalWrite(centerblue, HIGH);
    digitalWrite(eyeblue, LOW);
    delay(40);
  }
}

void eb() {
  if (!digitalRead(plug)) {
    while (1) {
      digitalWrite(centerRed, HIGH);
      digitalWrite(eyered, HIGH);
      digitalWrite(centerblue, LOW);
      digitalWrite(eyeblue, LOW);
      if (digitalRead(plug))  {
        digitalWrite(centerRed, LOW);
        digitalWrite(eyered, LOW);
        break;
      }
    }
  }
}

void confuse() {
  while (1) {
    digitalWrite(centerRed, HIGH);
    digitalWrite(eyered, LOW);
    delay(40);
    digitalWrite(centerRed, HIGH);
    digitalWrite(eyered, HIGH);
    delay(40);
  }
}

//    digitalWrite(centerblue,HIGH);
//    delay(100);
//    digitalWrite(centerRed,HIGH);
//    delay(100);
//    digitalWrite(eyeblue,HIGH);
//    delay(100);
//    digitalWrite(eyered,HIGH);
//    delay(100);
//
//    digitalWrite(centerblue,LOW);
//    digitalWrite(centerRed,LOW);
//    digitalWrite(eyeblue,LOW);
//    digitalWrite(eyered,LOW);
