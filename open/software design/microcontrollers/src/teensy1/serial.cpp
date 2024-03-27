#include "main.h"

void onLayer1Received(const byte *buf, size_t size)
{
    Layer1TxDataUnion data_received;

    // Don't continue if the payload is invalid
    if (size != sizeof(data_received))
        return;

    std::copy(buf, buf + size, std::begin(data_received.bytes));

    robot.on_line = data_received.data.on_line;
    if (robot.on_line)
    {
        robot.target_angle = data_received.data.target_angle;
    }
    else
    {
        robot.target_angle = 0;
    }
    ball.in_catchment = data_received.data.ball_in_catchment;
    robot.line_centre = data_received.data.line_centre;
    robot.chord_length = data_received.data.chord_length;
}

void onImuReceived(const byte *buf, size_t size)
{
    ImuTxDataUnion data_received;

    // Don't continue if the payload is invalid
    if (size != sizeof(data_received))
        return;

    std::copy(buf, buf + size, std::begin(data_received.bytes));

    teensy_1_tx_data.data.bearing = data_received.data.bearing;

    robot.sendSerial();
}

void onTeensyReceived(const byte *buf, size_t size)
{
    Teensy1RxDataUnion data_received;

    // Don't continue if the payload is invalid
    if (size != sizeof(data_received))
        return;

    std::copy(buf, buf + size, std::begin(data_received.bytes));

    if (data_received.data.lidar_detected)
    {
        digitalWriteFast(LIDAR_PWM, HIGH);
    }
    else
    {
        digitalWriteFast(LIDAR_PWM, LOW);
    }

    robot.current_pose = data_received.data.current_pose;
    robot.target_pose = data_received.data.target_pose;

    ball.distance_from_robot = sqrt(pow(data_received.data.target_pose.x, 2) + pow(data_received.data.target_pose.y, 2));

    // Serial.print(ball.current_pose.bearing);
    // Serial.print(" ");
    // Serial.println(ball.distance_from_robot);

    yellow_goal.current_pose.bearing = robot.current_pose.bearing + data_received.data.yellow_goal.current_pose.bearing;
    blue_goal.current_pose.bearing = robot.current_pose.bearing + data_received.data.blue_goal.current_pose.bearing;
}

void Robot::setUpSerial()
{
#ifdef SERIAL_DEBUG
    Serial.begin(1000000);
    while (!Serial)
    {
    }
    Serial.println("Debug serial connection established.");
#endif

    Serial1.begin(layer_1_serial_baud);
    while (!Serial1)
    {
    }
    Layer1Serial.setStream(&Serial1);
    Layer1Serial.setPacketHandler(&onLayer1Received);
#ifdef SERIAL_DEBUG
    Serial.println("Layer 1 serial connection established.");
#endif

    Serial3.begin(imu_serial_baud);
    while (!Serial3)
    {
    }
    ImuSerial.setStream(&Serial3);
    ImuSerial.setPacketHandler(&onImuReceived);
#ifdef SERIAL_DEBUG
    Serial.println("IMU serial connection established.");
#endif

    Serial5.begin(teensy1_serial_baud);
    while (!Serial5)
    {
    }
    TeensySerial.setStream(&Serial5);
    TeensySerial.setPacketHandler(&onTeensyReceived);
#ifdef SERIAL_DEBUG
    Serial.println("Teensy serial connection established.");
#endif
}

void Robot::updateSerial()
{
    Layer1Serial.update();
    ImuSerial.update();
    TeensySerial.update();
}

void Robot::sendSerial()
{
    if (Serial1.availableForWrite())
    {
        Layer1Serial.send(layer_1_rx_data.bytes, sizeof(layer_1_rx_data.bytes));
    }

    if (Serial5.availableForWrite())
    {
        TeensySerial.send(teensy_1_tx_data.bytes, sizeof(teensy_1_tx_data.bytes));
    }
}