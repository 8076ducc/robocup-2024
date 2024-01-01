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
#include <PacketSerial.h>

// shared project header files
#include <serial.h>

#define LED 13

#define DIP_1 2
#define DIP_2 3
#define DIP_3 4
#define DIP_4 5

const int wheel_angle = 50 * M_PI / 180.0;
const int min_speed = 25;
const int max_speed = 1;
const int max_accel = 0.5;

class Robot {
    public:
        void move(float speed, float angle, float angVel);
        void motorOut(int motor, float speed);

        Pose current_pose;
        Pose target_pose;
};

class Ball {
    public:
        Pose current_pose;
        Pose projected_pose;
};

//global variables

extern PacketSerial Cam1Serial;
extern PacketSerial Cam2Serial;
extern PacketSerial BtSerial;
extern PacketSerial TeensySerial;

extern Robot robot;
extern Ball ball;

void onPacketReceived(const byte *buf, size_t size);

#endif