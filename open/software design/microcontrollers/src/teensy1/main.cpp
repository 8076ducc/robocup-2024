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

int ball_catchment = 0;

// #define DEBUG

unsigned long time_ball_stopped;
double ball_last_dist;
bool was_goalie_now_striker;
bool is_goalie;

void striker()
{
  if (!ball.detected)
  {
    robot.task = 0;
  }
  else
  {
    if (ball.distance_from_robot > 100)
    {
      robot.task = 0;
    }
    else if (abs(ball.current_pose.bearing - blue_goal.current_pose.bearing) < 30)
    {
      robot.task = 1;
    }
    else
    {
      robot.task = 0;
    }

    if (ball.distance_from_robot > 500)
    {
      robot.dribbler.dribbling = false;
    }
    else
    {
      robot.dribbler.dribbling = true;
    }
  }
}

void soloGoalie()
{
  if (ball_last_dist - ball.distance_from_robot > 200)
  {
    time_ball_stopped = millis();
  }

  ball_last_dist = ball.distance_from_robot;

  // Serial.print("time ball stopped: ");
  // Serial.println(time_ball_stopped);

  if (millis() - time_ball_stopped > 1000 && ball.distance_from_robot < 500 && !was_goalie_now_striker)
  {
    robot.task = 0;
    was_goalie_now_striker = true;
  }
  // else if (ball.detected && ball.current_pose.y < 0)
  // {
  //   robot.task = 0;
  //   was_goalie_now_striker = true;
  // }
  else if (was_goalie_now_striker && robot.current_pose.y < 700)
  {
    robot.task = 0;
  }
  else
  {
    robot.task = 2;
    was_goalie_now_striker = false;
  }
}

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

void loop()
{
  robot.updateSerial();
  robot.sendSerial();

  if (ball.in_catchment || (ball.detected && ball.distance_from_robot < 1000))
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
  double kp = 0.0014;
  double ki = 0.0;
  double kd = 0.005;

  // if (ball_catchment > 100)
  // {
  //   robot.target_pose = robot.current_pose.x - 910 > 0 ? Pose({1430, 1785, 180}) : Pose({520, 1785, 180});
  //   flick_angle = robot.current_pose.x - 910 > 0 ? 270 : 90;
  //   kp = robot.current_pose.x - 910 > 0 ? 0.01 : 0.1;
  //   double dist_from_target = sqrt(pow(robot.current_pose.x - robot.target_pose.x, 2) + pow(robot.current_pose.y - robot.target_pose.y, 2));
  //   Serial.println(dist_from_target);
  //   if (dist_from_target < 400)
  //   {
  //     robot.task = 1;
  //   }
  //   else
  //     robot.task = 3;
  // }
  // else if (ball.detected)
  // {
  //   if (ball.current_pose.y < 0)
  //   {
  //     robot.task = 2;
  //   }
  //   else
  //   {
  //     robot.task = 0;
  //   }
  // }
  // else
  // {
  //   robot.task = 2;
  //

#ifndef BOT1
  // pure goalie code
  if (!robot.alliance_robot_detected)
  {
    if (robot.dip_1_on)
    {
      striker();
    }
    else
    {
      soloGoalie();
    }
  }
  else
  {
    if (robot.dip_2_on)
    {
      striker();
    }
    else
    {
      robot.task = 2;
    }
  }

#else
  // pure striker code
  striker();
  // if (!robot.alliance_robot_detected && ball.current_pose.y < 700)
  // {
  //   soloGoalie();
  // }
  // else
  // {
  //   striker();
  // }
#endif

  // robot.task = 2;
  // robot.task = 0;

  // if (is_goalie && ball.distance_from_robot < 500)
  // {
  //   robot.task = 0;
  //   is_goalie = false;
  // }
  // else if (!is_goalie && robot.current_pose.y < 1000)
  // {
  //   robot.task = 0;
  //   is_goalie = false;
  // }
  // else
  // {
  //   robot.task = 2;
  //   is_goalie = true;
  // }

  // Serial.print("task: ");
  // Serial.println(robot.task);

  switch (robot.task)
  {
  case 0:
    robot.orbitToBall(0);
    // robot.rotateToBall();
    break;
  case 1:
    // robot.move_data.speed = 0;
    // robot.move_data.target_bearing = flick_angle;
    // robot.move_data.target_angle = 0;
    robot.orbitScore();
    break;
  case 2:
    robot.defendGoal();
    break;
  case 3:
    kp = 0.0013;
    robot.moveToTargetPose();
    break;
  }

  // Serial.print("task");
  // Serial.println(robot.task);

  robot.dribbler.dribbling = true;
  robot.dribbler.update();

  robot.base.move(robot.move_data.speed, robot.move_data.target_angle, robot.move_data.target_bearing, kp, ki, kd);
  // robot.base.move(0.6, 0, 0, kp, ki, kd);
}
