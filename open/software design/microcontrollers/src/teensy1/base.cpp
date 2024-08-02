#include "main.h"

void Base::setUp()
{
    pinModeFast(FL_INA, OUTPUT);
    pinModeFast(FR_INA, OUTPUT);
    pinModeFast(BL_INA, OUTPUT);
    pinModeFast(BR_INA, OUTPUT);

    pinMode(FL_PWM, OUTPUT);
    pinMode(FR_PWM, OUTPUT);
    pinMode(BL_PWM, OUTPUT);
    pinMode(BR_PWM, OUTPUT);

    analogWriteResolution(13);
    analogWriteFrequency(FL_PWM, 18310.55);
    analogWriteFrequency(FR_PWM, 18310.55);
    analogWriteFrequency(BL_PWM, 18310.55);
    analogWriteFrequency(BR_PWM, 18310.55);

    // pinMode(FL_CS, INPUT);
    // pinMode(FR_CS, INPUT);
    // pinMode(BL_CS, INPUT);
    // pinMode(BR_CS, INPUT);
}

void Base::motorOut(int motor, double speed)
{
    analogWriteResolution(13);
    int INA = 0, dir = 0, pwm = 0;

    switch (motor)
    {
    case 1:
        INA = FL_INA;
        dir = speed > 0 ? LOW : HIGH;
        pwm = FL_PWM;
        break;
    case 2:
        INA = FR_INA;
        dir = speed > 0 ? HIGH : LOW;
        pwm = FR_PWM;
        break;
    case 3:
        INA = BL_INA;
        dir = speed > 0 ? LOW : HIGH;
        pwm = BL_PWM;
        break;
    case 4:
        INA = BR_INA;
        dir = speed > 0 ? HIGH : LOW;
        pwm = BR_PWM;
        break;
    }

    // if (abs(speed) > 5000)
    // {
    //     speed = sgn(speed) * 5000;
    // }

    // stop motor from stalling out if speed is below minimum threshold
    if (abs(speed) > 150)
    {
        analogWrite(pwm, abs(speed) > min_speed ? abs(speed) : min_speed);
    }
    else
    {
        analogWrite(pwm, 0);
    }

    digitalWriteFast(INA, dir);
}

// double kp = 0.0013;
// double ki = 0.00;
// double kd = 0.005;
double proportional = 0;
double integral = 0;
double derivative = 0;
double prev_error = 0;
double turn_angle = 0;

void Base::move(double vel, double angle, double bearing, double kp = 0.0013, double ki = 0.0, double kd = 0.005, double ema_constant)
{
    ema_constant = 0.005;
    double x_vel = sin(radians(angle)) * sin(wheel_angle);
    double y_vel = cos(radians(angle)) * cos(wheel_angle);
    double ang_vel = 0;

    double turn_angle = bearing - robot.current_pose.bearing;

    // Serial.print("bearing ");
    // Serial.println(bearing);
    // Serial.print(" robot pose ");
    // Serial.println(robot.current_pose.bearing);

    if (turn_angle > 180)
    {
        turn_angle -= 360;
    }
    else if (turn_angle < -180)
    {
        turn_angle += 360;
    }

    proportional = kp * turn_angle;
    integral += ki * turn_angle;
    derivative = kd * (abs(turn_angle) - abs(prev_error));

    ang_vel = min(proportional + integral + derivative, 0.2);
    prev_error = turn_angle;

    double a = x_vel + y_vel;
    double b = y_vel - x_vel;
    double a_scaled, b_scaled;

    if (abs(a) > abs(b))
    {
        a_scaled = sgn(a) * vel;
        b_scaled = sgn(b) * abs(b / a) * vel;
    }
    else
    {
        a_scaled = sgn(a) * abs(a / b) * vel;
        b_scaled = sgn(b) * vel;
    }

    // double fl = a * vel + ang_vel;
    // double fr = b * vel - ang_vel;
    // double bl = b * vel + ang_vel;
    // double br = a * vel - ang_vel;

    double fl = a_scaled + ang_vel;
    double fr = b_scaled - ang_vel;
    double bl = b_scaled + ang_vel;
    double br = a_scaled - ang_vel;

    // calculate new speeds
    double new_fl_out = fl * max_pwm;
    double new_fr_out = fr * max_pwm;
    double new_bl_out = bl * max_pwm;
    double new_br_out = br * max_pwm;

    // calculate exponential moving average
    double fl_out = fl_scale * (new_fl_out * ema_constant) + (prev_fl_out * (1 - ema_constant));
    double fr_out = fr_scale * (new_fr_out * ema_constant) + (prev_fr_out * (1 - ema_constant));
    double bl_out = bl_scale * (new_bl_out * ema_constant) + (prev_bl_out * (1 - ema_constant));
    double br_out = br_scale * (new_br_out * ema_constant) + (prev_br_out * (1 - ema_constant));

    // update previous speeds
    prev_fl_out = fl_out;
    prev_fr_out = fr_out;
    prev_bl_out = bl_out;
    prev_br_out = br_out;

#ifdef SERIAL_DEBUG
    Serial.print("1: ");
    Serial.print(fl_out);
    Serial.print("  2: ");
    Serial.print(fr_out);
    Serial.print("  3: ");
    Serial.print(bl_out);
    Serial.print("  4: ");
    Serial.println(br_out);
#endif

    motorOut(1, round(fl_out));
    motorOut(2, round(fr_out));
    motorOut(3, round(bl_out));
    motorOut(4, round(br_out));
}