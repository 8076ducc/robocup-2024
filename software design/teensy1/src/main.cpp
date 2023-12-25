#include "main.h"
#include "serial.h"

Robot robot;
Ball ball;

float degToRad(float deg) {
    return deg * M_PI / 180;
}

void setup() {
  pinModeFast(LED, OUTPUT);
  digitalWriteFast(LED, HIGH);

  robot.setUpMotors();
  robot.setUpDribbler();

  pinMode(LIDAR_PWM, OUTPUT);

  #ifdef DEBUG
  Serial.begin(115200);
  while (!Serial) {}
  Serial.println("Debug serial connection established.");
  #endif

  LAYER1.begin(115200);
  while (!LAYER1) {}
  digitalWriteFast(LED, LOW);
  #ifdef DEBUG
  Serial.println("Layer 1 serial connection established.");
  #endif

  LIDAR.begin(115200);
  while (!LIDAR) {}
  digitalWriteFast(LED, HIGH);
  #ifdef DEBUG
  Serial.println("LIDAR serial connection established.");
  #endif
}

void loop() {
  if (LAYER1.available()) {
    processLayer1Serial();
  }
}