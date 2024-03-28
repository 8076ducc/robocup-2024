#pragma once

#ifndef MAIN_H
#define MAIN_H

// #define DEBUG
#define STRATEGY 1
// #define WHITE_BOT
// #define BLACK_BOT

#include <common.h>

#define LED 13

#define DIP_1 2
#define DIP_2 3
#define DIP_3 4
#define DIP_4 5

class Robot
{
public:
    void setUpSerial();
    void updateSerial();
    void sendSerial();

    void setUpLidar();
    void processLidar();
    void getLidarPose();

    void getCameraPose(int yellow_goal_x, int yellow_goal_y, int blue_goal_x, int blue_goal_y);

    void getRobotPose();

    Pose previous_pose;
    Pose current_pose;
    Pose target_pose;
    Pose projected_pose;

    Pose lidar_pose;
    Pose camera_pose;

    double lidar_confidence_x;
    double lidar_confidence_y;
    double camera_confidence_x;
    double camera_confidence_y;
};

// global variables

extern PacketSerial Cam1Serial;
extern PacketSerial Cam2Serial;
extern PacketSerial BtSerial;
extern PacketSerial TeensySerial;

extern Robot robot;
extern Ball ball;
extern Goal yellow_goal;
extern Goal blue_goal;

extern BtRxDataUnion bt_rx_data;
extern BtTxDataUnion bt_tx_data;
extern CamRxDataUnion cam_rx_data;
extern CamTxDataUnion cam_tx_data;
extern Teensy1RxDataUnion teensy_1_rx_data;
extern Teensy1TxDataUnion teensy_1_tx_data;

#endif