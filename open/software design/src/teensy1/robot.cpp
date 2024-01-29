#include "main.h"

void Robot::setUpDribbler() {
    pinMode(DRIBBLER_PWM, OUTPUT);

    analogWriteFrequency(DRIBBLER_PWM, 4000); 
  
    analogWrite(DRIBBLER_PWM, 128);
    delay(3000);
}

void Robot::moveToTargetPose() {
    double x_diff = target_pose.x - current_pose.x;
    double y_diff = target_pose.y - current_pose.y;
    double angle = atan2(y_diff, x_diff);
    double vel = 1 * sqrt(x_diff * x_diff + y_diff * y_diff);
    
    base.move(vel, angle);
}