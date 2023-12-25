#include "serial.h"

void processLayer1Serial() {
    String identifier = LAYER1.readStringUntil(',');
    if (identifier == "l") {
        int angle = LAYER1.parseInt();
        robot.move(1, angle, 0);
        #ifdef DEBUG
        Serial.print("move angle: ");
        Serial.println(LAYER1.parseInt());
        #endif
    } else if (identifier == "b") {
        ball.in_catchment = !ball.in_catchment;
    }
}

void processImuSerial() {
    String identifier = IMU.readStringUntil(',');
    if (identifier == "x") {
        robot.current_pose.bearing = IMU.parseInt();
    }
}

void processTeensySerial() {
    String identifier = TEENSY.readStringUntil(',');
}