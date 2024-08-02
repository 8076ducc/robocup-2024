#pragma once

#ifndef MAIN_H
#define MAIN_H

// #define DEBUG
// #define SERIAL_DEBUG
#define BOT1

#include <common.h>

#define DRIBBLER_LOWER_LIMIT 32
#define DRIBBLER_UPPER_LIMIT 48

#ifdef BOT1
#define FL_PWM 3
#define FR_PWM 2
#define BL_PWM 4
#define BR_PWM 5
#define LIDAR_PWM 6
#define DRIBBLER_PWM 23

#define FL_INA 10
#define FR_INA 9
#define BL_INA 11
#define BR_INA 12

#else

#define FL_PWM 2
#define FR_PWM 3
#define BL_PWM 4
#define BR_PWM 5
#define LIDAR_PWM 6
#define DRIBBLER_PWM 23

#define FL_INA 9
#define FR_INA 10
#define BL_INA 11
#define BR_INA 12

#endif

#define FL_CS 17
#define FR_CS 16
#define BL_CS 18
#define BR_CS 19

class Base
{
public:
    void setUp();
    void move(double vel, double angle, double bearing, double kp = 0.0013, double ki = 0.0, double kd = 0.005, double ema_constant = 0.02);
    void motorOut(int motor, double speed);
    double getAggregateSpeed(int motor);

    const double wheel_angle = 50 * M_PI / 180.0;
    const int max_pwm = 8192;
    const int min_speed = 800;
    // const double ema_constant = 0.0002;

    const double fl_voltage = 2.500;
    const double fr_voltage = 2.300;
    const double bl_voltage = 2.282;
    const double br_voltage = 2.480;
    const double fastest_motor = fl_voltage;

    // const double fl_scale = fastest_motor / fl_voltage;
    // const double fr_scale = fastest_motor / fr_voltage;
    // const double bl_scale = fastest_motor / bl_voltage;
    // const double br_scale = fastest_motor / br_voltage;

    const int fl_scale = 1;
    const int fr_scale = 1;
    const int bl_scale = 1;
    const int br_scale = 1;

    double prev_fl_out = 0;
    double prev_fr_out = 0;
    double prev_bl_out = 0;
    double prev_br_out = 0;
};

struct MoveData
{
    double speed;
    double target_angle;
    double target_bearing;
    double ema_constant;
};

struct LineData
{
    bool on_line;
    bool already_on_line;

    double initial_line_angle;
    double line_angle;
    int chord_length;
    double line_centre;

    float line_start_ldr;
    float line_end_ldr;

