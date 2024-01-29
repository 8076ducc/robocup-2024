#include "main.h"
#include <RPLidar.h>

RPLidar lidar;

Line front_wall, left_wall, back_wall, right_wall;

void lidarSetup() {
  lidar.begin(Serial2);
  pinModeFast(LIDAR_PWM, OUTPUT);
}

int getClosestEdge(double x, double y) {
  double distance_to_front, distance_to_left, distance_to_back, distance_to_right;

  if (x < x_bounds[0]) {
    distance_to_front = sqrt(pow(x - x_bounds[0], 2) + pow(y - y_bounds[0], 2));
    distance_to_back = sqrt(pow(x - x_bounds[0], 2) + pow(y - y_bounds[1], 2));
  } else if (x > x_bounds[1]) {
    distance_to_front = sqrt(pow(x - x_bounds[1], 2) + pow(y - y_bounds[0], 2));
    distance_to_back = sqrt(pow(x - x_bounds[1], 2) + pow(y - y_bounds[1], 2));
  } else {
    distance_to_front = abs(y - y_bounds[0]);
    distance_to_back = abs(y - y_bounds[1]);
  }
  
  if (y < y_bounds[0]) {
    distance_to_left = sqrt(pow(x - x_bounds[0], 2) + pow(y - y_bounds[0], 2));
    distance_to_right = sqrt(pow(x - x_bounds[1], 2) + pow(y - y_bounds[0], 2));
  } else if (y > y_bounds[1]) {
    distance_to_left = sqrt(pow(x - x_bounds[0], 2) + pow(y - y_bounds[1], 2));
    distance_to_right = sqrt(pow(x - x_bounds[1], 2) + pow(y - y_bounds[1], 2));
  } else {
    distance_to_left = abs(x - x_bounds[0]);
    distance_to_right = abs(x - x_bounds[1]);
  }

  // assign a number to the closest edge
  if (distance_to_front < distance_to_left && distance_to_front < distance_to_back && distance_to_front < distance_to_right) {
    return 0;
  } else if (distance_to_left < distance_to_front && distance_to_left < distance_to_back && distance_to_left < distance_to_right) {
    return 1;
  } else if (distance_to_back < distance_to_front && distance_to_back < distance_to_left && distance_to_back < distance_to_right) {
    return 2;
  } else {
    return 3;
  }
}

std::vector<double> front_x, front_y, left_x, left_y, back_x, back_y, right_x, right_y;
int lidar_loop_count = 0;

void getRobotPose() {
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

  Serial.print(front_dist);
  Serial.print(", ");
  Serial.print(left_dist);
  Serial.print(", ");
  Serial.print(back_dist);
  Serial.print(", ");
  Serial.println(right_dist);

  // get the robot's pose
  if (right_wall.x.size() < 2) {
    robot.current_pose.y = 1820 - left_dist;
  } else if (left_wall.x.size() < 2) {
    robot.current_pose.y = 1820 - right_dist;
  } else {
    robot.current_pose.y = (1820 / (left_dist + right_dist)) * left_dist;
  }
  
  if (front_wall.x.size() < 2) {
    robot.current_pose.x = 2430 - back_dist;
  } else if (back_wall.x.size() < 2) {
    robot.current_pose.x = 2430 - front_dist;
  } else {
    robot.current_pose.x = (2430 / (front_dist + back_dist)) * front_dist;
  }

  // Serial.print("(");
  // Serial.print(robot.current_pose.x);
  // Serial.print(", ");
  // Serial.print(robot.current_pose.y);
  // Serial.println(")");
}

double front, left, back, right, last_front, last_left, last_back, last_right;

void tofLidar(double angle, double distance) {
    if (((abs(0 - angle) < 1) && distance !=0) && (last_front == 0 || abs(distance - last_front) < 300)) {
      front = distance * cos(degToRad(90 - angle));
      last_front = front;
    } else if ((abs(90 - angle) < 1 && distance !=0) && (last_right == 0 || abs(distance - last_right) < 300)) {
      right = distance * cos(degToRad(180 - angle));
      last_right = right;
    } else if ((abs(180 - angle) < 1 && distance !=0) && (last_back == 0 || abs (distance - last_back) < 300)) {
      back = distance * cos(degToRad(270 - angle));
      last_back = back;
    } else if ((abs(270 - angle) < 1 && distance !=0) && (last_left == 0 || abs (distance - last_left) < 300)) {
      left = distance * cos(degToRad(360 - angle));
      last_left = left;
    }
}

void processLidar() {
  if (IS_OK(lidar.waitPoint())) {
    lidar_loop_count += 1;

    float distance = lidar.getCurrentPoint().distance;
    float angle = lidar.getCurrentPoint().angle + robot.current_pose.bearing;
    byte quality = lidar.getCurrentPoint().quality;

    double x = distance * sin(degToRad(angle));
    double y = distance * cos(degToRad(angle));

    if (quality != 0 && distance < 2430 && distance != 0) {
      int side = getClosestEdge(x, y);

      switch (side) {
        case 0:
          front_wall.x.push_back(x);
          front_wall.y.push_back(y);
          // Serial.print("(");
          // Serial.print(x);
          // Serial.print(", ");
          // Serial.print(y);
          // Serial.println(")");
          break;
        case 1:
          left_wall.x.push_back(x);
          left_wall.y.push_back(y);
          // Serial.print("(");
          // Serial.print(x);
          // Serial.print(", ");
          // Serial.print(y);
          // Serial.println(")");
          break;
        case 2:
          back_wall.x.push_back(x);
          back_wall.y.push_back(y);
          // Serial.print("(");
          // Serial.print(x);
          // Serial.print(", ");
          // Serial.print(y);
          // Serial.println(")");
          break;
        case 3:
          right_wall.x.push_back(x);
          right_wall.y.push_back(y);
          // Serial.print("(");
          // Serial.print(x);
          // Serial.print(", ");
          // Serial.print(y);
          // Serial.println(")");
          break;
      }

      #ifdef SERIAL_DEBUG
      // Serial.print("(");
      // Serial.print(x);
      // Serial.print(", ");
      // Serial.print(y);
      // Serial.println(")");
      #endif
    }

    if (lidar_loop_count == 480) {
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
  } else {
    digitalWriteFast(LIDAR_PWM, LOW); //stop the rplidar motor

    #ifdef SERIAL_DEBUG
    Serial.println("Connecting to RPLIDAR A2M12...");
    #endif

    // try to detect RPLIDAR... 
    rplidar_response_device_info_t info;
    if (IS_OK(lidar.getDeviceInfo(info, 100))) {
       // detected...
       lidar.startScan();
       lidar_loop_count = 0;
       
       // start motor rotating at max allowed speed
       digitalWriteFast(LIDAR_PWM, HIGH);
       delay(1000);
    }
  }
}