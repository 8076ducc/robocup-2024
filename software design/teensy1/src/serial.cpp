#include "main.h"
#include "motion.h"

void processLayer1Serial() {
    String identifier = LAYER1.readStringUntil(',');
    if (identifier == "l") {
        int angle = LAYER1.parseInt();
        move(1, angle, 0);
        #ifdef DEBUG
        Serial.print("move angle: ");
        Serial.println(LAYER1.parseInt());
        #endif
    } else if (identifier == "b") {
        have_ball = !have_ball;
    }
}

void processLidarSerial() {
    String identifier = LIDAR.readStringUntil(',');
}

void processImuSerial() {
    String identifier = IMU.readStringUntil(',');
}

void processTeensySerial() {
    String identifier = TEENSY.readStringUntil(',');
}