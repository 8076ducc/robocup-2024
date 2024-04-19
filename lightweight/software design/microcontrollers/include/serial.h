#pragma once
#ifndef _SERIAL_H_
#define _SERIAL_H_

const int stm_serial_baud = 512000;
const int layer_1_serial_baud = 512000;
const int cam_serial_baud = 1000000;

struct Layer1TxData
{
    bool on_line;
    double line_angle;
    double chord_length;
    double line_centre;
    int line_track_error;
    bool ball_in_catchment;
    int line_start_ldr;
    int line_end_ldr;
};

struct Layer1RxData
{
    bool kick;
    int line_track_ldr;
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
    double yellow_goal_x;
    double yellow_goal_y;
    bool blue_goal_detected;
    double blue_goal_x;
    double blue_goal_y;
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
    byte bytes[8];
    uint16_t vals[4];

    CamTxDataUnion() : data() {}
};

#endif