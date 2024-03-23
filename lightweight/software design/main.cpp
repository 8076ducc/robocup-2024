//the further the ball, the lower the ir reading
//closer --> higher resistance --> lower voltage
//analogRead() voltage of ir 
void loop() {
  float maxVoltage = 0;
  int maxIndex = 0;

  for (int i = 0; i < 24; i = i + 1) {
    float voltage = analogRead(irPins[i]);
    if (voltage > maxVoltage) {
      maxVoltage = voltage;
      maxIndex = i;
    }
  }
}

ballBearing = (360 / 24) * maxIndex;
ballDist = 1 / maxVoltage //assume proportional relationship, tune constant 

struct STMTxData {
int ballBearing; //variables in struct
int ballDist;
}

union STMTxDataUnion {
    STMTxData data; //converts into data
    byte bytes[sizeof(STMTxData)]; //converts into bytes

    STMTxDataUnion(): data() {} 
};

STMTxDataUnion tx_data; //synonymous to int abc, tx_data is just an object

//void onTeensyReceived(const byte *buf, size_t size)
//{
  // Read the payload
  //STMRxDataUnion data_received;
  // Don't continue if the payload is invalid
  //if (size != sizeof(data_received))
    //return;
  //std::copy(buf, buf + size, std::begin(data_received.bytes));
//}

