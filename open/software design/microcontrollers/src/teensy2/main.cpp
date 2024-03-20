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
Ball ball;
// Strategy strategy;

void setup()
{
  robot.setUpSerial();
}

void loop()
{
  robot.updateSerial();
  robot.sendSerial();
}
