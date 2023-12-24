#include "main.h"
#include "motion.h"

void setup() {
  pinMode(LED, OUTPUT);
  digitalWrite(LED, LOW);

  LAYER1.begin(9600);
  while (!LAYER1) {}
  digitalWrite(LED, HIGH);

  LIDAR.begin(9600);
  while (!LIDAR) {}
  digitalWrite(LED, LOW);

  #ifdef DEBUG
  Serial.begin(9600);
  while (!Serial) {}
  Serial.println("Serial ready.");
  #endif
}

void loop() {

  LIDAR.println("Hello, world!");

  if (LAYER1.available()) {
    // int received = LAYER1.parseInt();
    #ifdef DEBUG
    Serial.println(char(LAYER1.available()));
    #endif
  }

  // if ((millis() / 1000) < 3) {
  //   move(1, 0, 0);
  // } else {
  //   delay(100);
  //   move(1, 90, 0);
  // } 
}