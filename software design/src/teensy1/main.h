#pragma once

#ifndef MAIN_H
#define MAIN_H

// #define DEBUG
#define SERIAL_DEBUG
#define STRATEGY 1
// #define WHITE_BOT
// #define BLACK_BOT

#include <common.h>
#include <RPLidar.h>


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
        void move(float vel, float angle);
        void motorOut(int motor, float speed);
        double getAggregateSpeed(int motor);

        const double wheel_angle = 50 * M_PI / 180.0;
        const int min_speed = 25;
        const double ema_constant = 0.01;

        double prev_fl_out = 0;
        double prev_fr_out = 0;
        double prev_bl_out = 0;
        double prev_br_out = 0;
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
        double target_angle;
};

struct Ball {
    Pose current_pose;
    Pose projected_pose;
    
    bool in_catchment;
    bool in_alliance_catchment;
};

class Line {
  public:
    std::vector<double> x;
    std::vector<double> y;

    double slope;
    double intercept;

    // Function to calculate the slope and intercept of a linear regression line
    void horizontalLinearRegression();
    void verticalLinearRegression();
    void orthogonalLinearRegression();
};


// g;obal variables
extern PacketSerial Layer1Serial;
extern PacketSerial ImuSerial;
extern PacketSerial TeensySerial;

extern Layer1RxDataUnion layer_1_rx_data;
extern ImuRxDataUnion imu_rx_data;
extern Teensy1TxDataUnion teensy_1_tx_data;

extern Robot robot;
extern Ball ball;
extern RPLidar lidar;

extern Line front_wall;
extern Line left_wall;
extern Line back_wall;
extern Line right_wall;

const double y_bounds [2] = {-1215, 1215};
const double x_bounds [2] = {-910, 910};

// extern double front, left, back, right, last_front, last_left, last_back, last_right;

float degToRad(float deg);
void lidarSetup();
void processLidar();

#endif