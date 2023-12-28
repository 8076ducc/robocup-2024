#ifndef SHARED_H
#define SHARED_H

struct Layer1TxData {
  bool on_line;
  double target_angle;
  double chord_length;
  bool ball_detected;
};

struct Layer1RxData {
  bool kick;
};

union Layer1TxDataUnion {
    Layer1TxData data;
    byte bytes[sizeof(Layer1TxData)];

    Layer1TxDataUnion() : data() {}
};

union Layer1RxDataUnion {
    Layer1RxData data;
    byte bytes[sizeof(Layer1RxData)];

    Layer1RxDataUnion() : data() {}
};

#endif