#include "main.h"
#include "light_ring.h"

PacketSerial TeensySerial;

Layer1TxDataUnion tx_data;

LightRing light_ring;

void detectBall()
{
  if (analogRead(LIGHTGATE) < ball_threshold)
  {
    tx_data.data.ball_in_catchment = true;
  }
  else
  {
    tx_data.data.ball_in_catchment = false;
  }
}

void kick()
{
  digitalWrite(KICKER, HIGH);
  delay(100);
  digitalWrite(KICKER, LOW);
}

void onTeensyReceived(const byte *buf, size_t size)
{
  // Read the payload
  Layer1RxDataUnion data_received;
  // Don't continue if the payload is invalid
  if (size != sizeof(data_received))
    return;
  std::copy(buf, buf + size, std::begin(data_received.bytes));
}

void setup()
{
#ifdef SERIAL_DEBUG
  Serial.begin(115200);
  while (!Serial)
  {
  }
  Serial.println("Debug serial connection established.");
#endif

  light_ring.setup();
  pinMode(LIGHTGATE, INPUT);
  pinMode(KICKER, OUTPUT);

  Serial0.begin(serial_baud);
  while (!Serial0)
  {
  }
  TeensySerial.setStream(&Serial0);
  TeensySerial.setPacketHandler(&onTeensyReceived);
}

void loop()
{
#ifdef DEBUG
  light_ring.calibrate();
#else
  TeensySerial.update();
  light_ring.read();
  detectBall();
  // if (rx_data.data.kick) {
  //   kick();
  // }
  TeensySerial.send(tx_data.bytes, sizeof(tx_data.bytes));
#endif
}