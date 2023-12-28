#include "main.h"
#include "serial.h"

PacketSerial LAYER1;
PacketSerial LIDAR;
PacketSerial IMU;
PacketSerial TEENSY;

Robot robot;
Ball ball;

float degToRad(float deg) {
    return deg * M_PI / 180;
}

void onPacketReceived(const byte *buf, size_t size) {
    // Read the payload
    Layer1TxDataUnion payload;
    // Don't continue if the payload is invalid
    if (size != sizeof(payload)) return;
    std::copy(buf, buf + size, std::begin(payload.bytes));

    Serial.print(payload.data.on_line);
}

void setup() {
  pinModeFast(LED, OUTPUT);
  digitalWriteFast(LED, HIGH);

  robot.setUpMotors();
  robot.setUpDribbler();

  pinMode(LIDAR_PWM, OUTPUT);

  pinMode(0, OUTPUT);
  pinMode(1, OUTPUT);
  pinMode(2, OUTPUT);
  pinMode(3, OUTPUT);
  pinMode(4, OUTPUT);
  pinMode(13, OUTPUT);
  digitalWrite(13, HIGH);
  digitalWrite(3, HIGH);
  analogWrite(2, 100);
}

void loop() {
  if (LAYER1.available()) {
    processLayer1Serial();
  }
}