#include "main.h"

void onCam1Received(const byte *buf, size_t size)
{
    Cam1TxDataUnion data_received;

    // Don't continue if the payload is invalid
    if (size != sizeof(data_received))
        return;

    std::copy(buf, buf + size, std::begin(data_received.bytes));
}

void onCam2Received(const byte *buf, size_t size)
{
    Cam2TxDataUnion data_received;

    // Don't continue if the payload is invalid
    if (size != sizeof(data_received))
        return;

    std::copy(buf, buf + size, std::begin(data_received.bytes));
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

    bt_rx_data.data.robot_pose.x = data_received.data.current_pose.x;
    bt_rx_data.data.robot_pose.y = data_received.data.current_pose.y;
    bt_rx_data.data.robot_pose.bearing = data_received.data.current_pose.bearing;

    Serial.print("Robot pose: ");
    Serial.print(bt_rx_data.data.robot_pose.x);
    Serial.print(", ");
    Serial.print(bt_rx_data.data.robot_pose.y);
    Serial.print(", ");
    Serial.println(bt_rx_data.data.robot_pose.bearing);
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

    Serial1.begin(115200);
    while (!Serial1)
    {
    }
    Cam1Serial.setStream(&Serial1);
    Cam1Serial.setPacketHandler(&onCam1Received);
#ifdef DEBUG
    Serial.println("Layer 1 serial connection established.");
#endif

    Serial2.begin(115200);
    while (!Serial2)
    {
    }
    Cam2Serial.setStream(&Serial2);
    Cam2Serial.setPacketHandler(&onCam2Received);
#ifdef DEBUG
    Serial.println("IMU serial connection established.");
#endif

    Serial4.begin(115200);
    while (!Serial4)
    {
    }
    BtSerial.setStream(&Serial4);
    BtSerial.setPacketHandler(&onBtReceived);
#ifdef DEBUG
    Serial.println("Teensy serial connection established.");
#endif

    Serial5.begin(115200);
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
    Cam1Serial.update();
    Cam2Serial.update();
    BtSerial.update();
    TeensySerial.update();
}

void Robot::sendSerial()
{
    if (Serial4.availableForWrite())
    {
        BtSerial.send(bt_rx_data.bytes, sizeof(bt_rx_data.bytes));
    }

    if (Serial5.availableForWrite())
    {
        TeensySerial.send(teensy_1_rx_data.bytes, sizeof(teensy_1_rx_data.bytes));
    }
}
