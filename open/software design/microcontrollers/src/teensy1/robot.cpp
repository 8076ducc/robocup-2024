#include "main.h"

void Robot::moveToTargetPose()
{
    x_diff = (target_pose.x - current_pose.x) == (target_pose.x - current_pose.x) ? (target_pose.x - current_pose.x) : x_diff;
    y_diff = (target_pose.y - current_pose.y) == (target_pose.y - current_pose.y) ? (target_pose.y - current_pose.y) : y_diff;
    angle_diff = atan2(x_diff, y_diff) == atan2(x_diff, y_diff) ? atan2(x_diff, y_diff) : angle_diff;
    double actual_angle_diff = correctBearing(degrees(angle_diff) - current_pose.bearing);

    double distance = sqrt(x_diff * x_diff + y_diff * y_diff);

    double correction = 0.0005 * distance + 0.0008 * (distance - prev_distance);

    base.move(correction, actual_angle_diff, target_pose.bearing);
    prev_distance = distance;
}

void Robot::defendGoal()
{
    target_pose = ball.current_pose;

    double kp = -4;
    double correction;

    if (target_pose.x - current_pose.x > 0)
    {
        correction = 90 + kp * (robot.chord_length - 12);
    }
    else if (target_pose.x - current_pose.x < 0)
    {
        correction = 270 - kp * (robot.chord_length - 12);
    }
    else
    {
        correction = 0;
    }

    // correction = 270 - kp * (robot.chord_length - 8);

    if (correction < 0)
    {
        correction = correction + 360;
    }
    else if (correction > 360)
    {
        correction = correction - 360;
    }

    double angular_corr = current_pose.bearing + (15.5 - robot.line_centre) * (360 / 32);

    if (angular_corr > 360)
    {
        angular_corr = angular_corr - 360;
    }
    else if (angular_corr < 0)
    {
        angular_corr = angular_corr + 360;
    }

    double speed = max(0.00055 * abs(target_pose.x - current_pose.x), 0.1);

    // Serial.println(speed);
    if (current_pose.bearing > 50 && current_pose.bearing < 180)
    {
        if (correction < 180)
        {
            speed = 0;
            angular_corr = current_pose.bearing;
        }
        else if (correction > 180)
            ;
    }
    else if (current_pose.bearing > 180 && current_pose.bearing < 310)
    {
        if (correction > 180)
        {
            speed = 0;
            angular_corr = current_pose.bearing;
        }
        else if (correction < 180)
            ;
    }

    base.move(speed, correction, angular_corr);
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

    double factor = 1 - (ball.distance_from_robot) / 2800;

    multiplier = fmin(1.1, 0.035 * exp(factor * 4.4));

    base.move(min(max(0.15, 0.00007 * ball.distance_from_robot), 0.3), correctBearing(ball.current_pose.bearing + multiplier * offset), 0);
}

void Robot::orbitScore()
{
    base.move(0.3, 0, yellow_goal.current_pose.bearing);
}

void Robot::rotateScore()
{
    if (abs(target_pose.x - current_pose.x) < 10 && abs(target_pose.y - current_pose.y) < 10 && abs(target_pose.bearing - current_pose.bearing) < 1)
    {
        dribbler.setSpeed(40);
        base.move(0, 0, 0);
        layer_1_rx_data.data.kick = true;
    }
    else
    {
        target_pose.x = 910;
        target_pose.y = 1900;
        target_pose.bearing = 0;
        moveToTargetPose();
    }
}