#ifndef LIGHT_RING_H
#define LIGHT_RING_H

#include "main.h"

class LightRing
{
public:
    void setup();
    void calibrate();
    void printThresholds();
    void read();

    int line_start_ldr;
    int line_end_ldr;

    int ldr_readings[32];

    int ldr_min_readings[32];
    int ldr_max_readings[32];

    const int ldr_thresholds[32] = {
        631, // 0
        600, // 1
        627, // 2
        600, // 3
        686, // 4
        600, // 5
        690, // 6
        600, // 7
        687, // 8
        600, // 9
        690, // 10
        600, // 11
        662, // 12
        600, // 13
        792, // 14
        600, // 15
        675, // 16
        600, // 17
        674, // 18
        600, // 19
        718, // 20
        600, // 21
        754, // 22
        600, // 23
        769, // 24
        600, // 25
        710, // 26
        600, // 27
        797, // 28
        600, // 29
        631, // 30
        600, // 31
    };

    const int mux_signals[16][4] = {
        {0, 0, 0, 0}, // 0
        {1, 0, 0, 0}, // 1
        {0, 1, 0, 0}, // 2
        {1, 1, 0, 0}, // 3
        {0, 0, 1, 0}, // 4
        {1, 0, 1, 0}, // 5
        {0, 1, 1, 0}, // 6
        {1, 1, 1, 0}, // 7
        {0, 0, 0, 1}, // 8
        {1, 0, 0, 1}, // 9
        {0, 1, 0, 1}, // 10
        {1, 1, 0, 1}, // 11
        {0, 0, 1, 1}, // 12
        {1, 0, 1, 1}, // 13
        {0, 1, 1, 1}, // 14
        {1, 1, 1, 1}  // 15
    };
};

const double ldr_angle = 11.25;

#endif