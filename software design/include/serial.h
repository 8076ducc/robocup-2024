#pragma once

#ifndef SHARED_H
#define SHARED_H

#include "utils.h"

struct BtTxData {
    bool ball_detected;
    Pose ball_pose;
    bool robot_detected;
    Pose robot_pose;
};

struct BtRxData {
    bool ball_detected;
    Pose ball_pose;
    Pose robot_pose;
};

struct Cam1TxData {
    bool ball_detected;
    Pose ball_pose;
};

struct Cam1RxData {
};

struct Cam2TxData {
    bool ball_detected;
    Pose ball_pose;
};

struct Cam2RxData {
};

struct ImuTxData {
    double bearing;
};

struct ImuRxData {
};

struct Layer1TxData {
  bool on_line;
  double target_angle;
  double chord_length;
  bool ball_in_catchment;
};

struct Layer1RxData {
  bool kick;
};

struct Teensy1TxData {
};

struct Teensy1RxData {
    Pose current_pose;
    Pose target_pose;
};

union BtTxDataUnion {
    BtTxData data;
    byte bytes[sizeof(BtTxData)];

    BtTxDataUnion() : data() {}
};

union BtRxDataUnion {
    BtRxData data;
    byte bytes[sizeof(BtRxData)];

    BtRxDataUnion() : data() {}
};

union Cam1TxDataUnion {
    Cam1TxData data;
    byte bytes[sizeof(Cam1TxData)];

    Cam1TxDataUnion() : data() {}
};

union Cam1RxDataUnion {
    Cam1RxData data;
    byte bytes[sizeof(Cam1RxData)];

    Cam1RxDataUnion() : data() {}
};

union Cam2TxDataUnion {
    Cam2TxData data;
    byte bytes[sizeof(Cam2TxData)];

    Cam2TxDataUnion() : data() {}
};

union Cam2RxDataUnion {
    Cam2RxData data;
    byte bytes[sizeof(Cam2RxData)];

    Cam2RxDataUnion() : data() {}
};

union ImuTxDataUnion {
    ImuTxData data;
    byte bytes[sizeof(ImuTxData)];

    ImuTxDataUnion() : data() {}
};

union ImuRxDataUnion {
    ImuRxData data;
    byte bytes[sizeof(ImuRxData)];

    ImuRxDataUnion() : data() {}
};

union Layer1TxDataUnion {
    Layer1TxData data;
    byte bytes[sizeof(Layer1TxData)];

    Layer1TxDataUnion() : data() {}
};

union Layer1RxDataUnion {
    Layer1RxData data;
    byte bytes[sizeof(Layer1RxData)];

    Layer1RxDataUnion() : data() {}
};

union Teensy1TxDataUnion {
    Teensy1TxData data;
    byte bytes[sizeof(Teensy1TxData)];

    Teensy1TxDataUnion() : data() {}
};

union Teensy1RxDataUnion {
    Teensy1RxData data;
    byte bytes[sizeof(Teensy1RxData)];

    Teensy1RxDataUnion() : data() {}
};

#endif