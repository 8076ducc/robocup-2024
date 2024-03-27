#include "main.h"
#include <line.h>
#include <RPLidar.h>

RPLidar lidar;

Line front_wall, left_wall, back_wall, right_wall;

const double y_bounds[2] = {-1215, 1215};
const double x_bounds[2] = {-910, 910};

void Robot::setUpLidar()
{
  lidar.begin(Serial3);
}

int getClosestEdge(double x, double y)
{
  double distance_to_front, distance_to_left, distance_to_back, distance_to_right;

  if (x < x_bounds[0])
  {
    distance_to_front = sqrt(pow(x - x_bounds[0], 2) + pow(y - y_bounds[0], 2));
    distance_to_back = sqrt(pow(x - x_bounds[0], 2) + pow(y - y_bounds[1], 2));
  }
  else if (x > x_bounds[1])
  {
    distance_to_front = sqrt(pow(x - x_bounds[1], 2) + pow(y - y_bounds[0], 2));
    distance_to_back = sqrt(pow(x - x_bounds[1], 2) + pow(y - y_bounds[1], 2));
  }
  else
  {
    distance_to_front = abs(y - y_bounds[0]);
    distance_to_back = abs(y - y_bounds[1]);
  }

  if (y < y_bounds[0])
  {
    distance_to_left = sqrt(pow(x - x_bounds[0], 2) + pow(y - y_bounds[0], 2));
    distance_to_right = sqrt(pow(x - x_bounds[1], 2) + pow(y - y_bounds[0], 2));
  }
  else if (y > y_bounds[1])
  {
    distance_to_left = sqrt(pow(x - x_bounds[0], 2) + pow(y - y_bounds[1], 2));
    distance_to_right = sqrt(pow(x - x_bounds[1], 2) + pow(y - y_bounds[1], 2));
  }
  else
  {
    distance_to_left = abs(x - x_bounds[0]);
    distance_to_right = abs(x - x_bounds[1]);
  }

  // assign a number to the closest edge
  if (distance_to_front < distance_to_left && distance_to_front < distance_to_back && distance_to_front < distance_to_right)
  {
    return 0;
  }
  else if (distance_to_left < distance_to_front && distance_to_left < distance_to_back && distance_to_left < distance_to_right)
  {
    return 1;
  }
  else if (distance_to_back < distance_to_front && distance_to_back < distance_to_left && distance_to_back < distance_to_right)
  {
    return 2;
  }
  else
  {
    return 3;
  }
}

int lidar_loop_count = 0;

void Robot::getRobotPose()
{
  front_wall.horizontalLinearRegression();
  left_wall.verticalLinearRegression();
  back_wall.horizontalLinearRegression();
  right_wall.verticalLinearRegression();

  double A = front_wall.slope;
  double B = 1;
  double C = front_wall.intercept;

  double x1 = 0;
  double y1 = 0;

  double front_dist = abs(A * x1 + B * y1 + C) / sqrt(pow(A, 2) + pow(B, 2));

  A = 1;
  B = -left_wall.slope;
  C = -left_wall.intercept;

  x1 = 0;
  y1 = 0;

  double left_dist = abs(A * x1 + B * y1 + C) / sqrt(pow(A, 2) + pow(B, 2));

  A = back_wall.slope;
  B = 1;
  C = back_wall.intercept;

  x1 = 0;
  y1 = 0;

  double back_dist = abs(A * x1 + B * y1 + C) / sqrt(pow(A, 2) + pow(B, 2));

  A = 1;
  B = -right_wall.slope;
  C = -right_wall.intercept;

  x1 = 0;
  y1 = 0;

  double right_dist = abs(A * x1 + B * y1 + C) / sqrt(pow(A, 2) + pow(B, 2));

  // Serial.print("y = ");
  // Serial.print(front_wall.slope);
  // Serial.print("x + ");
  // Serial.println(front_wall.intercept);

  // Serial.print("x = ");
  // Serial.print(left_wall.slope);
  // Serial.print("y + ");
  // Serial.println(left_wall.intercept);

  // Serial.print("y = ");
  // Serial.print(back_wall.slope);
  // Serial.print("x + ");
  // Serial.println(back_wall.intercept);

  // Serial.print("x = ");
  // Serial.print(right_wall.slope);
  // Serial.print("y + ");
  // Serial.println(right_wall.intercept);

  // Serial.print(front_dist);
  // Serial.print(", ");
  // Serial.print(left_dist);
  // Serial.print(", ");
  // Serial.print(back_dist);
  // Serial.print(", ");
  // Serial.println(right_dist);

  double ema_const = 0.2;
  double current_pose_x = 0, current_pose_y = 0;

  // get the robot's pose
  if (right_wall.x.size() < 5 || right_dist != right_dist)
  {
    current_pose_x = left_dist;
  }
  else if (left_wall.x.size() < 5 || left_dist != left_dist)
  {
    current_pose_x = 1820 - right_dist;
  }
  else
  {
    current_pose_x = (1820 / (left_dist + right_dist)) * left_dist;
  }

  if (front_wall.x.size() < 5 || front_dist != front_dist)
  {
    current_pose_y = 2430 - back_dist;
  }
  else if (back_wall.x.size() < 5 || back_dist != back_dist)
  {
    current_pose_y = front_dist;
  }
  else
  {
    current_pose_y = (2430 / (front_dist + back_dist)) * front_dist;
  }

  if (current_pose_x == current_pose_x)
  {
    current_pose.x = (current_pose_x * ema_const) + (previous_pose.x * (1 - ema_const));
  }

  if (current_pose_y == current_pose_y)
  {
    current_pose.y = (current_pose_y * ema_const) + (previous_pose.y * (1 - ema_const));
  }

  // Serial.print(robot.current_pose.x);
  // Serial.print(", ");
  // Serial.println(robot.current_pose.y);

  previous_pose.x = current_pose.x;
  previous_pose.y = current_pose.y;

  // Serial.print("(");
  // Serial.print(robot.current_pose.x);
  // Serial.print(", ");
  // Serial.print(robot.current_pose.y);
  // Serial.println(")");
}

