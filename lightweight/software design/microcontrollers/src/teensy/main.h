#pragma once

#ifndef MAIN_H
#define MAIN_H

// #define DEBUG
// #define SERIAL_DEBUG
#define STRATEGY 1
// #define WHITE_BOT
// #define BLACK_BOT

#include <common.h>

#define DRIBBLER_LOWER_LIMIT 32
#define DRIBBLER_UPPER_LIMIT 48

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

#define FL_CS 17
#define FR_CS 16
#define BL_CS 18
#define BR_CS 19

class Base
{
public:
    void setUp();
    void move(double vel, double angle, double bearing);
    void motorOut(int motor, double speed);
    double getAggregateSpeed(int motor);

    const double wheel_angle = 50 * M_PI / 180.0;
    const int max_pwm = 8192;
    const int min_speed = 1100;
    const double ema_constant = 0.03;

    const double fl_voltage = 2.500;
    const double fr_voltage = 2.300;
    const double bl_voltage = 2.282;
    const double br_voltage = 2.480;
    const double fastest_motor = fl_voltage;

    // const double fl_scale = fastest_motor / fl_voltage;
    // const double fr_scale = fastest_motor / fr_voltage;
    // const double bl_scale = fastest_motor / bl_voltage;
    // const double br_scale = fastest_motor / br_voltage;

    const int fl_scale = 1;
    const int fr_scale = 1;
    const int bl_scale = 1;
    const int br_scale = 1;

    double prev_fl_out = 0;
    double prev_fr_out = 0;
    double prev_bl_out = 0;
    double prev_br_out = 0;
};

class Robot
{
public:
    void moveToTargetPose();
    void setUpSerial();
    void updateSerial();
    void sendSerial();

    void setupIMU();
    void readIMU();

    void defendGoal();
    void rotateToBall();
    void orbitToBall();
    void rotateScore();
    void orbitScore();

    void getCameraPose(int yellow_goal_x, int yellow_goal_y, int blue_goal_x, int blue_goal_y);

    Base base;

    Pose previous_pose;
    Pose current_pose;
    Pose target_pose;

    double x_diff, y_diff, angle_diff, prev_distance;

    int chord_length;

    double line_centre;

    bool on_line;
    double target_angle;
};

// global variables
extern PacketSerial Layer1Serial;
extern PacketSerial STMSerial;
extern PacketSerial CamSerial;

extern Layer1RxDataUnion layer_1_rx_data;

extern Robot robot;
extern Ball ball;
extern Goal yellow_goal;
extern Goal blue_goal;

#endif