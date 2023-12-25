#include <Arduino.h>

void setup() {
  // put your setup code here, to run once:
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, HIGH);
  delay(1000);
  digitalWrite(LED_BUILTIN, LOW);
  delay(1000);

  Serial.begin(9600);
  while (!Serial) {}
  digitalWrite(LED_BUILTIN, HIGH);
  delay(1000);
  digitalWrite(LED_BUILTIN, LOW);
  delay(1000);
  Serial.println("serial start");
  

  Serial2.begin(9600);
  while (!Serial2) {}
  digitalWrite(LED_BUILTIN, HIGH);
  delay(1000);
  digitalWrite(LED_BUILTIN, LOW);
  delay(1000);  
}

void loop() {
  // put your main code here, to run repeatedly:
  if (Serial2.available()) {
    int msg = Serial2.parseInt();
    Serial.print("msg: ");
    Serial.println(msg);
  }
}