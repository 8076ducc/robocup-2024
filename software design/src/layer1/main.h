#pragma once

#ifndef MAIN_H
#define MAIN_H

#define DEBUG
#define STRATEGY 1
// #define WHITE_BOT
// #define BLACK_BOT

#include <common.h>

#define S0 D2
#define S1 D3
#define S2 D4
#define S3 D5

#define MUX1 D0
#define MUX2 D1
#define LIGHTGATE D9

#define KICKER D9

const int ball_threshold = 0;
extern Layer1TxDataUnion txData;
extern Layer1RxDataUnion rxData;

#endif