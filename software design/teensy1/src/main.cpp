#include "main.h"
#include "serial.h"

Robot robot;
Ball ball;

float degToRad(float deg) {
    return deg * M_PI / 180;
}

void setup() {
  pinMode(LED, OUTPUT);
  digitalWrite(LED, HIGH);

  LAYER1.begin(115200);
  while (!LAYER1) {}
  digitalWrite(LED, LOW);

  LIDAR.begin(115200);
  while (!LIDAR) {}
  digitalWrite(LED, HIGH);

  #ifdef DEBUG
  Serial.begin(115200);
  while (!Serial) {}
  Serial.println("Serial ready.");
  #endif
}

void loop() {
  if (LAYER1.available()) {
    processLayer1Serial();
  }
}