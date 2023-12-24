#include <main.h>

int ldr_readings[32];

int mux_signals[16][4] = {
    {0, 0, 0, 0}, // 1
	{1, 0, 0, 0}, // 2
	{0, 1, 0, 0}, // 3
	{1, 1, 0, 0}, // 4
	{0, 0, 1, 0}, // 5
	{1, 0, 1, 0}, // 6
	{0, 1, 1, 0}, // 7
	{1, 1, 1, 0}, // 8
	{0, 0, 0, 1}, // 9
	{1, 0, 0, 1}, // 10
	{0, 1, 0, 1}, // 11
	{1, 1, 0, 1}, // 12
	{0, 0, 1, 1}, // 13
	{1, 0, 1, 1}, // 14
	{0, 1, 1, 1}, // 15
	{1, 1, 1, 1}  // 16
};

int ldr_thresholds[32] = {
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
    0, // 32
};

void detectLight() {
    int line_start = 0, line_end = 0;

    for (int i = 0; i < 16; i++) {
        digitalWrite(S0, mux_signals[i][0]);
        digitalWrite(S1, mux_signals[i][1]);
        digitalWrite(S2, mux_signals[i][2]);
        digitalWrite(S3, mux_signals[i][3]);
        
        ldr_readings[i] = analogRead(MUX1);
        ldr_readings[i + 16] = analogRead(MUX2);

        if (ldr_readings[i] > ldr_thresholds[i]) {
            if (i < line_start) {
                line_start = i;
            } else if (i > line_end) {
                line_end = i;
            }
        }

        if (ldr_readings[i + 16] > ldr_thresholds[i + 16]) {
            if (i < line_start) {
                line_start = i + 16;
            } else if (i > line_end) {
                line_end = i + 16;
            }
        }
    }
    
    int approach_angle = (line_end - line_start) * (360 / 32) < 180 ? (line_start + (line_end - line_start) / 2) * (360 / 32) : 360 - ((line_end - line_start) / 2 - line_end + 32) * (360 / 32);

    TEENSY.print("l");
    TEENSY.println(approach_angle);
}

void CalibLight() {
    
}