double front, left, back, right, last_front, last_left, last_back, last_right;

void tofLidar(double angle, double distance)
{
  if (((abs(0 - angle) < 1) && distance != 0) && (last_front == 0 || abs(distance - last_front) < 300))
  {
    front = distance * cos(radians(90 - angle));
    last_front = front;
  }
  else if ((abs(90 - angle) < 1 && distance != 0) && (last_right == 0 || abs(distance - last_right) < 300))
  {
    right = distance * cos(radians(180 - angle));
    last_right = right;
  }
  else if ((abs(180 - angle) < 1 && distance != 0) && (last_back == 0 || abs(distance - last_back) < 300))
  {
    back = distance * cos(radians(270 - angle));
    last_back = back;
  }
  else if ((abs(270 - angle) < 1 && distance != 0) && (last_left == 0 || abs(distance - last_left) < 300))
  {
    left = distance * cos(radians(360 - angle));
    last_left = left;
  }
}

void Robot::processLidar()
{
  if (IS_OK(lidar.waitPoint()))
  {
    lidar_loop_count += 1;

    double distance = lidar.getCurrentPoint().distance;
    double angle = lidar.getCurrentPoint().angle + current_pose.bearing;
    byte quality = lidar.getCurrentPoint().quality;

    double x = distance * sin(radians(angle));
    double y = distance * cos(radians(angle));

    if (quality != 0 && distance < 2430 && distance != 0)
    {
      int side = getClosestEdge(x, y);

      switch (side)
      {
      case 0:
        front_wall.x.push_back(x);
        front_wall.y.push_back(y);
        break;
      case 1:
        left_wall.x.push_back(x);
        left_wall.y.push_back(y);
        break;
      case 2:
        back_wall.x.push_back(x);
        back_wall.y.push_back(y);
        break;
      case 3:
        right_wall.x.push_back(x);
        right_wall.y.push_back(y);
        break;
      }

      // Serial.print("(");
      // Serial.print(x);
      // Serial.print(", ");
      // Serial.print(y);
      // Serial.println(")");
    }

    if (lidar_loop_count == 120)
    {
      getRobotPose();
      lidar_loop_count = 0;
      front_wall.x.clear();
      front_wall.y.clear();
      left_wall.x.clear();
      left_wall.y.clear();
      back_wall.x.clear();
      back_wall.y.clear();
      right_wall.x.clear();
      right_wall.y.clear();
    }
  }
  else
  {
    teensy_1_rx_data.data.lidar_detected = false;
    sendSerial();
    // digitalWriteFast(LIDAR_PWM, LOW); // stop the rplidar motor

#ifdef SERIAL_DEBUG
    Serial.println("Connecting to RPLIDAR A2M12...");
#endif

    // try to detect RPLIDAR...
    rplidar_response_device_info_t info;
    if (IS_OK(lidar.getDeviceInfo(info, 100)))
    {
      // detected...
      lidar.startScan();
      lidar_loop_count = 0;

      // start motor rotating at max allowed speed
      // digitalWrite(LIDAR_PWM, HIGH);
      teensy_1_rx_data.data.lidar_detected = true;
      sendSerial();
      delay(1000);
    }
  }
}