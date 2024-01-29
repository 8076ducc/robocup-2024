#include "main.h"

// Pa cketSerial Cam1Serial;
// PacketSerial Cam2Serial;
// PacketSerial BtSerial;
// PacketSerial TeensySerial;

// Cam1RxDataUnion cam1_rx_data;
// Cam2RxDataUnion cam2_rx_data;
// BtRxDataUnion bt_rx_data;
// Teensy1RxDataUnion teensy_1_rx_data;

// Robot robot;
// Ball ball;
// Strategy strategy;

// void setup() {
//   robot.setUpSerial();
// }

// void loop() {

//   BtSerial.send(bt_rx_data.bytes, sizeof(bt_rx_data.bytes));
//   TeensySerial.send(teensy_1_rx_data.bytes, sizeof(teensy_1_rx_data.bytes));
// }

#include <RPLidar.h>

#define LIDAR Serial2

RPLidar lidar;

void setup() {
  // Serial.begin(256000);
  // while (!Serial) {
  // }
  // Serial.println("Initialising...");

  LIDAR.begin(256000);

  // bind the RPLIDAR driver to the arduino hardware serial
  lidar.begin(LIDAR);
}

void loop() {
  if (IS_OK(lidar.waitPoint())) {
    float distance = lidar.getCurrentPoint().distance; //distance value in mm unit
    float angle    = lidar.getCurrentPoint().angle; //anglue value in degree
    bool  startBit = lidar.getCurrentPoint().startBit; //whether this point is belong to a new scan
    byte  quality  = lidar.getCurrentPoint().quality; //quality of the current measurement
    
    //perform data processing here... 
    Serial.print("Angle: ");
    Serial.print(angle);
    Serial.print(" Distance: ");
    Serial.println(distance);
    
  } else {
    Serial.println("trying");

    analogWrite(6, 0);
    
    // try to detect RPLIDAR... 
    
    rplidar_response_device_info_t info;
    if (IS_OK(lidar.getDeviceInfo(info, 100))) {
       // detected...
       lidar.startScan();
       
       // start motor rotating at max allowed speed
       delay(1000);
       Serial.println("Started");
    }
  }
}
