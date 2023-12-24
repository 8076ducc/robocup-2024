#include <main.h>
#include <light_ring.cpp>

void setup() {
  pinMode(LED, OUTPUT);
  digitalWrite(LED, HIGH);

  pinMode(S0, OUTPUT);
  pinMode(S1, OUTPUT);
  pinMode(S2, OUTPUT);
  pinMode(S3, OUTPUT);

  pinMode(MUX1, INPUT);
  pinMode(MUX2, INPUT);
  pinMode(LIGHTGATE, INPUT);

  pinMode(KICKER, OUTPUT);

  TEENSY.begin(115200);
}

void loop() {
  detectLight();
  detectBall();
  if (TEENSY.available()) {
    char command = TEENSY.read();
    if (command == 'k') {
      kick();
    }
  }
}

void detectBall() {
  if (digitalRead(LIGHTGATE) == HIGH) {
    TEENSY.print("b");
  }
}

void kick() {
    digitalWrite(KICKER, HIGH);
    delay(500);
    digitalWrite(KICKER, LOW);
}