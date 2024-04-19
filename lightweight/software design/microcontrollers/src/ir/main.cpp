#include "main.h"

STMTxDataUnion tx_data;
PacketSerial teensy_serial;

std::array<size_t, 24> sorted_indices;

movingAvg avg_strength(25);

double convertStrength(double strength) {
    double distance = 0.0005327277 * pow(strength, 3) 
                    - 0.1411947477 * pow(strength, 2) 
                    + 10.1001602221 * strength 
                    - 78.2515478487;
    return distance;
}

void setupIRRing() {
    for (auto i : IR_PINS) pinMode(IR_PINS[i], INPUT);
    avg_strength.begin();
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
    ir_readings[3] = (0.75 * ir_readings[2]) + (0.25 * ir_readings[6]);
    ir_readings[4] = (0.5 * ir_readings[2]) + (0.5 * ir_readings[6]);
    ir_readings[5] = (0.25 * ir_readings[2]) + (0.75 * ir_readings[6]);
    ir_readings[16] = (0.5 * ir_readings[15]) + (0.5 * ir_readings[17]);
    sorted_indices = {0};
    std::iota(sorted_indices.begin(), sorted_indices.end(), 0);
    std::sort(sorted_indices.begin(), sorted_indices.end(),
            [ir_readings](size_t i, size_t j) {
                return ir_readings[i] > ir_readings[j];
            });

    //process 4 strongest readings and find dist and bearing
    double x = 0;
    double y = 0;
    for (int i = 0; i < 12; ++i) {
        double angle = radians((sorted_indices[i]) * -15);
        x += sin(angle) * ir_readings[sorted_indices[i]] * 100;
        y += cos(angle) * ir_readings[sorted_indices[i]] * 100;
    }

    tx_data.data.ball_bearing = degrees(atan2(x, y)) + 90;

    x = 0;
    y = 0;

    for (int i = 0; i < 4; ++i) {
        double angle = radians((sorted_indices[i]) * -15);
        x += sin(angle) * ir_readings[sorted_indices[i]] * 100;
        y += cos(angle) * ir_readings[sorted_indices[i]] * 100;
    }

    if (x == 0 && y == 0) tx_data.data.ball_detected = false;
    else tx_data.data.ball_detected = true;

    tx_data.data.readings = ir_readings;
    tx_data.data.ball_dist = avg_strength.reading(sqrt(x * x + y * y));
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