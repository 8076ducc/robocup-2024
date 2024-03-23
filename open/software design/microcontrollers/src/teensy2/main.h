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

    Pose current_pose;
    Pose target_pose;
    Pose projected_pose;
};

struct Ball
{
    Pose current_pose;
    Pose projected_pose;

    bool in_catchment;
    bool in_alliance_catchment;
};

class Strategy
{
    void chaseBall();
};

// global variables

extern PacketSerial Cam1Serial;
extern PacketSerial Cam2Serial;
extern PacketSerial BtSerial;
extern PacketSerial TeensySerial;

extern Robot robot;
extern Ball ball;
extern Strategy strategy;

extern BtRxDataUnion bt_rx_data;
extern BtTxDataUnion bt_tx_data;
extern Teensy1RxDataUnion teensy_1_rx_data;
extern Teensy1TxDataUnion teensy_1_tx_data;

void setUpLidar();
void processLidar();

#endif