#include <main.h>

int ldr_readings[32];

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
    
    float approach_angle = (line_end - line_start) < 16 ? (line_start + (line_end - line_start) / 2) * ldr_angle : 360 - ((line_end - line_start) * ldr_angle) - ((32 - line_end) * ldr_angle);
    TEENSY.print("l");
    TEENSY.println(approach_angle);
}

void CalibLight() {
    
}