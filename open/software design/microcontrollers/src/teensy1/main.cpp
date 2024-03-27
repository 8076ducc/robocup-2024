#include "main.h"

PacketSerial Layer1Serial;
PacketSerial ImuSerial;
PacketSerial TeensySerial;

Layer1RxDataUnion layer_1_rx_data;
ImuRxDataUnion imu_rx_data;
Teensy1TxDataUnion teensy_1_tx_data;

Robot robot;
Ball ball;
Goal yellow_goal;
Goal blue_goal;

unsigned long last_time = 0;
int real = 0;
int points = 0;

void setup()
{
  robot.dribbler.setUp();
  robot.base.setUp();
  robot.setUpSerial();

  robot.previous_pose.x = 0;
  robot.previous_pose.y = 0;
  robot.previous_pose.bearing = 0;

  last_time = millis();
}

void loop()
{
#ifdef DEBUG
#else
  layer_1_rx_data.data.kick = false;
  robot.updateSerial();
  robot.sendSerial();

  if (robot.on_line)
  {
    robot.base.move(0.6, robot.target_angle, 0);
  }
  else
  {
    if (ball.in_catchment)
    {
      robot.orbitScore();
    }
    else
    {
      robot.orbitToBall();
    }
    // robot.defendGoal();
    // robot.rotateToBall();
  }
#endif
}
