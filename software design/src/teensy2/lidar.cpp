#include "main.h"
#include <RPLidar.h>

RPLidar lidar;

// void setup() {
//   Serial3.begin(256000);
//   lidar.begin(Serial3);
// }

// void loop() {
//   if (IS_OK(lidar.waitPoint())) {
//     float distance = lidar.getCurrentPoint().distance;
//     float angle    = lidar.getCurrentPoint().angle;
//   }
// }