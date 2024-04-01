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
Goal yellow_goal;
Goal blue_goal;
Threads::Mutex lidar_data_lock;

void lidarThread()
{
  while (true)
  {
    robot.processLidar();
  }
}

void setup()
{
  robot.setUpSerial();
  robot.setUpLidar();
  threads.addThread(lidarThread);
}

void loop()
{
  robot.updateSerial();
  robot.sendSerial();

  teensy_1_rx_data.data.yellow_goal.current_pose = yellow_goal.current_pose;
  teensy_1_rx_data.data.blue_goal.current_pose = blue_goal.current_pose;

  robot.getRobotPose();
}
