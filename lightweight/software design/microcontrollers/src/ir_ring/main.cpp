#include "main.h"

void setup() {
    Serial.begin(115200);
    //while(!Serial);
    Serial.println("serial begun");
}

void loop() {
    Serial.println("serial");
}