#include "main.h"
#include "light_ring.h"

PacketSerial TEENSY;

Layer1TxDataUnion txData;
Layer1RxDataUnion rxData;

LightRing light_ring;

void detectBall() {
  if ((analogRead(LIGHTGATE) > ball_threshold) != ball_detected) {
    txData.data.ball_detected = true;
  } else {
    txData.data.ball_detected = false;
  }
}

void kick() {
    digitalWrite(KICKER, HIGH);
    delay(100);
    digitalWrite(KICKER, LOW);
}

void onPacketReceived(const byte *buf, size_t size) {
    // Read the payload
    Layer1TxDataUnion payload;
    // Don't continue if the payload is invalid
    if (size != sizeof(payload)) return;
    std::copy(buf, buf + size, std::begin(payload.bytes));

    Serial.print(payload.data.on_line);
}

void setup() {
  light_ring.setup();
  // pinMode(LIGHTGATE, INPUT);
  pinMode(KICKER, OUTPUT);

  pinMode(D9, OUTPUT);
  analogWrite(D9, 255);

  #ifdef DEBUG
  Serial.begin(115200);
  while (!Serial) {}
  Serial.println("Debug serial connection established.");
  #endif

  Serial0.begin(115200);
  while (!Serial0) {}
  TEENSY.setStream(&Serial0);
  TEENSY.setPacketHandler(&onPacketReceived);
}

void loop() {
  TEENSY.update();
  #ifdef DEBUG
  // light_ring.calibrate();
  #else
  // light_ring.read();
  // detectBall();
  // if (TEENSY.available()) {
  //   char command = TEENSY.read();
  //   if (command == 'k') {
  //     kick();
  //   }
  // }
  // kick();
  // delay(5000);
  #endif

  TEENSY.send(txData.bytes, sizeof(txData.bytes));
}
