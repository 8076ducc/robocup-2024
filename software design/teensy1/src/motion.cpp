#include <main.h>
#include <motion.h>

float prev_fl_out = 0;
float prev_fr_out = 0;
float prev_bl_out = 0;
float prev_br_out = 0;

void motorOut(int motor, float speed) {
    int INA, dir, pwm;

    switch (motor) {
        case 1:
            INA = M1_INA;
            dir = speed > 0 ?  HIGH : LOW;
            pwm = M1_PWM;
        case 2:
            INA = M2_INA;
            dir = speed > 0 ?  LOW : HIGH;
            pwm = M2_PWM;
        case 3:
            INA = M3_INA;
            dir = speed > 0 ?  HIGH : LOW;
            pwm = M3_PWM;
        case 4:
            INA = M4_INA;
            dir = speed > 0 ?  LOW : HIGH;
            pwm = M4_PWM;
    }
    
    // stop motor from stalling out if speed is below minimum threshold
    analogWrite(pwm, abs(speed) > MIN_SPEED ? abs(speed) : 0);
    digitalWriteFast(INA, dir);
}


void move(float speed, float angle, float angVel, float angSpeed=-1.0) {

    float rad_ang = angle * M_PI / 180;

    float x_speed = sinf(rad_ang)*sinf(WHEEL_ANGLE);
    float y_speed = cosf(rad_ang)*cosf(WHEEL_ANGLE);

    float fl = (x_speed + y_speed) * speed;
    float fr = (-x_speed + y_speed) * speed;
    float bl = (-x_speed + y_speed) * speed;
    float br = (x_speed + y_speed) * speed;

    // calculate new speeds
    float new_fl_out = MAX_SPEED * fl * 255;
    float new_fr_out = MAX_SPEED * fr * 255;
    float new_bl_out = MAX_SPEED * bl * 255;
    float new_br_out = MAX_SPEED * br * 255;

    // check if the acceleration exceeds the maximum threshold
    float fl_out = abs(new_fl_out) - abs(prev_fl_out) < MAX_ACCEL ? new_fl_out : prev_fl_out + MAX_ACCEL;
    float fr_out = abs(new_fr_out) - abs(prev_fr_out) < MAX_ACCEL ? new_fr_out : prev_fr_out + MAX_ACCEL;
    float bl_out = abs(new_bl_out) - abs(prev_bl_out) < MAX_ACCEL ? new_bl_out : prev_bl_out + MAX_ACCEL;
    float br_out = abs(new_br_out) - abs(prev_br_out) < MAX_ACCEL ? new_br_out : prev_br_out + MAX_ACCEL;

    // update previous speeds
    prev_fl_out = fl_out;
    prev_fr_out = fr_out;
    prev_bl_out = bl_out;
    prev_br_out = br_out;

    #ifdef DEBUG
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