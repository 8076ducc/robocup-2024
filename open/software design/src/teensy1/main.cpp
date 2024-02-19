#include "main.h"

PacketSerial Layer1Serial;
PacketSerial ImuSerial;
PacketSerial TeensySerial;

Layer1RxDataUnion layer_1_rx_data;
ImuRxDataUnion imu_rx_data;
Teensy1TxDataUnion teensy_1_tx_data;

Robot robot;
Ball ball;

unsigned long last_time = 0;
int real = 0;
int points = 0;

void setup()
{
  pinModeFast(LED, OUTPUT);
  digitalWriteFast(LED, HIGH);

  robot.setUpSerial();

  robot.base.setUpMotors();
  robot.setUpDribbler();

  lidarSetup();
  last_time = millis();
}

void loop()
{
#ifdef DEBUG
// if ((millis() / 5000) % 2 > 0) {
//   robot.base.move(0.4, 0);
// } else {
//   robot.base.move(0.4, 180);

// }
#else
  robot.updateSerial();
  processLidar();

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

  // switch (real)
  // {
  // case 0:
  //   robot.base.move(0.4, 0, 0);
  //   break;
  // case 1:
  //   robot.base.move(0.3, 90, 0);
  //   break;
  // case 2:
  //   robot.base.move(0.4, 180, 0);
  //   break;
  // case 3:
  //   robot.base.move(0.3, 270, 0);
  //   break;
  // }

  // if (millis() - last_time > 800) {
  //   if (real == 3) {
  //     real = 0;
  //   } else {
  //     real += 1;
  //   }
  //   last_time = millis();
  // }

  if (robot.on_line)
  {
    robot.base.move(0.4, robot.target_angle, 0);
    // Serial.println(robot.target_angle);
  }
  else
  {
    if (millis() - last_time > 6000)
    {
      if (points == 4)
      {
        points = 0;
        last_time = millis();
      }
      else
      {
        points += 1;
        last_time = millis();
      }
    }

    switch (points)
    {
    case 0:
      robot.target_pose.x = 1370;
      robot.target_pose.y = 800;
      break;

    case 1:
      robot.target_pose.x = 450;
      robot.target_pose.y = 800;
      break;

    case 2:
      robot.target_pose.x = 910;
      robot.target_pose.y = 1215;
      break;

    case 3:
      robot.target_pose.x = 450;
      robot.target_pose.y = 1630;
      break;

    case 4:
      robot.target_pose.x = 1370;
      robot.target_pose.y = 1630;
      break;

    default:
      break;
    }
    robot.moveToTargetPose();
  }
  robot.sendSerial();
#endif
}

// #include <Servo.h>

// Servo frontDrib;

// // #define DRIBBLER_WAIT 1000
// #define DRIBBLER_LOWER_LIMIT 32
// #define DRIBBLER_UPPER_LIMIT 48

// void setup()
// {
//   analogWriteFrequency(DRIBBLER_PWM, 1000);
//   analogWrite(DRIBBLER_PWM, DRIBBLER_LOWER_LIMIT);
//   delay(3000);
//   // put your setup code here, to run once:
//   // analogWriteResolution(8);
//   // pinMode(DRIBBLER_PWM, OUTPUT);

//   // analogWriteFrequency(DRIBBLER_PWM, 4000);

//   // analogWrite(DRIBBLER_PWM, 128);
//   // delay(DRIBBLER_WAIT);
//   // analogWrite(DRIBBLER_PWM, 48);

//   // pinMode(LED_BUILTIN, OUTPUT);
//   // digitalWrite(LED_BUILTIN, HIGH);

//   // frontDrib.attach(DRIBBLER_PWM);
//   // frontDrib.writeMicroseconds(700);
//   // delay(1000);
//   // frontDrib.writeMicroseconds(2000);
//   // delay(1000);
//   // frontDrib.writeMicroseconds(700);
// }

// void loop()
// {
//   analogWrite(DRIBBLER_PWM, DRIBBLER_UPPER_LIMIT);
//   // frontDrib.writeMicroseconds(2000);
//   //  delay(500);
//   //  if (pwm < 64) {
//   //    pwm += 1;
//   //  }
// }