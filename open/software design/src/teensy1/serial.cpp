#include "main.h"

void onLayer1Received(const byte *buf, size_t size) {
    Layer1TxDataUnion data_received;
    
    // Don't continue if the payload is invalid
    if (size != sizeof(data_received)) return;
    
    std::copy(buf, buf + size, std::begin(data_received.bytes));

    // Serial.print("Received: ");
    // Serial.print(data_received.data.on_line);
    // Serial.print(", ");
    // Serial.println(data_received.data.target_angle);

    robot.on_line = data_received.data.on_line;
    robot.target_angle = data_received.data.target_angle;
    ball.in_catchment = data_received.data.ball_in_catchment;
}

void onImuReceived(const byte *buf, size_t size) {
    ImuTxDataUnion data_received;

    // Don't continue if the payload is invalid
    if (size != sizeof(data_received)) return;

    std::copy(buf, buf + size, std::begin(data_received.bytes));
    robot.current_pose.bearing = data_received.data.bearing;
}

void onTeensyReceived(const byte *buf, size_t size) {
    Teensy1RxDataUnion data_received;

    // Don't continue if the payload is invalid
    if (size != sizeof(data_received)) return;

    std::copy(buf, buf + size, std::begin(data_received.bytes));

    robot.current_pose.x = data_received.data.current_pose.x;
    robot.current_pose.y = data_received.data.current_pose.y;
    robot.target_pose = data_received.data.target_pose;
    layer_1_rx_data.data.kick = data_received.data.kick;
}

void Robot::setUpSerial() {
    #ifdef SERIAL_DEBUG
    Serial.begin(115200);
    while (!Serial) {}
    Serial.println("Debug serial connection established.");
    #endif

    Serial1.begin(115200);
    while (!Serial1) {}
    Layer1Serial.setStream(&Serial1);
    Layer1Serial.setPacketHandler(&onLayer1Received);
    #ifdef SERIAL_DEBUG
    Serial.println("Layer 1 serial connection established.");
    #endif

    Serial3.begin(115200);
    while (!Serial3) {}
    ImuSerial.setStream(&Serial3);
    ImuSerial.setPacketHandler(&onImuReceived);
    #ifdef SERIAL_DEBUG
    Serial.println("IMU serial connection established.");
    #endif

    Serial5.begin(115200);
    while (!Serial5) {}
    TeensySerial.setStream(&Serial5);
    TeensySerial.setPacketHandler(&onTeensyReceived);
    #ifdef SERIAL_DEBUG
    Serial.println("Teensy serial connection established.");
    #endif
}