#pragma once

#ifndef MAIN_H
#define MAIN_H

// #define DEBUG
#define STRATEGY 1
// #define WHITE_BOT
// #define BLACK_BOT

#include <Arduino.h>
#include <cmath>
#include <digitalWriteFast.h>
#include <PacketSerial.h>

// shared project headerfiles
#include <serial.h>

#define LED 13

#define FL_PWM 2
#define FR_PWM 3
#define BL_PWM 4
#define BR_PWM 5
#define LIDAR_PWM 6
#define DRIBBLER_PWM 23

#define FL_INA 9
#define FR_INA 10
#define BL_INA 11
#define BR_INA 12

#define FL_CS 16
#define FR_CS 17
#define BL_CS 18
#define BR_CS 19

const int wheel_angle = 50 * M_PI / 180.0;
const int min_speed = 25;
const int max_speed = 1;
const int max_accel = 0.5;

class Pose {
    public:
        int x;
        int y;
        int bearing;
};

class Robot {
    public:
        void setUpMotors();
        void setUpDribbler();
        void move(float speed, float angle, float angVel);
        void motorOut(int motor, float speed);
        void moveToTargetPose();

        Pose current_pose;
        Pose target_pose;
};

class Ball {
    public:
        Pose current_pose;
        Pose projected_pose;

        bool in_catchment;
};

extern PacketSerial LAYER1;
extern PacketSerial LIDAR;
extern PacketSerial IMU;
extern PacketSerial TEENSY;

extern Robot robot;
extern Ball ball;

void onPacketReceived(const byte *buf, size_t size);

float degToRad(float deg);

#endif