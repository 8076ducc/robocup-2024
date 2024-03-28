#pragma once

#ifndef SHARED_H
#define SHARED_H

#include "utils.h"

const int serial_baud = 512000;
const int bt_serial_baud = 115200;
const int cam_serial_baud = 1000000;
const int imu_serial_baud = 512000;
const int layer_1_serial_baud = 512000;
const int teensy1_serial_baud = 1000000;
const int serial_monitor_baud = 512000;

struct BtTxData
{
    bool ball_detected;
    Pose ball_pose;
    bool robot_detected;
    Pose robot_pose;
};

struct BtRxData
{
    bool ball_detected;
    Pose ball_pose;
    Pose robot_pose;
};

struct CamTxData
{
    bool ball_detected;
    int ball_x;
    int ball_y;
    bool yellow_goal_detected;
    int yellow_goal_x;
    int yellow_goal_y;
    bool blue_goal_detected;
    int blue_goal_x;
    int blue_goal_y;
};

struct CamRxData
{
};

struct ImuTxData
{
    double bearing;
};

struct ImuRxData
{
};

struct Layer1TxData
{
    bool on_line;
    double target_angle;
    double chord_length;
    double line_centre;
    bool ball_in_catchment;
};

struct Layer1RxData
{
    bool kick;
    int line_start;
    int line_end;
};

struct Teensy1TxData
{
    double bearing;
};

struct Teensy1RxData
{
    Pose current_pose;
    Pose target_pose;

    Goal yellow_goal;
    Goal blue_goal;

    bool kick;
    bool lidar_detected;
};

union BtTxDataUnion
{
    BtTxData data;
    byte bytes[sizeof(BtTxData)];

    BtTxDataUnion() : data() {}
};

union BtRxDataUnion
{
    BtRxData data;
    byte bytes[sizeof(BtRxData)];

    BtRxDataUnion() : data() {}
};

union CamTxDataUnion
{
    CamTxData data;
    byte bytes[sizeof(CamTxData)];

    CamTxDataUnion() : data() {}
};

union CamRxDataUnion
{
    CamRxData data;
    byte bytes[sizeof(CamRxData)];

    CamRxDataUnion() : data() {}
};

union ImuTxDataUnion
{
    ImuTxData data;
    byte bytes[sizeof(ImuTxData)];

    ImuTxDataUnion() : data() {}
};

union ImuRxDataUnion
{
    ImuRxData data;
    byte bytes[sizeof(ImuRxData)];

    ImuRxDataUnion() : data() {}
};

union Layer1TxDataUnion
{
    Layer1TxData data;
    byte bytes[sizeof(Layer1TxData)];

    Layer1TxDataUnion() : data() {}
};

union Layer1RxDataUnion
{
    Layer1RxData data;
    byte bytes[sizeof(Layer1RxData)];

    Layer1RxDataUnion() : data() {}
};

union Teensy1TxDataUnion
{
    Teensy1TxData data;
    byte bytes[sizeof(Teensy1TxData)];

    Teensy1TxDataUnion() : data() {}
};

union Teensy1RxDataUnion
{
    Teensy1RxData data;
    byte bytes[sizeof(Teensy1RxData)];

    Teensy1RxDataUnion() : data() {}
};

#endif