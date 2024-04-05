#pragma once

#ifndef FUNCTIONS_H
#define FUNCTIONS_H

#include <Arduino.h>

double correctBearing(double bearing);
int sgn(double val);
double bound(double a, double b, double c);

#endif