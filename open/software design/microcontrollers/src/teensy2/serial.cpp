#include "main.h"

double getRegressedDistance(double distance)
{
    double regressed_distance = pow(0.0009606260 * distance, 3) - pow(0.1161190388 * distance, 2) + 8.8058432711 * distance;
    return regressed_distance;
}

void onCam2Received(const byte *buf, size_t size)
{
    CamTxDataUnion data_received;

    // Don't continue if the payload is invalid
    if (size != sizeof(data_received))
        return;

    std::copy(buf, buf + size, std::begin(data_received.bytes));

    if (data_received.data.ball_detected)
    {
        double ball_relative_x = getRegressedDistance(data_received.data.ball_x);
        double ball_relative_y = getRegressedDistance(data_received.data.ball_y);
        double distance_from_ball = sqrt(pow(teensy_1_rx_data.data.target_pose.x, 2) + pow(teensy_1_rx_data.data.target_pose.y, 2));
        double ball_relative_bearing = degrees(atan2(data_received.data.ball_x, -data_received.data.ball_y));

        ball.current_pose.x = robot.current_pose.x + cos(radians(ball_relative_bearing + robot.current_pose.bearing)) * distance_from_ball;
        ball.current_pose.y = robot.current_pose.y + sin(radians(ball_relative_bearing + robot.current_pose.bearing)) * distance_from_ball;
        ball.current_pose.bearing = ball_relative_bearing + robot.current_pose.bearing;
    } else {
        // compute ball's new bearing relative to robot based on new robot position
        double ball_relative_x = ball.current_pose.x - robot.current_pose.x;
        double ball_relative_y = ball.current_pose.y - robot.current_pose.y;
        double distance_from_ball = sqrt(pow(ball_relative_x, 2) + pow(ball_relative_y, 2));
        double ball_relative_bearing = degrees(atan2(ball_relative_x, -ball_relative_y));
    }

    teensy_1_rx_data.data.yellow_goal.current_pose.x = data_received.data.yellow_goal_x;
    teensy_1_rx_data.data.yellow_goal.current_pose.y = data_received.data.yellow_goal_y;
    double distance_from_yellow_goal = sqrt(pow(data_received.data.yellow_goal_x, 2) + pow(data_received.data.yellow_goal_y, 2));
    teensy_1_rx_data.data.yellow_goal.current_pose.bearing = degrees(atan2(data_received.data.yellow_goal_x, -data_received.data.yellow_goal_y));

    teensy_1_rx_data.data.blue_goal.current_pose.x = data_received.data.blue_goal_x;
    teensy_1_rx_data.data.blue_goal.current_pose.y = data_received.data.blue_goal_y;
    double distance_from_blue_goal = sqrt(pow(data_received.data.blue_goal_x, 2) + pow(data_received.data.blue_goal_y, 2));
    teensy_1_rx_data.data.blue_goal.current_pose.bearing = degrees(atan2(data_received.data.blue_goal_x, -data_received.data.blue_goal_y));
}

void onBtReceived(const byte *buf, size_t size)
{
    BtTxDataUnion data_received;

    // Don't continue if the payload is invalid
    if (size != sizeof(data_received))
        return;

    std::copy(buf, buf + size, std::begin(data_received.bytes));
}

void onTeensyReceived(const byte *buf, size_t size)
{
    Teensy1TxDataUnion data_received;

    // Don't continue if the payload is invalid
    if (size != sizeof(data_received))
        return;

    std::copy(buf, buf + size, std::begin(data_received.bytes));
}

void Robot::setUpSerial()
{
#ifdef DEBUG
    Serial.begin(115200);
    while (!Serial)
    {
    }
    Serial.println("Debug serial connection established.");
#endif

    Serial2.begin(cam_serial_baud);
    while (!Serial2)
    {
    }
    Cam2Serial.setStream(&Serial2);
    Cam2Serial.setPacketHandler(&onCam2Received);
#ifdef DEBUG
    Serial.println("IMU serial connection established.");
#endif

    Serial4.begin(bt_serial_baud);
    while (!Serial4)
    {
    }
    BtSerial.setStream(&Serial4);
    BtSerial.setPacketHandler(&onBtReceived);
#ifdef DEBUG
    Serial.println("Teensy serial connection established.");
#endif

    Serial5.begin(teensy1_serial_baud);
    while (!Serial5)
    {
    }
    TeensySerial.setStream(&Serial5);
    TeensySerial.setPacketHandler(&onTeensyReceived);
#ifdef DEBUG
    Serial.println("Teensy serial connection established.");
#endif
}

void Robot::updateSerial()
{
    Cam2Serial.update();
    BtSerial.update();
    TeensySerial.update();
}

void Robot::sendSerial()
{
    if (Serial2.availableForWrite())
    {
        Cam2Serial.send(cam_rx_data.bytes, sizeof(cam_rx_data.bytes));
    }

    if (Serial4.availableForWrite())
    {
        BtSerial.send(bt_rx_data.bytes, sizeof(bt_rx_data.bytes));
    }

    if (Serial5.availableForWrite())
    {
        teensy_1_rx_data.data.current_pose = current_pose;
        teensy_1_rx_data.data.target_pose = ball.current_pose;
        TeensySerial.send(teensy_1_rx_data.bytes, sizeof(teensy_1_rx_data.bytes));
    }
}
