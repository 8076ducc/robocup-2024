// #include "main.h"
// #include <RPLidar.h>

// // Function to calculate the mean of a vector
// double mean(vector<double> v)
// {
//     double sum = 0.0;
//     for (int i = 0; i < v.size(); i++)
//     {
//         sum += v[i];
//     }
//     return sum / v.size();
// }

// class Localisation
// {
//     double slope;
//     double intercept;

//     // Function to calculate the slope and intercept of a linear regression line
//     void linearRegression(vector<double> x, vector<double> y, double &slope, double &intercept)
//     {
//         double x_mean = mean(x);
//         double y_mean = mean(y);
//         double numerator = 0.0;
//         double denominator = 0.0;
//         for (int i = 0; i < x.size(); i++)
//         {
//             numerator += (x[i] - x_mean) * (y[i] - y_mean);
//             denominator += pow(x[i] - x_mean, 2);
//         }
//         slope = numerator / denominator;
//         intercept = y_mean - slope * x_mean;
//     }

//     double getRobotPose()
//     {
//         return 0;
//     }
// }

// Localisation localisation;

// int main()
// {
//     vector<double> x = {1, 2, 3, 4, 5};
//     vector<double> y = {2, 4, 6, 8, 10};
//     localisation.linearRegression(x, y, slope, intercept);
//     cout << localisation.slope << localisation.intercept << endl;
//     return 0;
// }

// // RPLidar lidar;

// // void setUpLidar() {
// //   Serial3.begin(256000);
// //   lidar.begin(Serial3);
// // }

// // void processLidar() {
// //   if (IS_OK(lidar.waitPoint())) {
// //     float distance = lidar.getCurrentPoint().distance;
// //     float angle    = lidar.getCurrentPoint().angle;

// //     Serial.print(distance);
// //     Serial.print(" ");
// //     Serial.println(angle);
// //   }
// // }