#include "main.h"

void Robot::moveToTargetPose()
{
    x_diff = target_pose.x - current_pose.x;
    y_diff = target_pose.y - current_pose.y;
    angle_diff = atan2(x_diff, y_diff);
    double actual_angle_diff = correctBearing(degrees(angle_diff) - current_pose.bearing);

    double distance = sqrt(x_diff * x_diff + y_diff * y_diff);

    Serial.print(x_diff);
    Serial.print(" ");
    Serial.print(y_diff);
    Serial.print(" ");
    Serial.println(actual_angle_diff);

    double correction = 0.0007 * distance + 0.0008 * (distance - prev_distance);

    Serial.println(target_pose.bearing);

    move_data.speed = correction;
    move_data.target_angle = correctBearing(actual_angle_diff);
    move_data.target_bearing = correctBearing(target_pose.bearing);
    prev_distance = distance;
}

void Robot::trackLine(double speed, double angle, int offset)
{
    double correction = 0;

    offset = angle > 180 ? offset : -offset;

    double ldr_start_angle = line_data.ldr_angles[7 + offset][31 - (int)line_data.line_start_ldr] + current_pose.bearing;
    double ldr_end_angle = line_data.ldr_angles[7 + offset][31 - (int)line_data.line_end_ldr] + current_pose.bearing;

    double ldr_start_correct_angle = correctBearing(angle - ldr_start_angle);
    double ldr_end_correct_angle = correctBearing(angle - ldr_end_angle);

    if (ldr_start_correct_angle > 180)
    {
        ldr_start_correct_angle -= 360;
    }
    else if (ldr_start_correct_angle < -180)
    {
        ldr_start_correct_angle += 360;
    }

    if (ldr_end_correct_angle > 180)
    {
        ldr_end_correct_angle -= 360;
    }
    else if (ldr_end_correct_angle < -180)
    {
        ldr_end_correct_angle += 360;
    }

    if (abs(ldr_start_correct_angle) < abs(ldr_end_correct_angle))
    {
        correction = ldr_start_angle;
    }
    else
    {
        correction = ldr_end_angle;
    }

    move_data.speed = speed;
    move_data.target_angle = correctBearing(correction);
    move_data.target_bearing = 0;
}

void Robot::defendGoal()
{
    if (line_data.on_line && current_pose.y < 600)
    {
        target_pose = ball.current_pose;
        trackLine(min(0.003 * ball.distance_from_robot, 0.3), target_pose.bearing, 1);
    }
    else
    {
        // target_pose.x = 910;
        // target_pose.y = 300;
        // moveToTargetPose();
        move_data.speed = 0.3;
        move_data.target_angle = yellow_goal.current_pose.bearing;
        move_data.target_bearing = 0;
    }
}

void Robot::rotateToBall()
{
    target_pose = ball.current_pose;
    moveToTargetPose();
}

void Robot::orbitToBall()
{
    double offset, multiplier;

    if (ball.current_pose.bearing < 180)
    {
        offset = fmin(ball.current_pose.bearing * 1.05, 90);
    }
    else
    {
        offset = fmax((ball.current_pose.bearing - 360) * 1.05, -90);
    }

    double factor = 1 - (ball.distance_from_robot) / 2000;

    multiplier = fmin(1.1, 0.02 * exp(factor * 4.4));

    double speed = min(max(0.15, 0.0000003 * pow(ball.distance_from_robot, 2)), 0.3);
    double correction = correctBearing(ball.current_pose.bearing + multiplier * offset);

    if (line_data.on_line && abs(correction - line_data.initial_line_angle) > 90)
    {
        trackLine(speed, correction, 0);
    }
    else
    {
        move_data.speed = speed;
        move_data.target_angle = correction;
        move_data.target_bearing = 0;
    }
}

void Robot::orbitScore()
{
    if (robot.current_pose.y > 1900 || line_data.on_line)
    {
        move_data.speed = 0;
        move_data.target_angle = 0;
        move_data.target_bearing = blue_goal.current_pose.bearing;
        layer_1_rx_data.data.kick = true;
    }
    else
    {
        move_data.speed = 0.2;
        move_data.target_angle = 0;
        move_data.target_bearing = blue_goal.current_pose.bearing;
    }
}

void Robot::rotateScore()
{
    if (abs(target_pose.x - current_pose.x) < 10 && abs(target_pose.y - current_pose.y) < 10 && abs(target_pose.bearing - current_pose.bearing) < 1)
    {
        base.move(0, 0, 0);
        layer_1_rx_data.data.kick = true;
        robot.sendSerial();
    }
    else
    {
        target_pose.x = 910;
        target_pose.y = 1900;
        target_pose.bearing = 0;
        moveToTargetPose();
        layer_1_rx_data.data.kick = false;
    }
}