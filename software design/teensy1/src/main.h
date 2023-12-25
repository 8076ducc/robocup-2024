#pragma once

#ifndef MAIN_H
#define MAIN_H

#define DEBUG
#define STRATEGY 1
// #define WHITE_BOT
// #define BLACK_BOT

#include <Arduino.h>
#include <cmath>
#include <digitalWriteFast.h>

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

extern Robot robot;
extern Ball ball;

float degToRad(float deg);

#endif