#include "main.h"

STMTxDataUnion tx_data;
PacketSerial teensy_serial;

std::array<size_t, 24> sorted_indices;

void setupIRRing() {
    for (auto i : IR_PINS) pinMode(IR_PINS[i], INPUT);
}

void readIRRing() { 
    std::array<double, 24> ir_readings = {0};
    int read_count = 0;
    const int start_time = micros();
    while ((micros() - start_time) < 833 * 4) {
        for (int i = 0; i < 24; ++i) {
            if (!digitalRead(IR_PINS[i])) ir_readings[i] += 1;
        }
        ++read_count;
    }
    for (int i = 0; i < 24; ++i) {
        ir_readings[i] /= read_count;
    }
    sorted_indices = {0};
    std::iota(sorted_indices.begin(), sorted_indices.end(), 0);
    std::sort(sorted_indices.begin(), sorted_indices.end(),
            [ir_readings](size_t i, size_t j) {
                return ir_readings[i] > ir_readings[j];
            });

    //process 4 strongest readings and find dist and bearing
    int x = 0;
    int y = 0;
    for (int i = 0; i < 4; ++i) {
        int angle = radians((sorted_indices[i] - 1) * 15);
        x += sin(angle) * ir_readings[sorted_indices[i]];
        y += cos(angle) * ir_readings[sorted_indices[i]];
    }

    if (x == 0 && y == 0) tx_data.data.ball_detected = false;
    else tx_data.data.ball_detected = true;

    tx_data.data.readings = ir_readings;
    tx_data.data.ball_dist = sqrt(x * x + y * y);
    tx_data.data.ball_bearing = atan2(x, y);
}

void setup() {
    pinMode(STM_LED, OUTPUT);
    digitalWrite(STM_LED, HIGH);
    Serial1.begin(512000);
    setupIRRing();
    while (!Serial1)
    {
    }
    teensy_serial.setStream(&Serial1);
}

void loop() {
    teensy_serial.update();
    readIRRing();
    teensy_serial.send(tx_data.bytes, sizeof(tx_data.bytes));
}