#include "serial.h"

void serialSetup() {
  #ifdef DEBUG
  Serial.begin(115200);
  while (!Serial) {}
  Serial.println("Debug serial connection established.");
  #endif

  Serial1.begin(115200);
  while (!Serial1) {}
  LAYER1.setStream(&Serial1);
  LAYER1.setPacketHandler(&onPacketReceived);
  #ifdef DEBUG
  Serial.println("Layer 1 serial connection established.");
  #endif

  Serial2.begin(115200);
  while (!Serial2) {}
  LIDAR.setStream(Serial2);
  LIDAR.setPacketHandler(&onPacketReceived);
  #ifdef DEBUG
  Serial.println("LIDAR serial connection established.");
  #endif
}

// void processLayer1Serial() {
//     String identifier = LAYER1.readStringUntil(',');
//     if (identifier == "l") {
//         int angle = LAYER1.parseInt();
//         robot.move(1, angle, 0);
//         #ifdef DEBUG
//         Serial.print("move angle: ");
//         Serial.println(LAYER1.parseInt());
//         #endif
//     } else if (identifier == "b") {
//         ball.in_catchment = !ball.in_catchment;
//     }
// }

// void processImuSerial() {
//     String identifier = IMU.readStringUntil(',');
//     if (identifier == "x") {
//         robot.current_pose.bearing = IMU.parseInt();
//     }
// }

// void processTeensySerial() {
//     String identifier = TEENSY.readStringUntil(',');
// }