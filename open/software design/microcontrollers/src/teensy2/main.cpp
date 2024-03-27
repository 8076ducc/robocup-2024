#include "main.h"

PacketSerial Cam1Serial;
PacketSerial Cam2Serial;
PacketSerial BtSerial;
PacketSerial TeensySerial;

BtTxDataUnion bt_tx_data;
BtRxDataUnion bt_rx_data;
CamTxDataUnion cam_tx_data;
CamRxDataUnion cam_rx_data;
Teensy1TxDataUnion teensy_1_tx_data;
Teensy1RxDataUnion teensy_1_rx_data;

Robot robot;
Ball ball;
Goal goal;
// Strategy strategy;

void setup()
{
  robot.setUpSerial();
  robot.setUpLidar();
}

void loop()
{
  robot.updateSerial();
  robot.sendSerial();

  robot.processLidar();
}
