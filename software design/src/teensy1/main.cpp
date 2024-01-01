#include "main.h"

PacketSerial Layer1Serial;
PacketSerial ImuSerial;
PacketSerial TeensySerial;

Robot robot;
Ball ball;

float degToRad(float deg) {
    return deg * M_PI / 180;
}

void setup() {
  pinModeFast(LED, OUTPUT);
  digitalWriteFast(LED, HIGH);

  robot.setUpSerial();
  
  robot.base.setUpMotors();
  robot.setUpDribbler();

  pinMode(LIDAR_PWM, OUTPUT);
}

void loop() {
  Layer1Serial.update();
  ImuSerial.update();
  TeensySerial.update();
}