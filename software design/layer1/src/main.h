#pragma once

#ifndef main.h
#define main.h

#include <Arduino.h>
#include <math.h>

#define LED D9

#define S0 D2
#define S1 D3
#define S2 D4
#define S3 D5

#define MUX1 D0
#define MUX2 D1
#define LIGHTGATE D9

#define KICKER D10

#define TEENSY Serial0

const int mux_signals[16][4] = {
    {0, 0, 0, 0}, // 0
	{1, 0, 0, 0}, // 1
	{0, 1, 0, 0}, // 2
	{1, 1, 0, 0}, // 3
	{0, 0, 1, 0}, // 4
	{1, 0, 1, 0}, // 5
	{0, 1, 1, 0}, // 6
	{1, 1, 1, 0}, // 7
	{0, 0, 0, 1}, // 8
	{1, 0, 0, 1}, // 9
	{0, 1, 0, 1}, // 10
	{1, 1, 0, 1}, // 11
	{0, 0, 1, 1}, // 12
	{1, 0, 1, 1}, // 13
	{0, 1, 1, 1}, // 14
	{1, 1, 1, 1}  // 15
};

const int ldr_thresholds[32] = {
    0, // 0
    0, // 1
    0, // 2
    0, // 3
    0, // 4
    0, // 5
    0, // 6
    0, // 7
    0, // 8
    0, // 9
    0, // 10
    0, // 11
    0, // 12
    0, // 13
    0, // 14
    0, // 15
    0, // 16
    0, // 17
    0, // 18
    0, // 19
    0, // 20
    0, // 21
    0, // 22
    0, // 23
    0, // 24
    0, // 25
    0, // 26
    0, // 27
    0, // 28
    0, // 29
    0, // 30
    0, // 31
};

const float ldr_angle = 11.25;

#endif