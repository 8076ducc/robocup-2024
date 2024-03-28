#pragma once

#ifndef SERIAL_H
#define SERIAL_H

#include <stdint.h> // uint8_t, etc.
#include <stddef.h> // size_t
#include <wiringPi.h>
#include <wiringSerial.h>
#include "cobs.h"

int serialDeviceId;

struct CamTxData
{
    bool ball_detected;
    int ball_x;
    int ball_y;
    bool yellow_goal_detected;
    int yellow_goal_x;
    int yellow_goal_y;
    bool blue_goal_detected;
    int blue_goal_x;
    int blue_goal_y;
};

struct CamRxData
{
};

union CamTxDataUnion
{
    CamTxData data;
    byte bytes[sizeof(CamTxData)];

    CamTxDataUnion() : data() {}
};

union CamRxDataUnion
{
    CamRxData data;
    byte bytes[sizeof(CamRxData)];

    CamRxDataUnion() : data() {}
};

void setUpSerial()
{
    wiringPiSetup();
    wiringPiSetupGpio();

    serialDeviceId = serialOpen("/dev/serial0", 1000000);

    if (serialDeviceId < 0)
    {
        std::cout << "unable to open serial device" << std::endl;
        // return 0;
    }

    if (wiringPiSetup() == -1)
    {
        std::cout << "unable to start wiringPi" << std::endl;
        // return 0;
    }
}

void serialWrite(const uint8_t *buffer, size_t size)
{
    uint8_t PacketMarker = 0;
    uint8_t _encodeBuffer[COBS::getEncodedBufferSize(size)];

    size_t numEncoded = COBS::encode(buffer,
                                     size,
                                     _encodeBuffer);

    write(serialDeviceId, &_encodeBuffer, numEncoded);
    write(serialDeviceId, &PacketMarker, 1);
}

#endif
