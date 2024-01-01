#include "main.h"

PacketSerial Cam1Serial;
PacketSerial Cam2Serial;
PacketSerial BtSerial;
PacketSerial TeensySerial;

void setup() {
  Serial.begin(9600);
  while (!Serial) {}
  digitalWrite(LED_BUILTIN, HIGH);
  delay(1000);
  digitalWrite(LED_BUILTIN, LOW);
  delay(1000);
  Serial.println("serial start");
  

  Serial2.begin(9600);
  while (!Serial2) {}
}

void loop() {
}