#include "main.h"

PacketSerial Layer1Serial;
PacketSerial ImuSerial;
PacketSerial TeensySerial;

Layer1RxDataUnion layer_1_rx_data;
ImuRxDataUnion imu_rx_data;
Teensy1TxDataUnion teensy_1_tx_data;

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
  lidarSetup();
  digitalWriteFast(LED, LOW);
}

void loop() {
  #ifdef DEBUG
  // if ((millis() / 5000) % 2 > 0) {
  //   robot.base.move(0.4, 0);
  // } else {
  //   robot.base.move(0.4, 180);
    
  // }
  processLidar();
  #else
  Layer1Serial.update();
  ImuSerial.update();
  TeensySerial.update();
  #endif
}