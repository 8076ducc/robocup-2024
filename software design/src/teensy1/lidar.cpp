#include "main.h"
#include <RPLidar.h>

RPLidar lidar;

void lidarSetup() {
  lidar.begin(Serial2);
  pinMode(LIDAR_PWM, OUTPUT);
}

// Function to calculate the mean of a vector
double mean(std::vector<double> v) {
    double sum = 0.0;
    for (int i = 0; i < v.size(); i++) {
        sum += v[i];
    }
    return sum / v.size();
}

class Line {
  public:
    std::vector<double> x;
    std::vector<double> y;

    double slope;
    double intercept;

    // Function to calculate the slope and intercept of a linear regression line
    void linearRegression();
};

void Line::linearRegression() {
  double x_mean = mean(x);
  double y_mean = mean(y);
  double numerator = 0.0;
  double denominator = 0.0;
  for (int i = 0; i < x.size(); i++) {
      numerator += (x[i] - x_mean) * (y[i] - y_mean);
      denominator += pow(x[i] - x_mean, 2);
  }
  slope = numerator / denominator;
  intercept = y_mean - slope * x_mean;
}

Line front_wall;
Line left_wall;
Line back_wall;
Line right_wall;

const double y_bounds [2] = {-1215, 1215};
const double x_bounds [2] = {-910, 910};

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
  front_wall.linearRegression();
  left_wall.linearRegression();
  back_wall.linearRegression();
  right_wall.linearRegression();

  // get the points of intercept
  double a1 = front_wall.slope;
  double b1 = 1;
  double c1 = front_wall.intercept;

  double a2 = left_wall.slope;
  double b2 = 1;
  double c2 = left_wall.intercept;
  
  double front_left_x = (((b1 * c2) - (b2 * c1)) / ((a1 * b2) - (a2 * b1)));
  double front_left_y = (((a2 * c1) - (a1 * c2)) / ((a1 * b2) - (a2 * b1)));

  double a1 = left_wall.slope;
  double b1 = 1;
  double c1 = left_wall.intercept;

  double a2 = back_wall.slope;
  double b2 = 1;
  double c2 = back_wall.intercept;

  double left_back_x = (left_c - back_c) / (back_m - left_m);
  double left_back_y = left_m * left_back_x + left_c;

  double a1 = back_wall.slope;
  double b1 = 1;
  double c1 = back_wall.intercept;

  double a2 = right_wall.slope;
  double b2 = 1;
  double c2 = right_wall.intercept;

  double back_right_x = (back_c - right_c) / (right_m - back_m);
  double back_right_y = back_m * back_right_x + back_c;

  double a1 = right_wall.slope;
  double b1 = 1;
  double c1 = right_wall.intercept;

  double a2 = front_wall.slope;
  double b2 = 1;
  double c2 = front_wall.intercept;

  double front_right_x = (front_c - right_c) / (right_m - front_m);
  double front_right_y = front_m * front_right_x + front_c;

  // get the robot's pose
  double x = (2430 / (fronty + back)) * front;
  double y = (front_left_y + front_right_y + left_back_y + back_right_y) / 4;

  Serial.print("(");
  Serial.print(x);
  Serial.print(", ");
  Serial.print(y);
  Serial.println(")");
}

void processLidar() {
  if (IS_OK(lidar.waitPoint())) {
    loop += 1;

    float distance = lidar.getCurrentPoint().distance;
    float angle = lidar.getCurrentPoint().angle;
    byte quality = lidar.getCurrentPoint().quality;

    double x = distance * sin(degToRad(angle));
    double y = distance * cos(degToRad(angle));

    // if (((abs(0 - angle) < 1) && distance !=0) && (last_front == 0 || abs(distance - last_front) < 300)) {
    //   front = distance * cos(degToRad(90 - angle));
    //   last_front = front;
    // } else if ((abs(90 - angle) < 1 && distance !=0) && (last_right == 0 || abs(distance - last_right) < 300)) {
    //   right = distance * cos(degToRad(180 - angle));
    //   last_right = right;
    // } else if ((abs(180 - angle) < 1 && distance !=0) && (last_back == 0 || abs (distance - last_back) < 300)) {
    //   back = distance * cos(degToRad(270 - angle));
    //   last_back = back;
    // } else if ((abs(270 - angle) < 1 && distance !=0) && (last_left == 0 || abs (distance - last_left) < 300)) {
    //   left = distance * cos(degToRad(360 - angle));
    //   last_left = left;
    // }

    if (quality != 0 && distance < 2430) {
      side = getClosestEdge(x, y)

      switch (side) {
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

      #ifdef SERIAL_DEBUG
      // Serial.print("(");
      // Serial.print(x);
      // Serial.print(", ");
      // Serial.print(y);
      // Serial.println(")");
      #endif
    }

    if (loop == 1600) {
      loop = 0;
      front_x.clear();
      front_y.clear();
      left_x.clear();
      left_y.clear();
      back_x.clear();
      back_y.clear();
      right_x.clear();
      right_y.clear();
    }
  } else {
    analogWrite(LIDAR_PWM, 0); //stop the rplidar motor

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
       analogWrite(LIDAR_PWM, 255);
       delay(1000);
    }
  }
}