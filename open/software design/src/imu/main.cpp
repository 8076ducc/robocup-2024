#include "main.h"

PacketSerial TeensySerial;

ImuTxDataUnion tx_data;

// Adafruit_BNO08x  bno08x;
// sh2_SensorValue_t sensorValue;

// // Here is where you define the sensor outputs you want to receive
// void setReports(void) {
//   Serial.println("Setting desired reports");
//   if (! bno08x.enableReport(SH2_GAME_ROTATION_VECTOR)) {
//     Serial.println("Could not enable game vector");
//   }
// }

// void setup(void) {
//   #ifdef SERIAL_DEBUG
//   Serial.begin(115200);
//   while (!Serial) {}
//   Serial.println("Debug serial connection established.");
//   #endif

//   // Try to initialize!
//   if (!bno08x.begin_I2C()) {
//   //if (!bno08x.begin_UART(&Serial1)) {  // Requires a device with > 300 byte UART buffer!
//   //if (!bno08x.begin_SPI(BNO08X_CS, BNO08X_INT)) {
//     Serial.println("Failed to find BNO08x chip");
//     while (1) { delay(10); }
//   }
//   Serial.println("BNO08x Found!");

//   for (int n = 0; n < bno08x.prodIds.numEntries; n++) {
//     Serial.print("Part ");
//     Serial.print(bno08x.prodIds.entry[n].swPartNumber);
//     Serial.print(": Version :");
//     Serial.print(bno08x.prodIds.entry[n].swVersionMajor);
//     Serial.print(".");
//     Serial.print(bno08x.prodIds.entry[n].swVersionMinor);
//     Serial.print(".");
//     Serial.print(bno08x.prodIds.entry[n].swVersionPatch);
//     Serial.print(" Build ");
//     Serial.println(bno08x.prodIds.entry[n].swBuildNumber);
//   }

//   setReports();

//   Serial.println("Reading events");
//   delay(100);
// }


// void loop() {
//   delay(10);

//   if (bno08x.wasReset()) {
//     Serial.print("sensor was reset ");
//     setReports();
//   }
  
//   if (! bno08x.getSensorEvent(&sensorValue)) {
//     return;
//   }

//   double bearing;
  
//   switch (sensorValue.sensorId) {
    
//     case SH2_GAME_ROTATION_VECTOR:
//       // Serial.print("i: ");
//       bearing = sensorValue.un.gameRotationVector.i > 0 ? 360 - sensorValue.un.gameRotationVector.i * 180 : 0 - sensorValue.un.gameRotationVector.i * 180;
//       // Serial.println(bearing);
//       double y = sensorValue.un.rawGyroscope.y;
//       double x = sensorValue.un.rawGyroscope.x;
//       double heading = atan2(y, x) * 180 / M_PI;
//       Serial.println(heading);
//       break;
//   }

//   tx_data.data.bearing = bearing;
//   TeensySerial.send(tx_data.bytes, sizeof(tx_data.bytes));
// }


// #define FAST_MODE

// For SPI mode, we also need a RESET 
//#define BNO08X_RESET 5
// but not for I2C or UART
#define BNO08X_RESET -1

struct euler_t {
  float yaw;
  float pitch;
  float roll;
} ypr;

Adafruit_BNO08x  bno08x(BNO08X_RESET);
sh2_SensorValue_t sensorValue;

#ifdef FAST_MODE
  // Top frequency is reported to be 1000Hz (but freq is somewhat variable)
  sh2_SensorId_t reportType = SH2_GYRO_INTEGRATED_RV;
  long reportIntervalUs = 2000;
#else
  // Top frequency is about 250Hz but this report is more accurate
  sh2_SensorId_t reportType = SH2_ARVR_STABILIZED_RV;
  long reportIntervalUs = 5000;
#endif
void setReports(sh2_SensorId_t reportType, long report_interval) {
  Serial.println("Setting desired reports");
  if (! bno08x.enableReport(reportType, report_interval)) {
    Serial.println("Could not enable stabilized remote vector");
  }
}

void setup(void) {

  Serial.begin(115200);
  while (!Serial) delay(10);     // will pause Zero, Leonardo, etc until serial console opens

  Serial.println("Adafruit BNO08x test!");

  // Try to initialize!
  if (!bno08x.begin_I2C()) {
  //if (!bno08x.begin_UART(&Serial1)) {  // Requires a device with > 300 byte UART buffer!
  //if (!bno08x.begin_SPI(BNO08X_CS, BNO08X_INT)) {
    Serial.println("Failed to find BNO08x chip");
    while (1) { delay(10); }
  }
  Serial.println("BNO08x Found!");


  setReports(reportType, reportIntervalUs);

  Serial.println("Reading events");
  delay(100);
}

void quaternionToEuler(float qr, float qi, float qj, float qk, euler_t* ypr, bool degrees = false) {

    float sqr = sq(qr);
    float sqi = sq(qi);
    float sqj = sq(qj);
    float sqk = sq(qk);

    ypr->yaw = atan2(2.0 * (qi * qj + qk * qr), (sqi - sqj - sqk + sqr));
    ypr->pitch = asin(-2.0 * (qi * qk - qj * qr) / (sqi + sqj + sqk + sqr));
    ypr->roll = atan2(2.0 * (qj * qk + qi * qr), (-sqi - sqj + sqk + sqr));

    if (degrees) {
      ypr->yaw *= RAD_TO_DEG;
      ypr->pitch *= RAD_TO_DEG;
      ypr->roll *= RAD_TO_DEG;
    }
}

void quaternionToEulerRV(sh2_RotationVectorWAcc_t* rotational_vector, euler_t* ypr, bool degrees = false) {
    quaternionToEuler(rotational_vector->real, rotational_vector->i, rotational_vector->j, rotational_vector->k, ypr, degrees);
}

void quaternionToEulerGI(sh2_GyroIntegratedRV_t* rotational_vector, euler_t* ypr, bool degrees = false) {
    quaternionToEuler(rotational_vector->real, rotational_vector->i, rotational_vector->j, rotational_vector->k, ypr, degrees);
}

void loop() {

  if (bno08x.wasReset()) {
    Serial.print("sensor was reset ");
    setReports(reportType, reportIntervalUs);
  }
  
  if (bno08x.getSensorEvent(&sensorValue)) {
    // in this demo only one report type will be received depending on FAST_MODE define (above)
    switch (sensorValue.sensorId) {
      case SH2_ARVR_STABILIZED_RV:
        quaternionToEulerRV(&sensorValue.un.arvrStabilizedRV, &ypr, true);
      case SH2_GYRO_INTEGRATED_RV:
        // faster (more noise?)
        quaternionToEulerGI(&sensorValue.un.gyroIntegratedRV, &ypr, true);
        break;
    }
    Serial.println(ypr.yaw + 180);
  }

}