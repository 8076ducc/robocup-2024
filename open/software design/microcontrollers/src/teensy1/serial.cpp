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

    ldr_reading = data_received.data.ldr_reading;
    chord_length = data_received.data.chord_length;
    line_centre = data_received.data.line_centre;
}

void onImuReceived(const byte *buf, size_t size)
{
    ImuTxDataUnion data_received;

    // Don't continue if the payload is invalid
    if (size != sizeof(data_received))
        return;

    std::copy(buf, buf + size, std::begin(data_received.bytes));
    robot.current_pose.bearing = data_received.data.bearing;
}

double last_cam_time = 0;

void onTeensyReceived(const byte *buf, size_t size)
{
    Teensy1RxDataUnion data_received;

    // Don't continue if the payload is invalid
    if (size != sizeof(data_received))
        return;

    std::copy(buf, buf + size, std::begin(data_received.bytes));

    // robot.current_pose.x = data_received.data.current_pose.x;
    // robot.current_pose.y = data_received.data.current_pose.y;
    // robot.target_pose = data_received.data.target_pose;
    // robot.target_pose.x = robot.current_pose.x - data_received.data.target_pose.x;
    // robot.target_pose.y = robot.current_pose.y + data_received.data.target_pose.y;
    // robot.target_pose.bearing = 0;

    double angle = degrees(atan2(data_received.data.target_pose.x, -data_received.data.target_pose.y));
    // robot.target_pose.bearing = robot.current_pose.bearing + data_received.data.target_pose.bearing;
    // Serial.print(angle);
    robot.target_pose.bearing = robot.current_pose.bearing + angle;

    if (robot.target_pose.bearing > 360)
    {
        robot.target_pose.bearing = robot.target_pose.bearing - 360;
    }
    else if (robot.target_pose.bearing < 0)
    {
        robot.target_pose.bearing = robot.target_pose.bearing + 360;
    }
    double distance_from_ball = sqrt(pow(data_received.data.target_pose.x, 2) + pow(data_received.data.target_pose.y, 2));

    Serial.println(distance_from_ball);

    robot.target_pose.x = robot.current_pose.x + sin(radians(robot.target_pose.bearing)) * distance_from_ball;
    robot.target_pose.y = robot.current_pose.y + cos(radians(robot.target_pose.bearing)) * distance_from_ball;
    // robot.target_pose.x = robot.current_pose.x;
    // robot.target_pose.y = robot.current_pose.y;
    // Serial.print("Target pose: ");
    // Serial.print(robot.target_pose.x);
    // Serial.print(", ");
    // Serial.print(robot.target_pose.y);
    // Serial.print(", ");
    // Serial.println(robot.target_pose.bearing);
    // layer_1_rx_data.data.kick = data_received.data.kick;
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

    Serial1.begin(serial_baud);
    while (!Serial1)
    {
    }
    Layer1Serial.setStream(&Serial1);
    Layer1Serial.setPacketHandler(&onLayer1Received);
#ifdef SERIAL_DEBUG
    Serial.println("Layer 1 serial connection established.");
#endif

    Serial3.begin(serial_baud);
    while (!Serial3)
    {
    }
    ImuSerial.setStream(&Serial3);
    ImuSerial.setPacketHandler(&onImuReceived);
#ifdef SERIAL_DEBUG
    Serial.println("IMU serial connection established.");
#endif

    Serial5.begin(1000000);
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