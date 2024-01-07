#ifndef LIGHT_RING_H
#define LIGHT_RING_H

#include "main.h"

class LightRing {
    public:
        void setup();
        void calibrate();
        void printThresholds();
        void read();

        int ldr_readings[32];

        int ldr_min_readings[32];
        int ldr_max_readings[32];

        const int ldr_thresholds[32] = {
            0, // 0
            0, // 1
            0, // 2
            0, // 3
            0, // 4
            0, // 5
            0, // 6
            0, // 7
            0, // 8
            0, // 9
            0, // 10
            0, // 11
            0, // 12
            0, // 13
            0, // 14
            0, // 15
            0, // 16
            0, // 17
            0, // 18
            0, // 19
            0, // 20
            0, // 21
            0, // 22
            0, // 23
            0, // 24
            0, // 25
            0, // 26
            0, // 27
            0, // 28
            0, // 29
            0, // 30
            0, // 31
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