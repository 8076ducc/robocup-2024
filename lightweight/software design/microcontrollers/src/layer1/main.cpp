#include "main.h"
#include "light_ring.h"

PacketSerial TeensySerial;

Layer1TxDataUnion tx_data;
Layer1RxData rx_data;

LightRing light_ring;

void detectBall()
{
  Serial.println(analogRead(LIGHTGATE));
  if (analogRead(LIGHTGATE) > ball_threshold && !rx_data.kick)
  {
    tx_data.data.ball_in_catchment = true;
  }
  else
  {
    tx_data.data.ball_in_catchment = false;
  }
}

unsigned long start_kick_time = 0;
bool kicking = false;

void onTeensyReceived(const byte *buf, size_t size)
{
  // Read the payload
  Layer1RxDataUnion data_received;
  // Don't continue if the payload is invalid
  if (size != sizeof(data_received))
    return;
  std::copy(buf, buf + size, std::begin(data_received.bytes));

  rx_data.kick = data_received.data.kick;
  rx_data.line_track_ldr = data_received.data.line_track_ldr;
}

void setup()
{
#ifdef SERIAL_DEBUG
  Serial.begin(512000);
  while (!Serial)
  {
  }
  Serial.println("Debug serial connection established.");
#endif
  light_ring.setup();
  pinMode(LIGHTGATE, INPUT);

  Serial0.begin(layer_1_serial_baud);
  //Serial0.setTxBufferSize(120);
  //Serial0.setRxBufferSize(120);
  while (!Serial0)
  {
  }
  TeensySerial.setStream(&Serial0);
  TeensySerial.setPacketHandler(&onTeensyReceived);
  Serial.println("done");
}

void loop()
{
#ifdef DEBUG
  light_ring.calibrate();
#else
  TeensySerial.update();
  light_ring.read();
  detectBall();

  TeensySerial.send(tx_data.bytes, sizeof(tx_data.bytes));
#endif
}