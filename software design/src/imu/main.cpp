#include "main.h"

// Global variables for IMU
Adafruit_BNO08x bno;
Eigen::Quaterniond initialRotationOffset = Eigen::Quaterniond::Identity();

void setup() {
    // Initialise IMU
#ifdef NEW_BOT
  if (!bno.begin_I2C()) {
#else
  IMUSerial.begin(115200);
  if (!bno.begin_UART(&IMUSerial)) {
#endif
      Serial.println("Failed to find BNO085!!");
      // Blink the debug LED on failure
      // while (1) {
      //     checkTeensyReset();
      //     digitalWrite(PIN_LED_BUILTIN, HIGH);
      //     delay(100);
      //     digitalWrite(PIN_LED_BUILTIN, LOW);
      //     delay(100);
      // }
  }
  // TODO: try SH2_ROTATION_VECTOR, SH2_GYRO_INTEGRATED_RV
  // The SH2_GAME_ROTATION_VECTOR mode uses fusion of gyroscope and
  // accelerometer data to determine a rotation vector in quaternions
  if (!bno.enableReport(SH2_GAME_ROTATION_VECTOR)) {
      Serial.println("Could not enable game rotation vector!!");
      // Blink the debug LED on failure
      while (1) {
          checkTeensyReset();
          digitalWrite(PIN_LED_BUILTIN, HIGH);
          delay(100);
          digitalWrite(PIN_LED_BUILTIN, LOW);
          delay(100);
      }
  }

}

void loop() {
  readIMUHeading();
}

double readIMUHeading() {
    sh2_SensorValue_t bnoValue;
    if (bno.getSensorEvent(&bnoValue)) {
        switch (bnoValue.sensorId) {
        case SH2_GAME_ROTATION_VECTOR: {
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
            return degrees(yaw);

            break;
        }
        default:
            break;
        }
    }

    return NAN;
}