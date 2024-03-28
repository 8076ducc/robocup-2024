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

void onSTMReceived(const byte *buf, size_t size) {
    STMTxDataUnion data_received;

    // Don't continue if the payload is invalid
    if (size != sizeof(data_received))
        return;

    std::copy(buf, buf + size, std::begin(data_received.bytes));

    ball.distance_from_robot = data_received.data.ball_dist;
    ball.bearing_from_robot = data_received.data.ball_bearing;
    
    // for (int i = 0; i < 24; ++i) {
    //     Serial.print(data_received.data.readings[i]);
    //     Serial.print(" ");
    // }
    // Serial.println("end");
    // Serial.print("strongest: ");
    // Serial.println(data_received.data.indices[0]);
}

void onCamReceived(const byte *buf, size_t size)
{
    CamTxDataUnion data_received;

    // Don't continue if the payload is invalid
    if (size != sizeof(data_received))
        return;

    std::copy(buf, buf + size, std::begin(data_received.bytes));

    if (data_received.data.yellow_goal_detected && data_received.data.blue_goal_detected)
    {
        robot.getCameraPose(data_received.data.yellow_goal_x, data_received.data.yellow_goal_y, data_received.data.blue_goal_x, data_received.data.blue_goal_y);
    }

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
    while (!Serial1);
    Layer1Serial.setStream(&Serial1);
    Layer1Serial.setPacketHandler(&onLayer1Received);
#ifdef SERIAL_DEBUG
    Serial.println("Layer 1 serial connection established.");
#endif

    Serial3.begin(stm_serial_baud);
    while(!Serial3);
    STMSerial.setStream(&Serial3);
    STMSerial.setPacketHandler(&onSTMReceived);

#ifdef SERIAL_DEBUG
    Serial.println("STM serial connection established.");
#endif

    Serial2.begin(cam_serial_baud);
    while(!Serial2);
    CamSerial.setStream(&Serial2);
    CamSerial.setPacketHandler(&onCamReceived);

}

void Robot::updateSerial()
{
    Layer1Serial.update();
    STMSerial.update();
    CamSerial.update();
}

void Robot::sendSerial()
{
    if (Serial1.availableForWrite())
    {
        Layer1Serial.send(layer_1_rx_data.bytes, sizeof(layer_1_rx_data.bytes));
    }
}