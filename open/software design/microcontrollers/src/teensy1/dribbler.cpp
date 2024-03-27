#include "main.h"

void Dribbler::setUp()
{
    pinMode(DRIBBLER_PWM, OUTPUT);

    analogWriteFrequency(DRIBBLER_PWM, 1000);
    analogWrite(DRIBBLER_PWM, DRIBBLER_LOWER_LIMIT);
    delay(3000);
    analogWrite(DRIBBLER_PWM, DRIBBLER_UPPER_LIMIT);
    delay(1000);
    // analogWrite(DRIBBLER_PWM, DRIBBLER_LOWER_LIMIT);
}

void Dribbler::setSpeed(int speed)
{
    analogWrite(DRIBBLER_PWM, speed);
}