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
        1657, // 0
        1594, // 1
        1583, // 2
        1683, // 3
        1639, // 4
        1680, // 5
        1635, // 6
        1750, // 7
        1742, // 8
        1688, // 9
        1780, // 10
        1681, // 11
        1690, // 12
        1627, // 13
        1684, // 14
        1661, // 15
        1706, // 16
        1623, // 17
        1609, // 18
        1690, // 19
        1660, // 20
        1555, // 21
        1707, // 22
        1613, // 23
        1784, // 24
        1732, // 25
        1786, // 26
        1681, // 27
        1732, // 28
        1737, // 29
        1586, // 30
        1632, // 31
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