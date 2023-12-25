#include <main.h>
#include <light_ring.h>

LightRing light_ring;

bool ball_detected = false;

void detectBall() {
  if ((analogRead(LIGHTGATE) > ball_threshold) != ball_detected) {
    TEENSY.print("b,");
    ball_detected = !ball_detected;
  }
}

void kick() {
    digitalWrite(KICKER, HIGH);
    delay(500);
    digitalWrite(KICKER, LOW);
}

void setup() {
  light_ring.setup();
  pinMode(LIGHTGATE, INPUT);

  pinMode(KICKER, OUTPUT);

  #ifdef DEBUG
  Serial.begin(9600);
  while (!Serial) {}
  Serial.println("Serial connection established.");
  #endif

  TEENSY.begin(115200);
  while (!TEENSY) {}
}

void loop() {
  #ifdef DEBUG
  light_ring.calibrate();
  #else
  light_ring.read();
  detectBall();
  if (TEENSY.available()) {
    char command = TEENSY.read();
    if (command == 'k') {
      kick();
    }
  }
  #endif
}