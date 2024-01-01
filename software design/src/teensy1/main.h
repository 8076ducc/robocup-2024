#pragma once

#ifndef MAIN_H
#define MAIN_H

// #define DEBUG
#define STRATEGY 1
// #define WHITE_BOT
// #define BLACK_BOT

#include <Arduino.h>
#include <bits/stdc++.h>
#include <cmath>
#include <digitalWriteFast.h>
#include <PacketSerial.h>

// shared project header files
#include <serial.h>
#include <utils.h>

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


class Base {
    public:
        void setUpMotors();
        void move(float speed, float angle, float angVel);
        void motorOut(int motor, float speed);
        double getAggregateSpeed(int motor);

        const int wheel_angle = 50 * M_PI / 180.0;
        const int min_speed = 25;
        const double ema_constant = 1;

        double prev_fl_out;
        double prev_fr_out;
        double prev_bl_out;
        double prev_br_out;
};


class Robot {
    public:
        void setUpDribbler();
        void moveToTargetPose();
        void setUpSerial();
        
        Base base;

        Pose current_pose;
        Pose target_pose;

        bool on_line;
};


class Ball {
    public:
        Pose current_pose;
        Pose projected_pose;

        bool in_catchment;
};


// g;obal variables
extern PacketSerial Layer1Serial;
extern PacketSerial ImuSerial;
extern PacketSerial TeensySerial;

extern Robot robot;
extern Ball ball;

float degToRad(float deg);

#endif