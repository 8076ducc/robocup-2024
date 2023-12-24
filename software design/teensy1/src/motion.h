#pragma once
#ifndef motion.h
#define motion.h

#define WHEEL_ANGLE 50 * M_PI / 180.0
#define MIN_SPEED 0.1 * 255
#define MAX_SPEED 1
#define MAX_ACCEL 0.5

void move(float speed, float angle, float angVel, float angSpeed=-1.0);

#endif