#include <main.h>
// #include <light_ring.cpp>

void setup() {
  // put your setup code here, to run once:
  pinMode(LED, OUTPUT);
  digitalWrite(LED, HIGH);
  delay(1000);
  digitalWrite(LED, LOW);
  delay(1000);

  TEENSY.begin(9600);
  while (!TEENSY) {}
  digitalWrite(LED, HIGH);
  delay(1000);
  digitalWrite(LED, LOW);
  delay(1000);  
}

// void setup() {
//   pinMode(LED, OUTPUT);
//   digitalWrite(LED, HIGH);
//   delay(1000);
//   // digitalWrite(LED, LOW);

//   pinMode(S0, OUTPUT);
//   pinMode(S1, OUTPUT);
//   pinMode(S2, OUTPUT);
//   pinMode(S3, OUTPUT);

//   pinMode(MUX1, INPUT);
//   pinMode(MUX2, INPUT);
//   pinMode(LIGHTGATE, INPUT);

//   pinMode(KICKER, OUTPUT);

//   // Serial.begin(9600);
//   // while (!Serial) {}
//   // Serial.println("Serial connection established.");

//   TEENSY.begin(9600);
//   // digitalWrite(LED, HIGH);
//   // delay(1000);
//   // digitalWrite(LED, LOW);
//   while (!TEENSY) {}
//   TEENSY.println("Teensy connection established.");
//   // digitalWrite(LED, HIGH);
//   // delay(1000);
//   // digitalWrite(LED, LOW);
// }

void loop() {
  TEENSY.print("l,");
  TEENSY.print(160);
  // detectLight();
  // detectBall();
  // if (TEENSY.available()) {
  //   char command = TEENSY.read();
  //   if (command == 'k') {
  //     kick();
  //   }
  // }
}

// void detectBall() {
//   if (digitalRead(LIGHTGATE) == HIGH) {
//     TEENSY.print("b");
//   }
// }

// void kick() {
//     digitalWrite(KICKER, HIGH);
//     delay(500);
//     digitalWrite(KICKER, LOW);
// }