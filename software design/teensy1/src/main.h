#pragma once 

#ifndef main.h
#define main.h

#define DEBUG

#include <Arduino.h>
#include <cmath>

#define LED 13

#define M1_PWM 2
#define M2_PWM 3
#define M3_PWM 4
#define M4_PWM 5
#define LIDAR_PWM 6
#define DRIBBLER_PWM 23

#define M1_INA 9
#define M2_INA 10
#define M3_INA 11
#define M4_INA 12

#define M1_CS 16
#define M2_CS 17
#define M3_CS 18
#define M4_CS 19

#define LAYER1 Serial1
#define LIDAR Serial2
#define IMU Serial3
#define TEENSY Serial5

#endif