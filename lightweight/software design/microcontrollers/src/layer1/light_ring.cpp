#include "light_ring.h"

void LightRing::setup()
{
    pinModeFast(S0, OUTPUT);
    pinModeFast(S1, OUTPUT);
    pinModeFast(S2, OUTPUT);
    pinModeFast(S3, OUTPUT);

    digitalWriteFast(S0, LOW);
    digitalWriteFast(S1, LOW);
    digitalWriteFast(S2, LOW);
    digitalWriteFast(S3, LOW);

    pinMode(MUX1, INPUT);
    pinMode(MUX2, INPUT);
}

void LightRing::printThresholds()
{
    Serial.println("Thresholds:");
    int threshold;
    for (int i = 0; i < 32; i++)
    {
        threshold = (ldr_max_readings[i] + ldr_min_readings[i]) / 2;
        Serial.print(threshold);
        Serial.print(", // ");
        Serial.println(i);
    }
    Serial.println();
}

void LightRing::calibrate()
{
    for (int i = 0; i < 32; i++)
    {
        ldr_min_readings[i] = 0;
        ldr_max_readings[i] = 1200;
    }

    while (true)
    {
        for (int i = 0; i < 16; i += 2)
        {
            digitalWriteFast(S0, mux_signals[i][0]);
            digitalWriteFast(S1, mux_signals[i][1]);
            digitalWriteFast(S2, mux_signals[i][2]);
            digitalWriteFast(S3, mux_signals[i][3]);

            ldr_readings[i] = analogRead(MUX1);
            ldr_readings[i + 16] = analogRead(MUX2);

            if (ldr_readings[i] > ldr_max_readings[i])
            {
                ldr_max_readings[i] = ldr_readings[i];
            }
            else if (ldr_min_readings[i] == 0 || ldr_readings[i] < ldr_min_readings[i])
            {
                ldr_min_readings[i] = ldr_readings[i];
            }

            if (ldr_readings[i + 16] > ldr_max_readings[i + 16])
            {
                ldr_max_readings[i + 16] = ldr_readings[i + 16];
            }
            else if (ldr_min_readings[i + 16] == 0 || ldr_readings[i + 16] < ldr_min_readings[i + 16])
            {
                ldr_min_readings[i + 16] = ldr_readings[i + 16];
            }
        }

        for (int i = 0; i < 32; i += 2)
        {
            Serial.print(i);
            Serial.print(": | ");
            Serial.print(ldr_min_readings[i]);
            Serial.print(" | ");
            Serial.println(ldr_max_readings[i]);
        }

        printThresholds();
    }
}

void LightRing::read()
{
    //Serial.println("reading");
    int line_start = 31, line_end = 0;

    for (int i = 0; i < 16; i += 2)
    {
        digitalWriteFast(S0, mux_signals[i][0]);
        digitalWriteFast(S1, mux_signals[i][1]);
        digitalWriteFast(S2, mux_signals[i][2]);
        digitalWriteFast(S3, mux_signals[i][3]);

        ldr_readings[i] = analogRead(MUX1);
        ldr_readings[i + 16] = analogRead(MUX2);

        if (ldr_readings[i] > ldr_thresholds[i])
        {
            if (i < line_start)
            {
                line_start = i;
            }
            if (i > line_end)
            {
                line_end = i;
            }
        }

        if (ldr_readings[i + 16] > ldr_thresholds[i + 16])
        {
            if ((i + 16) < line_start)
            {
                line_start = i + 16;
            }
            if ((i + 16) > line_end)
            {
                line_end = i + 16;
            }
        }
    }

    tx_data.data.line_track_error = ldr_readings[rx_data.line_track_ldr] - ldr_thresholds[rx_data.line_track_ldr];

    if (line_start == 31 && line_end == 0)
    {
        tx_data.data.on_line = false;
        return;
    }

    double approach_angle = (line_end - line_start) < 16 ? (line_start + (line_end - line_start) / 2) * ldr_angle + ldr_angle / 2 : 360 - ((line_end - line_start) * ldr_angle) - ((32 - line_end) * ldr_angle);
    if (approach_angle != 0)
    {
        approach_angle = 360 - approach_angle;
    }

    tx_data.data.on_line = true;
    tx_data.data.line_angle = approach_angle;
    tx_data.data.chord_length = min(abs(line_end - line_start), abs(line_start - line_end));
    tx_data.data.line_centre = (line_start + line_end) / 2;
    tx_data.data.line_start_ldr = line_start;
    tx_data.data.line_end_ldr = line_end;
    Serial.print("Line start: ");
    Serial.print(line_start);
    Serial.print(" Line end: ");
    Serial.print(line_end);
    Serial.print(" Line centre: ");
    Serial.print((line_start + (line_end - line_start) / 2));
    Serial.print(" Approach angle: ");
    Serial.println(approach_angle);

#ifdef SERIAL_DEBUG
    Serial.print("Line start: ");
    Serial.print(line_start);
    Serial.print(" Line end: ");
    Serial.print(line_end);
    Serial.print(" Line centre: ");
    Serial.print((line_start + (line_end - line_start) / 2));
    Serial.print(" Approach angle: ");
    Serial.println(approach_angle);

    // for (int i = 0; i < 32; i++)
    // {
    //     Serial.print(i);
    //     Serial.print(": | ");
    //     Serial.println(ldr_readings[i]);
    // }
#endif
}