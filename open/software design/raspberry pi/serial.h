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
    // Pose ball_pose;
    int ball_pose_x;
    int ball_pose_y;
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

    serialDeviceId = serialOpen("/dev/serial0", 115200);

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
