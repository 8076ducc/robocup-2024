#include "main.h"

PacketSerial Layer1Serial;
PacketSerial ImuSerial;
PacketSerial TeensySerial;

Layer1RxDataUnion layer_1_rx_data;
ImuRxDataUnion imu_rx_data;
Teensy1TxDataUnion teensy_1_tx_data;

Robot robot;
Ball ball;

float degToRad(float deg) {
    return deg * M_PI / 180;
}

double front, left, back, right, last_front, last_left, last_back, last_right;

void setup() {
  pinModeFast(LED, OUTPUT);
  digitalWriteFast(LED, HIGH);

  Serial.begin(115200);
  while (!Serial) {}
  Serial.println("Debug serial connection established.");

  robot.setUpSerial();
  
  robot.base.setUpMotors();
  robot.setUpDribbler();

  lidarSetup();
}

void loop() {
  #ifdef DEBUG
  // if ((millis() / 5000) % 2 > 0) {
  //   robot.base.move(0.4, 0);
  // } else {
  //   robot.base.move(0.4, 180);
    
  // }
  #else
  Layer1Serial.update();
  ImuSerial.update();
  TeensySerial.update();

  processLidar();

  robot.current_pose.y = (2430 / (front + back)) * front;
  robot.current_pose.x = (1820 / (left + right)) * left;

  // Serial.print("(");
  // Serial.print(robot.current_pose.x);
  // Serial.print(", ");
  // Serial.print(robot.current_pose.y);
  // Serial.println(")");

  // Serial.print("Front: ");
  // Serial.print(front);
  // Serial.print(" Left: ");
  // Serial.print(left);
  // Serial.print(" Back: ");
  // Serial.print(back);
  // Serial.print(" Right: ");
  // Serial.println(right);

  // if (robot.on_line) {
  //   robot.base.move(0.5, robot.target_angle);
  // } else {
  //   robot.base.move(0.5, 0);
  // }

  Layer1Serial.send(layer_1_rx_data.bytes, sizeof(layer_1_rx_data.bytes));
  TeensySerial.send(teensy_1_tx_data.bytes, sizeof(teensy_1_tx_data.bytes));
  #endif
}