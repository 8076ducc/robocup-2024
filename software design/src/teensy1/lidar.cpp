#include "main.h"
#include <RPLidar.h>

RPLidar lidar;

void lidarSetup() {
  Serial2.begin(256000);
  lidar.begin(Serial2);
  pinMode(LIDAR_PWM, OUTPUT);
}

void processLidar() {
  if (IS_OK(lidar.waitPoint())) {
    float distance = lidar.getCurrentPoint().distance;
    float angle    = lidar.getCurrentPoint().angle;
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