#include "main.h"
#include <Adafruit_BNO08x.h>
//#include <ArduinoEigen.h>
#include <ArduinoEigenDense.h>

Adafruit_BNO08x bno;
Eigen::Quaterniond initialRotationOffset = Eigen::Quaterniond::Identity();

double bearing = 0;

void Robot::setupIMU()
{
    if (!bno.begin_I2C())
    {
        Serial.println("Failed to find BNO085!!");
        // Blink the debug LED on failure
        // while (1) {
        // digitalWrite(PIN_LED_BUILTIN, HIGH);
        // delay(100);
        // digitalWrite(PIN_LED_BUILTIN, LOW);
        // delay(100);
        // }
    }
    else
    {
        Serial.println("BNO085 started");
    }
    // TODO: try SH2_ROTATION_VECTOR, SH2_GYRO_INTEGRATED_RV
    // The SH2_GAME_ROTATION_VECTOR mode uses fusion of gyroscope and
    // accelerometer data to determine a rotation vector in quaternions
    if (!bno.enableReport(SH2_GAME_ROTATION_VECTOR))
    {
        Serial.println("Could not enable game rotation vector!!");
        // Blink the debug LED on failure
        // while (1) {
        //     digitalWrite(LED_BUILTIN, HIGH);
        //     delay(100);
        //     digitalWrite(LED_BUILTIN, LOW);
        //     delay(100);
        // }
    }
}

void Robot::readIMU()
{
    sh2_SensorValue_t bnoValue;
    if (bno.getSensorEvent(&bnoValue))
    {
        switch (bnoValue.sensorId)
        {
        case SH2_GAME_ROTATION_VECTOR:
        {
            const Eigen::Quaterniond rotation = {
                bnoValue.un.gameRotationVector.real,
                bnoValue.un.gameRotationVector.i,
                bnoValue.un.gameRotationVector.j,
                bnoValue.un.gameRotationVector.k,
            };

            // Set the initial offset if it hasn't been set
            if (initialRotationOffset == Eigen::Quaterniond::Identity())
                initialRotationOffset = rotation.inverse();

            // Compute the robot angle
            const auto correctedRotation = initialRotationOffset * rotation;
            const auto rotationMatrix = correctedRotation.toRotationMatrix();
            const auto yaw = -atan2(rotationMatrix(1, 0), rotationMatrix(0, 0));
            bearing = degrees(yaw) > 0 ? degrees(yaw) : 360 - abs(degrees(yaw));
            break;
        }
        default:
            break;
        }
    }
    robot.current_pose.bearing = bearing;
    //Serial.println(bearing);
}