#include "main.h"
#include <RPLidar.h>

PacketSerial Cam1Serial;
PacketSerial Cam2Serial;
PacketSerial BtSerial;
PacketSerial TeensySerial;

Cam1RxDataUnion cam1_rx_data;
Cam2RxDataUnion cam2_rx_data;
BtRxDataUnion bt_rx_data;
Teensy1RxDataUnion teensy_1_rx_data;

Robot robot;
Ball ball;
Strategy strategy;

// You need to create an driver instance 
RPLidar lidar;

#define RPLIDAR_MOTOR 6 // The PWM pin for control the speed of RPLIDAR's motor.
                        // This pin should connected with the RPLIDAR's MOTOCTRL signal 
                       
                        
void setup() {
  pinMode(led, OUTPUT);
  digitalWrite(led, HIGH);
}

void loop() {
}

// void setup() {
//   robot.setUpSerial();
// }

// void loop() {

//   BtSerial.send(bt_rx_data.bytes, sizeof(bt_rx_data.bytes));
//   TeensySerial.send(teensy_1_rx_data.bytes, sizeof(teensy_1_rx_data.bytes));
// }