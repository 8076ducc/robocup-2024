#include "main.h"
#include <Arduino.h>
float maxVoltage;
int maxIndex;

void setup() {
  Serial.begin(115200);
  while(!Serial);
  Serial.println("Serial started");
}

void loop() {
  // maxVoltage = 0;
  // maxIndex = 0;
  // for (int i = 0; i < 24; i = i + 1) {
  //   float voltage = analogRead(irPins[i]);
  //   if (voltage > maxVoltage) {
  //     maxVoltage = voltage;
  //     maxIndex = i;
  //   }
  // }
}

// float ballBearing; 
// float ballDist;
// ballBearing = (360 / 24) * maxIndex;
// ballDist = 1 / maxVoltage //tune constant

