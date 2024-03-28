#include "functions.h"

double correctBearing(double bearing)
{
    if (bearing > 360)
    {
        bearing = bearing - 360;
    }
    else if (bearing < 0)
    {
        bearing = bearing + 360;
    }

    return bearing;
}

int sgn(double val)
{
    if (val > 0)
    {
        return 1;
    }
    else if (val < 0)
    {
        return -1;
    }
    else
    {
        return 0;
    }
}