    float ldr_angles[15][32] = {
        {
            318.43,
            324.19,
            329.99,
            335.86,
            341.87,
            348.16,
            355.30,
            8.36,
            171.64,
            184.70,
            191.84,
            198.13,
            204.14,
            210.01,
            215.81,
            221.57,
            227.31,
            233.03,
            238.73,
            244.42,
            250.11,
            255.80,
            261.48,
            267.16,
            272.84,
            278.52,
            284.20,
            289.89,
            295.58,
            301.27,
            306.97,
            312.69,
        },
        {
            320.31,
            326.49,
            332.84,
            339.47,
            346.62,
            354.91,
            6.50,
            36.04,
            143.96,
            173.50,
            185.09,
            193.38,
            200.53,
            207.16,
            213.51,
            219.69,
            225.76,
            231.76,
            237.70,
            243.62,
            249.50,
            255.37,
            261.23,
            267.08,
            272.92,
            278.77,
            284.63,
            290.50,
            296.38,
            302.30,
            308.24,
            314.24,
        },
        {
            323.61,
            330.51,
            337.79,
            345.70,
            354.74,
            6.11,
            23.38,
            59.09,
            120.91,
            156.62,
            173.89,
            185.26,
            194.30,
            202.21,
            209.49,
            216.39,
            223.04,
            229.53,
            235.91,
            242.20,
            248.43,
            254.62,
            260.78,
            266.93,
            273.07,
            279.22,
            285.38,
            291.57,
            297.80,
            304.09,
            310.47,
            316.96,
        },
        {
            328.53,
            336.46,
            345.04,
            354.63,
            5.93,
            20.35,
            40.72,
            71.21,
            108.79,
            139.28,
            159.65,
            174.07,
            185.37,
            194.96,
            203.54,
            211.47,
            218.97,
            226.19,
            233.19,
            240.05,
            246.80,
            253.48,
            260.10,
            266.70,
            273.30,
            279.90,
            286.52,
            293.20,
            299.95,
            306.81,
            313.81,
            321.03,
        },
        {
            335.31,
            344.51,
            354.55,
            5.82,
            18.92,
            34.70,
            54.12,
            77.43,
            102.57,
            125.88,
            145.30,
            161.08,
            174.18,
            185.45,
            195.49,
            204.69,
            213.29,
            221.47,
            229.35,
            236.99,
            244.48,
            251.84,
            259.14,
            266.38,
            273.62,
            280.86,
            288.16,
            295.52,
            303.01,
            310.65,
            318.53,
            326.71,
        },
        {
            344.04,
            354.48,
            5.74,
            18.04,
            31.60,
            46.64,
            63.18,
            80.91,
            99.09,
            116.82,
            133.36,
            148.40,
            161.96,
            174.26,
            185.52,
            195.96,
            205.78,
            215.12,
            224.08,
            232.76,
            241.24,
            249.56,
            257.77,
            265.93,
            274.07,
            282.23,
            290.44,
            298.76,
            307.24,
            315.92,
            324.88,
            334.22,
        },
        {
            354.43,
            5.68,
            17.40,
            29.61,
            42.33,
            55.54,
            69.14,
            83.02,
            96.98,
            110.86,
            124.46,
            137.67,
            150.39,
            162.60,
            174.32,
            185.57,
            196.41,
            206.89,
            217.08,
            227.02,
            236.76,
            246.36,
            255.86,
            265.29,
            274.71,
            284.14,
            293.64,
            303.24,
            312.98,
            322.92,
            333.11,
            343.59,
        },
        {
            5.63,
            16.88,
            28.12,
            39.37,
            50.63,
            61.87,
            73.13,
            84.38,
            95.62,
            106.88,
            118.12,
            129.37,
            140.63,
            151.87,
            163.13,
            174.38,
            185.62,
            196.87,
            208.13,
            219.37,
            230.62,
            241.88,
            253.13,
            264.37,
            275.62,
            286.87,
            298.12,
            309.38,
            320.63,
            331.87,
            343.12,
            354.37,
        },
        {
            16.41,
            26.89,
            37.08,
            47.02,
            56.76,
            66.36,
            75.86,
            85.29,
            94.71,
            104.14,
            113.64,
            123.24,
            132.98,
            142.92,
            153.11,
            163.59,
            174.43,
            185.68,
            197.40,
            209.61,
            222.33,
            235.54,
            249.14,
            263.02,
            276.98,
            290.86,
            304.46,
            317.67,
            330.39,
            342.60,
            354.32,
            5.57,
        },
        {
            25.78,
            35.12,
            44.08,
            52.76,
            61.24,
            69.56,
            77.77,
            85.93,
            94.07,
            102.23,
            110.44,
            118.76,
            127.24,
            135.92,
            144.88,
            154.22,
            164.04,
            174.48,
            185.74,
            198.04,
            211.60,
            226.64,
            243.18,
            260.91,
            279.09,
            296.82,
            313.36,
            328.40,
            341.96,
            354.26,
            5.52,
            15.96,
        },
        {
            33.29,
            41.47,
            49.35,
            56.99,
            64.48,
            71.84,
            79.14,
            86.38,
            93.62,
            100.86,
            108.16,
            115.52,
            123.01,
            130.65,
            138.53,
            146.71,
            155.31,
            164.51,
            174.55,
            185.82,
            198.92,
            214.70,
            234.12,
            257.43,
            282.57,
            305.88,
            325.30,
            341.08,
            354.18,
            5.45,
            15.49,
            24.69,
        },
        {
            38.97,
            46.19,
            53.19,
            60.05,
            66.80,
            73.48,
            80.10,
            86.70,
            93.30,
            99.90,
            106.52,
            113.20,
            119.95,
            126.81,
            133.81,
            141.03,
            148.53,
            156.46,
            165.04,
            174.63,
            185.93,
            200.35,
            220.72,
            251.21,
            288.79,
            319.28,
            339.65,
            354.07,
            5.37,
            14.96,
            23.54,
            31.47,
        },
        {
            43.04,
            49.53,
            55.91,
            62.20,
            68.43,
            74.62,
            80.78,
            86.93,
            93.07,
            99.22,
            105.38,
            111.57,
            117.80,
            124.09,
            130.47,
            136.96,
            143.61,
            150.51,
            157.79,
            165.70,
            174.74,
            186.11,
            203.38,
            239.09,
            300.91,
            336.62,
            353.89,
            5.26,
            14.30,
            22.21,
            29.49,
            36.39,
        },
        {
            45.76,
            51.76,
            57.70,
            63.62,
            69.50,
            75.37,
            81.23,
            87.08,
            92.92,
            98.77,
            104.63,
            110.50,
            116.38,
            122.30,
            128.24,
            134.24,
            140.31,
            146.49,
            152.84,
            159.47,
            166.62,
            174.91,
            186.50,
            216.03,
            323.96,
            353.50,
            5.09,
            13.38,
            20.53,
            27.16,
            33.51,
            39.69,
        },
        {
            47.31,
            53.03,
            58.73,
            64.42,
            70.11,
            75.80,
            81.48,
            87.16,
            92.84,
            98.52,
            104.20,
            109.89,
            115.58,
            121.27,
            126.97,
            132.69,
            138.43,
            144.19,
            149.99,
            155.86,
            161.87,
            168.16,
            175.30,
            188.36,
            351.64,
            4.70,
            11.84,
            18.13,
            24.14,
            30.01,
            35.81,
            41.57,
        },
    };
};

class Dribbler
{
public:
    void update();

    bool dribbling;
};

class Robot
{
public:
    void moveToTargetPose();
    void setUpSerial();
    void updateSerial();
    void sendSerial();

    void defendGoal();
    void rotateToBall();
    void orbitToBall(double bearing);
    void rotateScore();
    void orbitScore();
    void moveToNeutralPoint(int neutral_point, bool behind_line);

    void trackLine(double speed, double angle, int offset);
    void trackLineGoalie(double speed, double angle, int offset);
    void rejectLine(double bearing);

    Base base;
    Dribbler dribbler;

    Pose previous_pose;
    Pose current_pose;
    Pose target_pose;

    MoveData move_data;
    LineData line_data;

    bool dip_1_on;
    bool dip_2_on;
    bool dip_3_on;
    bool dip_4_on;

    int task;

    bool alliance_robot_detected;
};

// global variables
extern PacketSerial Layer1Serial;
extern PacketSerial ImuSerial;
extern PacketSerial TeensySerial;

extern Layer1RxDataUnion layer_1_rx_data;
extern ImuRxDataUnion imu_rx_data;
extern Teensy1TxDataUnion teensy_1_tx_data;

extern Robot robot;
extern Ball ball;
extern Goal yellow_goal;
extern Goal blue_goal;

void lidarSetup();
void processLidar();

#endif