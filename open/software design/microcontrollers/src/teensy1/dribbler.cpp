#include "main.h"

bool armed = false;
unsigned long armTime;

void Dribbler::update()
{
    analogWriteResolution(8);

    if (!armed)
    {
        analogWriteFrequency(DRIBBLER_PWM, 1000);
        analogWrite(DRIBBLER_PWM, DRIBBLER_LOWER_LIMIT);
        armed = true;
        armTime = millis();
    }
    if (millis() - armTime > 3000)
    {
        if (dribbling)
        {
            analogWrite(DRIBBLER_PWM, 40);
        }
        else
        {
            analogWrite(DRIBBLER_PWM, DRIBBLER_LOWER_LIMIT);
        }
    }
}