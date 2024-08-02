#pragma once

#ifndef MAIN_H
#define MAIN_H

// #define DEBUG
#define STRATEGY 1
// #define WHITE_BOT
// #define BLACK_BOT

#include <common.h>
#include <TeensyThreads.h>

#define LED 13

#define DIP_1 5
#define DIP_2 4
#define DIP_3 3
#define DIP_4 2

#define SERIAL5_TX_BUFFER_SIZE 100

class Robot
{
public:
    void setUpSerial();
    void updateSerial();
    void sendSerial();

    void setUpLidar();
    void processLidar();
    void getLidarPose();

    void getCameraPose(double yellow_goal_x, double yellow_goal_y, double blue_goal_x, double blue_goal_y);
    void getSingleCameraPose(int x, int y);

    void getRobotPose();

    Pose previous_pose;
    Pose current_pose;
    Pose target_pose;
    Pose projected_pose;

    Pose lidar_pose;
    Pose camera_pose;

    bool update_lidar_pose;
    bool has_updated_lidar_pose;

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

extern Threads::Mutex lidar_data_lock;

extern BtRxDataUnion bt_rx_data;
extern BtTxDataUnion bt_tx_data;
extern CamRxDataUnion cam_rx_data;
extern CamTxDataUnion cam_tx_data;
extern Teensy1RxDataUnion teensy_1_rx_data;
extern Teensy1TxDataUnion teensy_1_tx_data;

void processLidar();
void getLidarPose();

#endif