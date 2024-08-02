#include "main.h"

double getRegressedDistance(double distance)
{
    double regressed_distance = pow(0.0009606260 * distance, 3) - pow(0.1161190388 * distance, 2) + 8.8058432711 * distance;
    return regressed_distance;
}

void onCam2Received(const byte *buf, size_t size)
{
    CamTxDataUnion data_received;

    // Serial.println("Received data");

    // Don't continue if the payload is invalid
    if (size != sizeof(data_received))
        return;

    std::copy(buf, buf + size, std::begin(data_received.bytes));

    // Serial.print("Yellow goal: ");
    // Serial.print(data_received.data.yellow_goal_detected);
    // Serial.print(" ");
    // Serial.print(data_received.data.yellow_goal_x);
    // Serial.print(" ");
    // Serial.print(data_received.data.yellow_goal_y);
    // Serial.print(" ");
    // Serial.print(" Blue goal: ");
    // Serial.print(data_received.data.blue_goal_detected);
    // Serial.print(" ");
    // Serial.print(data_received.data.blue_goal_x);
    // Serial.print(" ");
    // Serial.print(data_received.data.blue_goal_y);
    // Serial.print(" Ball: ");
    // Serial.print(data_received.data.ball_detected);
    // Serial.print(" ");
    // Serial.print(data_received.data.ball_x);
    // Serial.print(" ");
    // Serial.println(data_received.data.ball_y);

    // Serial.print("fps: ");
    // Serial.println(data_received.data.fps);

    if (data_received.data.yellow_goal_detected && data_received.data.blue_goal_detected)
    {
        Serial.println("Both goals detected");
        robot.getCameraPose(data_received.data.yellow_goal_x, data_received.data.yellow_goal_y, data_received.data.blue_goal_x, data_received.data.blue_goal_y);
        yellow_goal.current_pose.bearing = robot.current_pose.bearing + degrees(atan2(data_received.data.yellow_goal_x, data_received.data.yellow_goal_y));
        blue_goal.current_pose.bearing = robot.current_pose.bearing + degrees(atan2(data_received.data.blue_goal_x, data_received.data.blue_goal_y));
    }
    else if (data_received.data.yellow_goal_detected)
    {
        Serial.println("Yellow goal detected");
        yellow_goal.current_pose.bearing = robot.current_pose.bearing + degrees(atan2(data_received.data.yellow_goal_x, data_received.data.yellow_goal_y));
        robot.getSingleCameraPose(data_received.data.yellow_goal_x, data_received.data.yellow_goal_y);
    }
    else if (data_received.data.blue_goal_detected)
    {
        Serial.println("Blue goal detected");
        blue_goal.current_pose.bearing = robot.current_pose.bearing + degrees(atan2(data_received.data.blue_goal_x, data_received.data.blue_goal_y));
        robot.getSingleCameraPose(data_received.data.blue_goal_x, data_received.data.blue_goal_y);
    }

    if (data_received.data.ball_detected)
    {
        double ball_relative_bearing = degrees(atan2(data_received.data.ball_x, data_received.data.ball_y));

        ball.current_pose.bearing = correctBearing(ball_relative_bearing + robot.current_pose.bearing);
        ball.current_pose.x = bound(robot.current_pose.x + data_received.data.ball_x, 0, 1820);
        ball.current_pose.y = bound(robot.current_pose.y + data_received.data.ball_y, 0, 2430);
        ball.detected = true;
    }
    else
    {
        ball.detected = false;
    }

    teensy_1_rx_data.data.ball_detected = ball.detected;
}

unsigned long last_bt_received_time = 0;

void onBtReceived(const byte *buf, size_t size)
{
    BtTxDataUnion data_received;

    // Don't continue if the payload is invalid
    if (size != sizeof(data_received))
        return;

    std::copy(buf, buf + size, std::begin(data_received.bytes));
    // Serial.print("other robot detected: ");
    // Serial.println(data_received.data.robot_detected);
    // Serial.print(data_received.data.robot_pose.x);
    // Serial.print(" ");
    teensy_1_rx_data.data.robot_detected = data_received.data.robot_detected;
    last_bt_received_time = millis();
}

void onTeensyReceived(const byte *buf, size_t size)
{
    Teensy1TxDataUnion data_received;

    // Don't continue if the payload is invalid
    if (size != sizeof(data_received))
        return;

    std::copy(buf, buf + size, std::begin(data_received.bytes));

    robot.current_pose.bearing = data_received.data.bearing;
}

void Robot::setUpSerial()
{
#ifdef DEBUG
    Serial.begin(serial_monitor_baud);
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
    Serial.println("Bluetooth serial connection established.");
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
    if (Serial2.availableForWrite() > sizeof(cam_rx_data.bytes))
    {
        Cam2Serial.send(cam_rx_data.bytes, sizeof(cam_rx_data.bytes));
    }

    if (Serial4.availableForWrite() > sizeof(bt_tx_data.bytes))
    {
        BtSerial.send(bt_rx_data.bytes, sizeof(bt_rx_data.bytes));
    }

    if (Serial5.availableForWrite() > sizeof(teensy_1_rx_data.bytes))
    {
        teensy_1_rx_data.data.current_pose = current_pose;
        teensy_1_rx_data.data.target_pose = ball.current_pose;

#ifdef BOT1
        if (millis() - last_bt_received_time > 1000)
        {
            teensy_1_rx_data.data.robot_detected = false;
        }
#endif

        TeensySerial.send(teensy_1_rx_data.bytes, sizeof(teensy_1_rx_data.bytes));
    }
}
