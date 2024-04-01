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
        1547, // 0
        1489, // 1
        1503, // 2
        1559, // 3
        1555, // 4
        1580, // 5
        1589, // 6
        1630, // 7
        1627, // 8
        1614, // 9
        1675, // 10
        1618, // 11
        1589, // 12
        1564, // 13
        1587, // 14
        1516, // 15
        1594, // 16
        1493, // 17
        1521, // 18
        1572, // 19
        1536, // 20
        1521, // 21
        1580, // 22
        1568, // 23
        1617, // 24
        1588, // 25
        1629, // 26
        1561, // 27
        1595, // 28
        1632, // 29
        1502, // 30
        1459, // 31
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