#include "main.h"
#include <line.h>
#include <RPLidar.h>

RPLidar lidar;

Line front_wall, left_wall, back_wall, right_wall;

const double y_bounds[2] = {-1215, 1215};
const double x_bounds[2] = {-910, 910};

double regressedDistance(double distance)
{
  double regressed_distance = pow(0.0009606260 * distance, 3) - pow(0.1161190388 * distance, 2) + 8.8058432711 * distance;
  return regressed_distance;
}

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

void Robot::getLidarPose()
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

  double current_pose_x = 0, current_pose_y = 0;

  bool left_wall_invalid = left_wall.x.size() < 5 || left_dist != left_dist;
  bool right_wall_invalid = right_wall.x.size() < 5 || right_dist != right_dist;

  // get the robot's pose
  if (left_wall_invalid && right_wall_invalid)
  {
    lidar_confidence_x = 0;
  }
  else if (right_wall_invalid)
  {
    current_pose_x = left_dist;
    lidar_confidence_x = 1;
  }
  else if (left_wall_invalid)
  {
    current_pose_x = 1820 - right_dist;
    lidar_confidence_x = 1;
  }
  else
  {
    current_pose_x = (1820 / (left_dist + right_dist)) * left_dist;
    lidar_confidence_x = 1;
  }

  bool front_wall_invalid = front_wall.x.size() < 5 || front_dist != front_dist;
  bool back_wall_invalid = back_wall.x.size() < 5 || back_dist != back_dist;

  if (front_wall_invalid && back_wall_invalid)
  {
    lidar_confidence_y = 0;
  }
  else if (front_wall_invalid)
  {
    current_pose_y = 2430 - back_dist;
    lidar_confidence_y = 1;
  }
  else if (back_wall_invalid)
  {
    current_pose_y = front_dist;
    lidar_confidence_y = 1;
  }
  else
  {
    current_pose_y = (2430 / (front_dist + back_dist)) * front_dist;
    lidar_confidence_y = 1;
  }

  Threads::Scope m(lidar_data_lock);

  if (current_pose_x == current_pose_x && current_pose_x != 0)
  {
    lidar_pose.x = current_pose_x;
  }

  if (current_pose_y == current_pose_y && current_pose_y != 0)
  {
    lidar_pose.y = current_pose_y;
  }

  Serial.print("Lidar: ");
  Serial.print(lidar_pose.x);
  Serial.print(", ");
  Serial.println(lidar_pose.y);

  // Serial.print(robot.current_pose.x);
  // Serial.print(", ");
  // Serial.println(robot.current_pose.y);

  // previous_pose.x = current_pose.x;
  // previous_pose.y = current_pose.y;

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
      getLidarPose();
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

    // #ifdef SERIAL_DEBUG
    Serial.println("Connecting to RPLIDAR A2M12...");
    // #endif

    // try to detect RPLIDAR...
    rplidar_response_device_info_t info;
    if (IS_OK(lidar.getDeviceInfo(info, 100)))
    {
      // detected...
      lidar.startScan();
      lidar_loop_count = 0;

      // start motor rotating at max allowed speed
      teensy_1_rx_data.data.lidar_detected = true;
      sendSerial();
      delay(1000);
    }
  }
}

void Robot::getCameraPose(double yellow_goal_x, double yellow_goal_y, double blue_goal_x, double blue_goal_y)
{
  Pose centre_of_field;

  centre_of_field.x = (yellow_goal_x + blue_goal_x) / 2;
  centre_of_field.y = ((abs(yellow_goal_y) + abs(blue_goal_y)) / 2298) * (yellow_goal_y + blue_goal_y) / 2;
  centre_of_field.bearing = degrees(atan2(regressedDistance(centre_of_field.x), regressedDistance(centre_of_field.y)));

  double distance_from_centre = sqrt(pow(centre_of_field.x, 2) + pow(centre_of_field.y, 2));

  centre_of_field.bearing += current_pose.bearing;
  centre_of_field.x = sin(radians(centre_of_field.bearing)) * distance_from_centre;
  centre_of_field.y = cos(radians(centre_of_field.bearing)) * distance_from_centre;

  camera_pose.x = 1820 / 2 - centre_of_field.x;
  camera_pose.y = 2430 / 2 - centre_of_field.y;

  camera_confidence_x = 1 - lidar_confidence_x;
  camera_confidence_y = 1 - lidar_confidence_y;

  Serial.print("Camera: ");
  Serial.print(camera_pose.x);
  Serial.print(", ");
  Serial.println(camera_pose.y);
}

void Robot::getSingleCameraPose(int x, int y)
{
  double angle_from_robot = degrees(atan2(x, y));

  camera_pose.bearing = correctBearing(angle_from_robot + current_pose.bearing);
  camera_pose.x = 1820 / 2 - x;
  camera_pose.y = cos(radians(camera_pose.bearing)) > 0 ? 2298 - y : 0 - y;

  Serial.print("Camera: ");
  Serial.print(camera_pose.x);
  Serial.print(", ");
  Serial.println(camera_pose.y);
}

void Robot::getRobotPose()
{
  lidar_confidence_x = 1;
  lidar_confidence_y = 1;
  camera_confidence_x = 1 - lidar_confidence_x;
  camera_confidence_y = 1 - lidar_confidence_y;

  Threads::Scope m(lidar_data_lock);
  current_pose.x = lidar_confidence_x * lidar_pose.x + camera_confidence_x * camera_pose.x;
  current_pose.y = lidar_confidence_y * lidar_pose.y + camera_confidence_y * camera_pose.y;

  // Serial.print(current_pose.x);
  // Serial.print(", ");
  // Serial.println(current_pose.y);

  double ema_const = 0.2;

  current_pose.x = (current_pose.x * ema_const) + (previous_pose.x * (1 - ema_const));
  current_pose.y = (current_pose.y * ema_const) + (previous_pose.y * (1 - ema_const));

  previous_pose.x = current_pose.x;
  previous_pose.y = current_pose.y;
}