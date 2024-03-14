#include "main.h"

PacketSerial Cam1Serial;
PacketSerial Cam2Serial;
PacketSerial BtSerial;
PacketSerial TeensySerial;

// Cam1RxDataUnion cam1_rx_data;
// Cam2RxDataUnion cam2_rx_data;
BtTxDataUnion bt_tx_data;
BtRxDataUnion bt_rx_data;
Teensy1TxDataUnion teensy_1_tx_data;
Teensy1RxDataUnion teensy_1_rx_data;

Robot robot;
// Ball ball;
// Strategy strategy;

void setup()
{
  // robot.setUpSerial();
  Serial1.begin(115200);
  while (!Serial1)
  {
  }

  Serial.begin(115200);
  while (!Serial)
  {
  }
  Serial.println("Debug serial connection established.");
}

void loop()
{
  // robot.updateSerial();

  // robot.sendSerial();
  if (Serial1.available())
  {
    Serial.println(Serial1.read());
  }
}
