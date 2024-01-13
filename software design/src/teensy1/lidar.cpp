#include "main.h"
#include <RPLidar.h>

RPLidar lidar;

void lidarSetup() {
  lidar.begin(Serial2);
  pinMode(LIDAR_PWM, OUTPUT);
}

void processLidar() {
  if (IS_OK(lidar.waitPoint())) {
    float distance = lidar.getCurrentPoint().distance;
    float angle = lidar.getCurrentPoint().angle;
    byte quality = lidar.getCurrentPoint().quality;

    double x = distance * sin(degToRad(angle));
    double y = distance * cos(degToRad(angle));

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

    #ifdef SERIAL_DEBUG
    // if (quality != 0) {
      Serial.print("(");
      Serial.print(x);
      Serial.print(", ");
      Serial.print(y);
      Serial.println(")");
    // }
    #endif

    // Serial.print(distance);
    // Serial.print(", ");
    // Serial.print(angle);
    // Serial.print(", ");
    // Serial.println(quality);

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
       
       // start motor rotating at max allowed speed
       analogWrite(LIDAR_PWM, 255);
       delay(1000);
    }
  }
}