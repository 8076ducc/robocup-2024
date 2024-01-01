#include "main.h"

void onLayer1Received(const byte *buf, size_t size) {
  Layer1TxDataUnion data_received;
  // Don't continue if the payload is invalid
  if (size != sizeof(data_received)) return;
  
  std::copy(buf, buf + size, std::begin(data_received.bytes));
  robot.on_line = data_received.data.on_line;
  ball.in_catchment = data_received.data.ball_in_catchment;
}

void onImuReceived(const byte *buf, size_t size) {
  ImuTxDataUnion data_received;
  // Don't continue if the payload is invalid
  if (size != sizeof(data_received)) return;

  std::copy(buf, buf + size, std::begin(data_received.bytes));
  robot.current_pose.bearing = data_received.data.bearing;
}

void onTeensyReceived(const byte *buf, size_t size) {
  Teensy1RxDataUnion data_received;
  // Don't continue if the payload is invalid
  if (size != sizeof(data_received)) return;

  std::copy(buf, buf + size, std::begin(data_received.bytes));
}

void Robot::setUpSerial() {
  #ifdef DEBUG
  Serial.begin(115200);
  while (!Serial) {}
  Serial.println("Debug serial connection established.");
  #endif

  Serial1.begin(115200);
  while (!Serial1) {}
  Layer1Serial.setStream(&Serial1);
  Layer1Serial.setPacketHandler(&onLayer1Received);
  #ifdef DEBUG
  Serial.println("Layer 1 serial connection established.");
  #endif

  Serial3.begin(115200);
  while (!Serial3) {}
  ImuSerial.setStream(&Serial3);
  ImuSerial.setPacketHandler(&onImuReceived);
  #ifdef DEBUG
  Serial.println("IMU serial connection established.");
  #endif

  Serial5.begin(115200);
  while (!Serial5) {}
  TeensySerial.setStream(&Serial5);
  TeensySerial.setPacketHandler(&onTeensyReceived);
  #ifdef DEBUG
  Serial.println("Teensy serial connection established.");
  #endif
}
