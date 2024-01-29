#include "main.h"

void Base::setUpMotors() {
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

    pinMode(FL_CS, INPUT);
    pinMode(FR_CS, INPUT);
    pinMode(BL_CS, INPUT);
    pinMode(BR_CS, INPUT);
}

void Base::motorOut(int motor, float speed) {
    int INA = 0, dir = 0, pwm = 0;

    switch (motor) {
        case 1:
            INA = FL_INA;
            dir = speed > 0 ?  LOW : HIGH;
            pwm = FL_PWM;
            break;
        case 2:
            INA = FR_INA;
            dir = speed > 0 ?  HIGH : LOW;
            pwm = FR_PWM;
            break;
        case 3:
            INA = BL_INA;
            dir = speed > 0 ?  LOW : HIGH;
            pwm = BL_PWM;
            break;
        case 4:
            INA = BR_INA;
            dir = speed > 0 ?  HIGH : LOW;
            pwm = BR_PWM;
            break;
    }
    
    // stop motor from stalling out if speed is below minimum threshold
    analogWrite(pwm, abs(speed) > min_speed ? abs(speed) : 0);
    digitalWriteFast(INA, dir);
}


void Base::move(float vel, float angle) {
    double x_vel = sin(degToRad(angle)) * sin(wheel_angle) * vel;
    double y_vel = cos(degToRad(angle)) * cos(wheel_angle) * vel;
    double ang_vel;

    if (angle - robot.current_pose.bearing > 180) {
        ang_vel = (angle - robot.current_pose.bearing) * 0.5;
    } else if (angle - robot.current_pose.bearing < -180) {
        ang_vel = (robot.current_pose.bearing - ang_vel) * 0.5;
    }

    double fl = (x_vel + y_vel) + ang_vel;
    double fr = (y_vel - x_vel) - ang_vel;
    double bl = (y_vel - x_vel) - ang_vel;
    double br = (x_vel + y_vel) + ang_vel;

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