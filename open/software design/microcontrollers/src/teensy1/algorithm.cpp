#include "main.h"

void Robot::defendGoal()
{
    // if (line_data.on_line && (line_data.initial_line_angle > 90 && line_data.initial_line_angle < 270))
    if (line_data.on_line)
    {
        if (ball.detected)
        {
            target_pose = ball.current_pose;
            target_pose.x = bound(target_pose.x, 795, 1025);
        }
        else
        {
            target_pose.x = current_pose.x;
            target_pose.bearing = 0;
        }

        trackLineGoalie(min(0.002 * abs(current_pose.x - target_pose.x), 0.3), target_pose.bearing, 0);
    }
    else
    {
        if (ball.detected)
        {
            target_pose.x = bound(ball.current_pose.x, 795, 1025);
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

void Robot::orbitToBall(double bearing)
{
    if (ball.detected)
    {
        double bearing_from_robot = correctBearing(ball.current_pose.bearing - robot.current_pose.bearing);
        double offset, multiplier;

        if (bearing_from_robot < 180)
        {
            offset = fmin(bearing_from_robot * 1.05, 90);
        }
        else
        {
            offset = fmax((bearing_from_robot - 360) * 1.05, -90);
        }

#ifdef BOT_1
        double factor = 1.1 - (ball.distance_from_robot) / 2340;

        multiplier = fmin(1.1, 0.011 * exp(factor * 3.7));

#else
        double factor = 1.1 - (ball.distance_from_robot) / 2340;

        multiplier = fmin(1.1, 0.03 * exp(factor * 3.7));

#endif

        // double speed = min(max(0.25, 0.00001 * pow(ball.distance_from_robot, 2)), 0.3);
        // double speed = min(0.00xx1 * ball.distance_from_robot, 0.45);
        double speed;
        if (ball.distance_from_robot > 500)
        {
            speed = 0.35;
        }
        else
        {
            speed = 0.25;
        }

        double correction = correctBearing(bearing_from_robot + multiplier * offset);

        if (line_data.on_line)
        {
            if (abs(correction - line_data.initial_line_angle) < 90 && (line_data.initial_line_angle > 20 && line_data.initial_line_angle < 330))
            {
                // trackLine(speed, correction, 7);
                rejectLine(bearing);
            }
            else
            {
                Serial.print("correction: ");
                Serial.println(correction);
                Serial.print("line angle: ");
                Serial.println(line_data.initial_line_angle);
                rejectLine(bearing);
            }
        }
        else
        {
            move_data.speed = speed;
            move_data.target_angle = correction;
            move_data.target_bearing = bearing;
            move_data.ema_constant = 0.0002;
        }
    }
    else
    {
        target_pose.x = 910;
        target_pose.y = 1215;
        moveToTargetPose();
    }
}

void Robot::orbitScore()
{
    double target_bearing = robot.dip_4_on ? yellow_goal.current_pose.bearing : blue_goal.current_pose.bearing;
    target_bearing = blue_goal.current_pose.bearing;
    if (line_data.on_line)
    {
        rejectLine(target_bearing);
        // layer_1_rx_data.data.kick = true;
    }
    else
    {
        move_data.speed = 0.5;
        move_data.target_angle = 0;
        move_data.target_bearing = target_bearing;
        move_data.ema_constant = 0.00017;
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