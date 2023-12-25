#include "main.h"
#include "motion.h"
#include "serial.cpp"

void setup() {
  pinMode(LED, OUTPUT);
  digitalWrite(LED, HIGH);

  LAYER1.begin(9600);
  while (!LAYER1) {}
  digitalWrite(LED, LOW);

  LIDAR.begin(9600);
  while (!LIDAR) {}
  digitalWrite(LED, HIGH);

  #ifdef DEBUG
  Serial.begin(9600);
  while (!Serial) {}
  Serial.println("Serial ready.");
  #endif
}

void loop() {
  if (LAYER1.available()) {
    processLayer1Serial();
  }
}