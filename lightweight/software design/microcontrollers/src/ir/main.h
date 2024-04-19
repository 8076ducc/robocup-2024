#pragma once
#ifndef _MAIN_H_
#define _MAIN_H_

#include <common.h>
#include <numeric>
#include <stm32f103c_variant_generic.h>
#include <movingAvg.h>

#define STM_LED PA3

extern STMTxDataUnion tx_data;

constexpr std::array<uint8_t, 24> IR_PINS = {
    PA2,
    PA1,
    PA0,
    PC15,
    PC14,
    PC13,
    PB9,
    PB8, 
    PB7,
    PB6,
    PB5,
    PB4,
    PB3,
    PA15,
    PA12,
    PA11,
    PB12, 
    PB15,
    PB14,
    PB0,
    PA7,
    PA6,
    PA5,
    PA4
};

#endif