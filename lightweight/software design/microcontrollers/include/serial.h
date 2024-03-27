#pragma once
#ifndef _SERIAL_H_
#define _SERIAL_H_

struct STMTxData {
    double ballBearing; 
    double ballDist;
};

union STMTxDataUnion {
    STMTxData data; 
    byte bytes[sizeof(STMTxData)]; 

    STMTxDataUnion(): data() {} 
};

#endif