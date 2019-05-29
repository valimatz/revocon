#include <Wire.h>
#include <PCA9685.h>
#include <IRremote.h>
#include "SoftwareSerial.h"
#include "DFRobotDFPlayerMini.h"

int khz = 38;
IRsend irsend;

#define muzl 5
#define flapLed 16
#define trigger 4
#define turnOnSW 6

#define SERVOMIN 150
#define SERVOMAX 600

const int readyLed = 5;

PCA9685 pwm = PCA9685(0x40);

SoftwareSerial mySoftwareSerial(14, 15); // RX, TX
DFRobotDFPlayerMini myDFPlayer;

void setup() {
  pwm.begin();
  pwm.setPWMFreq(60);

  pinMode(muzl, OUTPUT);
  pinMode(flapLed, OUTPUT);
  pinMode(trigger, INPUT_PULLUP);
  pinMode(turnOnSW, INPUT_PULLUP);

  mySoftwareSerial.begin(9600);
  if (!myDFPlayer.begin(mySoftwareSerial)) {
    while (true) {
      delay(0);
    }
  }
  myDFPlayer.volume(25);

  initializeFlap();

  myDFPlayer.playMp3Folder(1); //起動音
  delay(100);

  for (int i = 255; i > readyLed; i--) {
    analogWrite(muzl, i);
    delay(2);
  }
}

void loop() {
  normal();
}

void normal() {
  while (1) {
    analogWrite(muzl, readyLed);
    if (!digitalRead(trigger)) { //オープンフラップ
      openFlap();
      flapLedBlink();
      while (1) {
        analogWrite(muzl, readyLed);
        digitalWrite(flapLed, HIGH);
        if (!digitalRead(trigger)) { //発砲
          shoot();
          if (!digitalRead(turnOnSW)) {
            turnOn();
          } else {
            turnOff();
          }
          while (1) {
            analogWrite(muzl, readyLed);
            if (!digitalRead(trigger)) { //クローズフラップ
              flapLedBlink();
              closeFlap();
              goto out;
            }
          }
        }
      }
    }
  }
out:;
}

void servo_write(int ch, int ang) {
  ang = map(ang, 0, 180, SERVOMIN, SERVOMAX);
  pwm.setPWM(ch, 0, ang);
}

void initializeFlap() {
  for (int i = 0; i < 4; i++)
    servo_write(i, 1);
  for (int i = 4; i < 9; i++)
    servo_write(i, 1);
  delay(500);
}

void openFlap() {
  analogWrite(muzl, readyLed);
  myDFPlayer.playMp3Folder(2);
  for (int i = 0; i < 4; i++)
    servo_write(i, 179);
  for (int i = 4; i < 8; i++)
    servo_write(i, 150);
  delay(500);
  analogWrite(muzl, readyLed);
  delay(500);  
}

void closeFlap() {
  analogWrite(muzl, readyLed);
  delay(100);
  analogWrite(muzl, readyLed);
  myDFPlayer.playMp3Folder(2);
  for (int i = 0; i < 4; i++)
    servo_write(i, 1);
  for (int i = 4; i < 8; i++)
    servo_write(i, 1);
  delay(700);
}

void flapLedBlink() {
  myDFPlayer.playMp3Folder(3);
  for (int i = 0; i < 2; i++) {
    digitalWrite(flapLed, HIGH);
    analogWrite(muzl, readyLed);
    delay(70);
    digitalWrite(flapLed, LOW);
    analogWrite(muzl, readyLed);
    delay(70);
  }
}

void shoot() {
  myDFPlayer.playMp3Folder(4); //発砲音
  for (int i = 0; i < 3; i++) {
    analogWrite(muzl, 255);
    delay(50);
    analogWrite(muzl, 0);
    delay(50);
  }
}

void turnOn() {
  unsigned int  ON[85] = {1950, 950, 5450, 1000, 1500, 500, 1500, 450, 550, 450, 550, 450, 550, 450, 550, 450, 550, 500, 500, 500, 1500, 450, 550, 450, 550, 450, 550, 450, 550, 450, 550, 450, 550, 500, 1500, 450, 550, 450, 550, 450, 1550, 450, 500, 500, 500, 500, 550, 450, 550, 450, 550, 450, 550, 450, 550, 450, 550, 450, 550, 450, 550, 450, 550, 450, 550, 450, 550, 450, 550, 450, 550, 450, 1550, 450, 1550, 450, 1500, 500, 500, 500, 1500, 450, 1550, 450, 500};
  irsend.sendRaw(ON, sizeof(ON) / sizeof(ON[0]), khz);
  delay(1000);
}

void turnOff() {
  unsigned int  OFF[85] = {1950, 950, 5450, 1000, 1500, 450, 1550, 450, 550, 450, 550, 450, 550, 450, 550, 450, 550, 450, 550, 450, 1550, 450, 500, 500, 500, 500, 550, 450, 550, 450, 550, 450, 550, 450, 1500, 500, 1500, 500, 500, 450, 550, 500, 500, 500, 500, 500, 500, 500, 500, 500, 500, 500, 500, 500, 500, 500, 500, 500, 500, 500, 500, 500, 500, 500, 500, 500, 500, 500, 1500, 500, 500, 500, 500, 500, 1500, 450, 1550, 450, 550, 450, 1550, 450, 1500, 450, 550}; // UNKNOWN A9BD2C6E
  irsend.sendRaw(OFF, sizeof(OFF) / sizeof(OFF[0]), khz);
  delay(1000);
}
