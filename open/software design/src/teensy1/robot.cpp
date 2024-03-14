#include "main.h"

void Robot::setUpDribbler()
{
    pinMode(DRIBBLER_PWM, OUTPUT);

    analogWriteFrequency(DRIBBLER_PWM, 4000);

    analogWrite(DRIBBLER_PWM, 128);
    delay(3000);
}

void Robot::moveToTargetPose()
{
    x_diff = (target_pose.x - current_pose.x) == (target_pose.x - current_pose.x) ? (target_pose.x - current_pose.x) : x_diff;
    y_diff = (target_pose.y - current_pose.y) == (target_pose.y - current_pose.y) ? (target_pose.y - current_pose.y) : y_diff;
    angle_diff = atan2(x_diff, y_diff) == atan2(x_diff, y_diff) ? atan2(x_diff, y_diff) : angle_diff;
    double actual_angle_diff = degrees(angle_diff) - robot.current_pose.bearing;

    Serial.println(x_diff);

    if (actual_angle_diff > 360)
    {
        actual_angle_diff = actual_angle_diff - 360;
    }

    // if (atan2(x_diff, y_diff) == atan2(x_diff, y_diff)) {
    //     angle_diff = y_diff < 0 ? atan2(x_diff, y_diff) : atan2(x_diff, y_diff);
    // } else {
    //     angle_diff = angle_diff;
    // }
    // double vel = sqrt(x_diff * x_diff + y_diff * y_diff) / 1000;

    // print values
    //  Serial.print("(");
    //  Serial.print(robot.current_pose.x);
    //  Serial.print(", ");
    //  Serial.print(robot.current_pose.y);
    //  Serial.print(") ");
    //  Serial.print("x_diff: ");
    //  Serial.print(x_diff);
    //  Serial.print(" y_diff: ");
    //  Serial.print(y_diff);
    //  Serial.print(" angle_diff: ");
    //  Serial.println(angle_diff);

    double distance = sqrt(x_diff * x_diff + y_diff * y_diff);

    double correction = 0.0006 * distance + 0.0008 * (distance - prev_distance);

    base.move(correction, actual_angle_diff, target_angle);
    prev_distance = distance;
}