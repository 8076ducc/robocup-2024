#pragma once

#ifndef LINE_H
#define LINE_H

#include <vector>
#include <math.h>

class Line
{
public:
    std::vector<double> x;
    std::vector<double> y;

    double slope;
    double intercept;

    // Function to calculate the slope and intercept of a linear regression line
    void horizontalLinearRegression();
    void verticalLinearRegression();
};

#endif