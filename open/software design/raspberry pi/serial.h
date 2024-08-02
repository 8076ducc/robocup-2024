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
    int yellow_open_x;
    int yellow_open_y;
    bool blue_goal_detected;
    int blue_goal_x;
    int blue_goal_y;
    int blue_open_x;
    int blue_open_y;
    double fps;
};

struct CamRxData
{
    bool track_orange;
    bool track_yellow;
    bool track_blue;
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

CamRxData rx_data;
CamTxDataUnion tx_data;

void setUpSerial()
{
    wiringPiSetup();
    wiringPiSetupGpio();

    serialDeviceId = serialOpen("/dev/serial0", 2000000);

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

uint8_t PacketMarker = 0;
const size_t ReceiveBufferSize = 256;
uint8_t _receiveBuffer[ReceiveBufferSize];
size_t _receiveBufferIndex = 0;
bool _recieveBufferOverflow = false;

void serialWrite(const uint8_t *buffer, size_t size)
{
    uint8_t _encodeBuffer[COBS::getEncodedBufferSize(size)];

    size_t numEncoded = COBS::encode(buffer,
                                     size,
                                     _encodeBuffer);

    write(serialDeviceId, &_encodeBuffer, numEncoded);
    write(serialDeviceId, &PacketMarker, 1);

    //std::cout << "sending serial" << std::endl;
}

void serialRead()
{
    while (serialDataAvail(serialDeviceId) != -1)
    {
        uint8_t data;
        read(serialDeviceId, &data, 1);

        if (data == PacketMarker)
        {
            // If packet marker is received
            uint8_t _decodeBuffer[ReceiveBufferSize];
            size_t numDecoded = COBS::decode(_receiveBuffer, _receiveBufferIndex, _decodeBuffer);
            _receiveBufferIndex = 0;
            _recieveBufferOverflow = false;

            CamRxDataUnion data_received;

            // Don't continue if the payload is invalid
            if (numDecoded != sizeof(data_received))
                return;

            std::copy(_decodeBuffer, _decodeBuffer + numDecoded, std::begin(data_received.bytes));

            rx_data.track_orange = data_received.data.track_orange;
            rx_data.track_yellow = data_received.data.track_yellow;
            rx_data.track_blue = data_received.data.track_blue;

            // std::cout << "orange: " << rx_data.track_orange << "blue: " << rx_data.track_yellow << "yellow: " << rx_data.track_blue << std::endl;
        }
        else
        {
            // If data is not packet marker, store it in receive buffer
            if ((_receiveBufferIndex + 1) < ReceiveBufferSize)
            {
                _receiveBuffer[_receiveBufferIndex++] = data;
            }
            else
            {
                _recieveBufferOverflow = true; // Set buffer overflow flag
            }
        }
    }
}

#endif
