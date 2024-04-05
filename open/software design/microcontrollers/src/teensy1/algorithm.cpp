#include "main.h"

void Robot::defendGoal()
{
    // if (line_data.on_line && (line_data.initial_line_angle > 90 && line_data.initial_line_angle < 270))
    if (line_data.on_line)
    {
        if (ball.detected)
        {
            target_pose = ball.current_pose;
            target_pose.x = bound(target_pose.x, 595, 1225);
        }
        else
        {
            target_pose.x = current_pose.x;
            target_pose.bearing = 0;
        }

        trackLineGoalie(min(0.002 * abs(current_pose.x - target_pose.x), 0.4), target_pose.bearing, 0);
    }
    else
    {
        if (ball.detected)
        {
            target_pose.x = ball.current_pose.x;
        }
        else
        {
            target_pose.x = 910;
        }
        target_pose.y = 250;
        target_pose.bearing = 0;
        moveToTargetPose();
    }
}

void Robot::rotateToBall()
{
    target_pose = ball.current_pose;
    target_pose.bearing -= robot.current_pose.bearing;
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

    double factor = 1.1 - (ball.distance_from_robot) / 2340;

    multiplier = fmin(1.1, 0.013 * exp(factor * 4.1));

    double speed = min(max(0.15, 0.000005 * pow(ball.distance_from_robot, 2)), 0.3);
    double correction = correctBearing(ball.current_pose.bearing + multiplier * offset);

    if (line_data.on_line)
    {
        if (abs(correction - line_data.initial_line_angle) < 90 && (line_data.initial_line_angle > 20 && line_data.initial_line_angle < 330))
        {
            trackLine(speed, correction, 7);
        }
        else
        {
            Serial.print("correction: ");
            Serial.println(correction);
            Serial.print("line angle: ");
            Serial.println(line_data.initial_line_angle);
            rejectLine();
        }
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
    if (robot.current_pose.y > 1800 || line_data.on_line)
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

void Robot::moveToNeutralPoint(int neutral_point, bool behind_point)
{
    int neutral_points[5][2] = {{0, 0}, {0, 0}, {915, 1215}, {0, 0}, {0, 0}};
    target_pose.x = neutral_points[neutral_point][0];
    target_pose.y = behind_point ? neutral_points[neutral_point][1] - 200 : neutral_points[neutral_point][1];
    target_pose.bearing = 0;

    moveToTargetPose();
}