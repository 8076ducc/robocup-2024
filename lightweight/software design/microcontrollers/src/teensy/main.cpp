#include "main.h"

PacketSerial Layer1Serial;
PacketSerial STMSerial;
PacketSerial CamSerial;

Layer1RxDataUnion layer_1_rx_data;

Robot robot;
Ball ball;
Goal yellow_goal;
Goal blue_goal;

int ball_catchment = 0;

unsigned long last_time = 0;
int real = 0;
int points = 0;

void setup()
{
  robot.base.setUp();
  pinMode(BL_PWM, OUTPUT);
  pinMode(BL_INA,  OUTPUT);
  digitalWrite(BL_INA, LOW);
  analogWrite(BL_PWM, 100);
  robot.setUpSerial();
  robot.setupIMU();

  robot.previous_pose.x = 0;
  robot.previous_pose.y = 0;
  robot.previous_pose.bearing = 0;

}

void loop()
{
  layer_1_rx_data.data.kick = false;
  robot.readIMU();
  robot.updateSerial();
  robot.sendSerial();

  Serial.print(robot.on_line);

  if (ball.in_catchment)
  {
    Serial.println("Ball in catchment");
    ++ball_catchment;
  }
  else
  {
    ball_catchment = 0;
  }

  if (robot.on_line)
  {
    robot.base.move(0.7, correctBearing(robot.target_angle + 180), 0);
  }
  else
  {
    if (false)
    {
      robot.orbitScore();
    }
    else
    {
      robot.orbitToBall();
    }
  // //   // robot.defendGoal();
  // //   // robot.rotateToBall();
  }
}
