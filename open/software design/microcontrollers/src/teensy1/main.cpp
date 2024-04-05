#include "main.h"
#include "HardwareSerial.h"

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
  robot.base.setUp();
  robot.setUpSerial();
  pinModeFast(DRIBBLER_PWM, OUTPUT);
  pinModeFast(LIDAR_PWM, OUTPUT);

  robot.previous_pose.x = 0;
  robot.previous_pose.y = 0;
  robot.previous_pose.bearing = 0;

  last_time = millis();
  robot.dribbler.update();
  robot.dribbler.dribbling = true;
}

int ball_catchment = 0;

// #define DEBUG

void loop()
{
#ifdef DEBUG
  // robot.base.motorOut(1, 0.3 * robot.base.max_pwm);
  // robot.base.motorOut(2, 0.3 * robot.base.max_pwm);
  // robot.base.motorOut(3, -0.3 * robot.base.max_pwm);
  // robot.base.motorOut(4, -0.3 * robot.base.max_pwm);
  robot.base.move(0.3, 0, 0);
#else
  robot.updateSerial();
  robot.sendSerial();

  if (ball.in_catchment || (ball.detected && ball.distance_from_robot < 500))
  {
    robot.dribbler.dribbling = true;
  }
  else
  {
    robot.dribbler.dribbling = false;
  }

  if (ball.in_catchment)
  {
    // Serial.println("Ball in catchment");
    ++ball_catchment;
  }
  else
  {
    ball_catchment = 0;
  }

  double flick_angle = 0;

  if (ball_catchment > 100)
  {
    robot.target_pose = robot.current_pose.x - 910 > 0 ? Pose({1430, 560, 0}) : Pose({520, 560, 0});
    flick_angle = robot.current_pose.x - 910 > 0 ? 270 : 90;
    double dist_from_target = sqrt(pow(robot.current_pose.x - robot.target_pose.x, 2) + pow(robot.current_pose.y - robot.target_pose.y, 2));
    Serial.println(dist_from_target);
    if (dist_from_target < 400) {
      robot.task = 1;
    }
    else
      robot.task = 3;
  }
  else if (ball.detected)
  {
    if (ball.current_pose.y < 0)
    {
      robot.task = 2;
    }
    else
    {
      robot.task = 0;
    }
  }
  else
  {
    robot.task = 2;
  }

  // Serial.print("Task: ");
  // Serial.println(robot.task);

  double kp = 0.0013;
  double ki = 0.0;
  double kd = 0.005;

  switch (robot.task)
  {
  case 0:
    robot.orbitToBall();
    // robot.rotateToBall();
    break;
  case 1:
    kp = 0.01;
    robot.move_data.speed = 0;
    robot.move_data.target_bearing = flick_angle;
    robot.move_data.target_angle = 0;
    // robot.orbitScore();
    break;
  case 2:
    robot.defendGoal();
    break;
  case 3:
    kp = 0.0013;
    robot.moveToTargetPose();
    break;
  }

  // Serial.print("task: ");

  // Serial.println(robot.task);
  robot.dribbler.update();

  robot.base.move(robot.move_data.speed, robot.move_data.target_angle, robot.move_data.target_bearing, kp, ki, kd);

#endif
}
