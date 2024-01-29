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

void setup() {
  pinModeFast(LED, OUTPUT);
  digitalWriteFast(LED, HIGH);

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

  // processLidar();

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
  //   robot.base.move(0.25, robot.target_angle);
  // } else {
  //   robot.base.move(0.25, 0);
  // }

  robot.base.move(0.35, 0);

  Serial.print(analogRead(FL_CS));
  Serial.print(" ");
  Serial.print(analogRead(FR_CS));
  Serial.print(" ");
  Serial.print(analogRead(BL_CS));
  Serial.print(" ");
  Serial.println(analogRead(BR_CS));

  // digitalWriteFast(FL_INA, HIGH);
  // analogWrite(FL_PWM, 3000);
  // digitalWriteFast(FR_INA, HIGH);
  // analogWrite(FR_PWM, 3000);
  // digitalWriteFast(BL_INA, HIGH);
  // analogWrite(BL_PWM, 3000);
  // digitalWriteFast(BR_INA, HIGH);
  // analogWrite(BR_PWM, 3000);

  teensy_1_tx_data.data.bearing = robot.current_pose.bearing;

  Layer1Serial.send(layer_1_rx_data.bytes, sizeof(layer_1_rx_data.bytes));
  TeensySerial.send(teensy_1_tx_data.bytes, sizeof(teensy_1_tx_data.bytes));
  #endif
}