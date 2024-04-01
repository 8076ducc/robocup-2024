#include "line.h"

double mean(std::vector<double> v)
{
    double sum = 0.0;
    for (int i = 0; i < v.size(); i++)
    {
        sum += v[i];
    }
    return sum / v.size();
}

void Line::horizontalLinearRegression()
{
    double x_mean = mean(x);
    double y_mean = mean(y);
    double numerator = 0.0;
    double denominator = 0.0;
    for (int i = 0; i < x.size(); i++)
    {
        numerator += (x[i] - x_mean) * (y[i] - y_mean);
        denominator += pow(x[i] - x_mean, 2);
    }
    slope = numerator / denominator;
    intercept = y_mean - slope * x_mean;
}

void Line::verticalLinearRegression()
{
    double x_mean = mean(x);
    double y_mean = mean(y);
    // TODO: maybe write average code
    // for (int i = 0; i < x.size(); i++)
    // {
    // }
    double numerator = 0.0;
    double denominator = 0.0;
    for (int i = 0; i < x.size(); i++)
    {
        numerator += (y[i] - y_mean) * (x[i] - x_mean);
        denominator += pow(y[i] - y_mean, 2);
    }
    slope = numerator / denominator;
    intercept = x_mean - slope * y_mean;
}