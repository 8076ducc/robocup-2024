#pragma once
#ifndef _SERIAL_H_
#define _SERIAL_H_

const int stm_serial_baud = 512000;
const int layer_1_serial_baud = 512000;
const int cam_serial_baud = 1000000;

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

struct STMTxData {
    std::array<double, 24> readings;
    bool ball_detected;
    double ball_bearing; 
    double ball_dist;
};

struct CamTxData
{
    bool yellow_goal_detected;
    int yellow_goal_x;
    int yellow_goal_y;
    bool blue_goal_detected;
    int blue_goal_x;
    int blue_goal_y;
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

union STMTxDataUnion {
    STMTxData data; 
    byte bytes[sizeof(STMTxData)]; 

    STMTxDataUnion(): data() {} 
};

union CamTxDataUnion
{
    CamTxData data;
    byte bytes[sizeof(CamTxData)];

    CamTxDataUnion() : data() {}
};

#endif