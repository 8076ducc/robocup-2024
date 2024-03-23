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
  pinMode(LED_BUILTIN, OUTPUT);
  robot.setUpSerial();
  robot.base.setUpMotors();
  robot.setUpDribbler();

  robot.previous_pose.x = 0;
  robot.previous_pose.y = 0;

  lidarSetup();
  last_time = millis();

  delay(1000);
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

  // teensy_1_tx_data.data.current_pose.x = robot.current_pose.x;
  // teensy_1_tx_data.data.current_pose.y = robot.current_pose.y;
  // teensy_1_tx_data.data.current_pose.bearing = robot.current_pose.bearing;

  if (robot.on_line)
  {
    Serial.println("On line");
    robot.base.move(0.7, robot.target_angle, 0);
  }
  else
  {
    double actual_angle_diff = robot.target_pose.bearing - robot.current_pose.bearing;

    if (actual_angle_diff > 360)
    {
      actual_angle_diff = actual_angle_diff - 360;
    }

    robot.base.move(0.3, actual_angle_diff, robot.target_pose.bearing);
    Serial.println(actual_angle_diff);
    // Serial.println(robot.target_pose.bearing);
    // robot.target_angle = 0;

    // if (millis() - last_time > 6000)
    // {
    //   if (points == 4)
    //   {
    //     points = 0;
    //     last_time = millis();
    //   }
    //   else
    //   {
    //     points += 1;
    //     last_time = millis();
    //   }
    // }

    // switch (points)
    // {
    // case 0:
    //   robot.target_pose.x = 1370;
    //   robot.target_pose.y = 800;
    //   break;

    // case 1:
    //   robot.target_pose.x = 450;
    //   robot.target_pose.y = 800;
    //   break;

    // case 2:
    //   robot.target_pose.x = 910;
    //   robot.target_pose.y = 1215;
    //   robot.target_angle = 45;
    //   break;

    // case 3:
    //   robot.target_pose.x = 450;
    //   robot.target_pose.y = 1630;
    //   break;

    // case 4:
    //   robot.target_pose.x = 1370;
    //   robot.target_pose.y = 1630;
    //   break;

    // default:
    //   break;
    // }
    // robot.moveToTargetPose();
  }

  // if (millis() % 5000 <= 100)
  // {
  //   layer_1_rx_data.data.kick = true;
  // }
  // layer_1_rx_data.data.kick = false;

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
//   analogWrite(DRIBBLER_PWM, 40);
//   // frontDrib.writeMicroseconds(2000);
//   //  delay(500);
//   //  if (pwm < 64) {
//   //    pwm += 1;
//   //  }
// }