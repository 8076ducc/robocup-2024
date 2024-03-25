#include "main.h"

PacketSerial Layer1Serial;
PacketSerial ImuSerial;
PacketSerial TeensySerial;

Layer1RxDataUnion layer_1_rx_data;
ImuRxDataUnion imu_rx_data;
Teensy1TxDataUnion teensy_1_tx_data;

Robot robot;
Ball ball;

unsigned long last_time = 0;
int real = 0;
int points = 0;
int ldr_reading = ldr_threshold;
double chord_length = 0;
double line_centre = 15.5;

void setup()
{
  robot.setUpDribbler();

  pinMode(LED_BUILTIN, OUTPUT);
  robot.setUpSerial();
  robot.base.setUpMotors();

  robot.previous_pose.x = 0;
  robot.previous_pose.y = 0;

  lidarSetup();
  last_time = millis();
}

int goalie = 0;

void loop()
{
#ifdef DEBUG
// if ((millis() / 5000) % 2 > 0) {
//   robot.base.move(0.4, 0);
// } else {
//   robot.base.move(0.4, 180);

// }
#else
  robot.updateSerial();
  processLidar();
  robot.updateSerial();

  // teensy_1_tx_data.data.current_pose.x = robot.current_pose.x;
  // teensy_1_tx_data.data.current_pose.y = robot.current_pose.y;
  // teensy_1_tx_data.data.current_pose.bearing = robot.current_pose.bearing;

  // if (robot.on_line)
  // {
  //   Serial.println("On line");
  //   robot.base.move(0.7, robot.target_angle, 0);
  // }
  // else
  // {
  //   // double actual_angle_diff = robot.target_pose.bearing - robot.current_pose.bearing;

  //   // if (actual_angle_diff > 360)
  //   // {
  //   //   actual_angle_diff = actual_angle_diff - 360;
  //   // }

  //   // robot.base.move(0.3, actual_angle_diff, 0);
  //   // Serial.println(robot.target_pose.bearing);

  //   robot.moveToTargetPose();

  //   // robot.target_angle = 0;

  //   // if (millis() - last_time > 6000)
  //   // {
  //   //   if (points == 4)
  //   //   {
  //   //     points = 0;
  //   //     last_time = millis();
  //   //   }
  //   //   else
  //   //   {
  //   //     points += 1;
  //   //     last_time = millis();
  //   //   }
  //   // }

  //   // switch (points)
  //   // {
  //   // case 0:
  //   //   robot.target_pose.x = 1370;
  //   //   robot.target_pose.y = 800;
  //   //   break;

  //   // case 1:
  //   //   robot.target_pose.x = 450;
  //   //   robot.target_pose.y = 800;
  //   //   break;

  //   // case 2:
  //   //   robot.target_pose.x = 910;
  //   //   robot.target_pose.y = 1215;
  //   //   robot.target_angle = 45;
  //   //   break;

  //   // case 3:
  //   //   robot.target_pose.x = 450;
  //   //   robot.target_pose.y = 1630;
  //   //   break;

  //   // case 4:
  //   //   robot.target_pose.x = 1370;
  //   //   robot.target_pose.y = 1630;
  //   //   break;

  //   // default:
  //   //   break;
  //   // }
  //   // robot.moveToTargetPose();
  // }

  // if (millis() % 5000 <= 100)
  // {
  //   layer_1_rx_data.data.kick = true;
  // }
  // layer_1_rx_data.data.kick = false;

  double kp = -4;
  double correction;
  // chord_length = 8;
  // double correction = kp * (ldr_threshold - ldr_reading);

  if (robot.target_pose.x - robot.current_pose.x > 0)
  {
    correction = 90 + kp * (chord_length - 8);
  }
  else if (robot.target_pose.x - robot.current_pose.x < 0)
  {
    correction = 270 - kp * (chord_length - 8);
  }
  else
  {
    correction = 0;
  }

  switch (goalie)
  {
  case 0:
    correction = 270 - kp * (chord_length - 8);
    break;
  case 1:
    correction = 90 + kp * (chord_length - 8);
    break;
  }

  if (correction < 0)
  {
    correction = correction + 360;
  }
  else if (correction > 360)
  {
    correction = correction - 360;
  }

  double angular_corr = robot.current_pose.bearing + (14.5 - line_centre) * (360 / 32);

  if (angular_corr > 360)
  {
    angular_corr = angular_corr - 360;
  }
  else if (angular_corr < 0)
  {
    angular_corr = angular_corr + 360;
  }

  // Serial.print(line_centre);
  Serial.print("correction ");
  Serial.println(correction);

  // robot.base.move(max(0.0009 * abs(robot.target_pose.x - robot.current_pose.x), 0.1), correction, angular_corr);
  double speed = 0.2;

  if (robot.current_pose.bearing > 70 && robot.current_pose.bearing < 180)
  {
    if (correction < 180)
    {
      speed = 0;
      angular_corr = robot.current_pose.bearing;
      switch (goalie)
      {
      case 0:
        goalie = 1;
        break;
      case 1:
        goalie = 0;
        break;
      }
    }
    else if (correction > 180)
      ;
  }
  else if (robot.current_pose.bearing > 180 && robot.current_pose.bearing < 290)
  {
    if (correction > 180)
    {
      speed = 0;
      angular_corr = robot.current_pose.bearing;
      switch (goalie)
      {
      case 0:
        goalie = 1;
        break;
      case 1:
        goalie = 0;
        break;
      }
    }
    else if (correction < 180)
      ;
  }

  robot.base.move(speed, correction, angular_corr);

  robot.updateSerial();
  robot.sendSerial();

#endif
}
