#include "main.h"

void serialSetup() {
  #ifdef DEBUG
  Serial.begin(115200);
  while (!Serial) {}
  Serial.println("Debug serial connection established.");
  #endif

  Serial1.begin(115200);
  while (!Serial1) {}
  Cam1Serial.setStream(&Serial1);
  Cam1Serial.setPacketHandler(&onPacketReceived);
  #ifdef DEBUG
  Serial.println("Layer 1 serial connection established.");
  #endif

  Serial2.begin(115200);
  while (!Serial2) {}
  Cam2Serial.setStream(Serial2);
  Cam2Serial.setPacketHandler(&onPacketReceived);
  #ifdef DEBUG
  Serial.println("IMU serial connection established.");
  #endif

  Serial4.begin(115200);
  while (!Serial4) {}
  BtSerial.setStream(Serial4);
  BtSerial.setPacketHandler(&onPacketReceived);
  #ifdef DEBUG
  Serial.println("Teensy serial connection established.");
  #endif

  Serial5.begin(115200);
  while (!Serial5) {}
  TeensySerial.setStream(Serial5);
  TeensySerial.setPacketHandler(&onPacketReceived);
  #ifdef DEBUG
  Serial.println("Teensy serial connection established.");
  #endif
}

void onPacketReceived(const void* sender, const byte *buf, size_t size) {
  if (sender == &Cam1Serial) {
    Cam1TxDataUnion data_received;
    // Don't continue if the payload is invalid
    if (size != sizeof(data_received)) return;
    std::copy(buf, buf + size, std::begin(data_received.bytes));
  } else if (sender == &Cam2Serial) {
    Cam2TxDataUnion data_received;
    // Don't continue if the payload is invalid
    if (size != sizeof(data_received)) return;
    std::copy(buf, buf + size, std::begin(data_received.bytes));
  } else if (sender == &BtSerial) {
    BtTxDataUnion data_received;
    // Don't continue if the payload is invalid
    if (size != sizeof(data_received)) return;
    std::copy(buf, buf + size, std::begin(data_received.bytes));
  } else if (sender == &TeensySerial) {
    Teensy1TxDataUnion data_received;
    // Don't continue if the payload is invalid
    if (size != sizeof(data_received)) return;
    std::copy(buf, buf + size, std::begin(data_received.bytes));
  }
}
