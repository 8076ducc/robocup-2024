#include "main.h"

void onLayer1Received(const byte *buf, size_t size)
{
    Layer1TxDataUnion data_received;

    // Don't continue if the payload is invalid
    if (size != sizeof(data_received))
        return;

    //Serial.println("received");
    std::copy(buf, buf + size, std::begin(data_received.bytes));

    robot.on_line = data_received.data.on_line;
    if (robot.on_line)
    {
        robot.target_angle = data_received.data.line_angle;
    }
    else
    {
        robot.target_angle = 0;
    }
    ball.in_catchment = data_received.data.ball_in_catchment;
    robot.line_centre = data_received.data.line_centre;
    robot.chord_length = data_received.data.chord_length;
    // Serial.print("angle: ");
    // Serial.println(data_received.data.line_angle);
    //Serial.print(data_received.data.on_line);
}

void onSTMReceived(const byte *buf, size_t size) {
    STMTxDataUnion data_received;

    // Don't continue if the payload is invalid
    if (size != sizeof(data_received))
        return;

    std::copy(buf, buf + size, std::begin(data_received.bytes));

    ball.distance_from_robot = data_received.data.ball_dist;
    ball.bearing_from_robot = correctBearing(data_received.data.ball_bearing + 10);
    ball.current_pose.bearing = correctBearing(ball.bearing_from_robot + robot.current_pose.bearing);
    
    // for (int i = 0; i < 24; ++i) {
    //     Serial.print(data_received.data.readings[i]);
    //     Serial.print(" ");
    // }
    // Serial.println("end");
    // Serial.print("strength: ");
    // Serial.println(ball.distance_from_robot);
    // Serial.print(" angle: ");
    // Serial.println(ball.bearing_from_robot);
}

void onCamReceived()
{
    CamTxDataUnion data_received;
    double tmpArray[4];
    while (Serial2.available() >= 6) {
        uint8_t syncByte = Serial2.read();
        if (syncByte == 42) {
            for (int i = 0; i < 8; i++) {
                data_received.bytes[i] = Serial2.read();
            }
        }
        for (int i = 0; i < 4; i++) {
            tmpArray[i] = (double)(data_received.vals[i]);
        }
        data_received.data.yellow_goal_x = sin(radians(tmpArray[1]/100)) * tmpArray[0]/10;
        data_received.data.yellow_goal_y = cos(radians(tmpArray[1]/100)) * tmpArray[0]/10;
        data_received.data.blue_goal_x = sin(radians(tmpArray[3]/100)) * tmpArray[2]/10;
        data_received.data.blue_goal_y = cos(radians(tmpArray[3]/100)) * tmpArray[2]/10;

        yellow_goal.current_pose.bearing = robot.current_pose.bearing + degrees(atan2(data_received.data.yellow_goal_x, data_received.data.yellow_goal_y));
        blue_goal.current_pose.bearing = robot.current_pose.bearing + degrees(atan2(data_received.data.blue_goal_x, data_received.data.blue_goal_y));

        //cannot send negative numbers over uart
        // Serial.print("yellow: ");
        // Serial.print(data_received.data.yellow_goal_x);
        // Serial.print(" ");
        // Serial.println(data_received.data.yellow_goal_y);
        // Serial.print("blue: ");
        // Serial.print(data_received.data.blue_goal_x);
        // Serial.print(" ");
        // Serial.println(data_received.data.blue_goal_y);
        // Serial.print(tmpArray[0]/10);
        // Serial.print(" ");
        // Serial.println(tmpArray[1]/100);
        robot.getCameraPose(data_received.data.yellow_goal_x, data_received.data.yellow_goal_y, data_received.data.blue_goal_x, data_received.data.blue_goal_y);
    }
}

void Robot::setUpSerial()
{
#ifdef SERIAL_DEBUG
    Serial.begin(512000);
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

#ifdef SERIAL_DEBUG
    Serial.println("Cam serial connection established.");
#endif
}

void Robot::updateSerial()
{
    Layer1Serial.update();
    STMSerial.update();
    onCamReceived();
}

void Robot::sendSerial()
{
    // if (Serial1.availableForWrite())
    // {
    //     Layer1Serial.send(layer_1_rx_data.bytes, sizeof(layer_1_rx_data.bytes));
    // }
}