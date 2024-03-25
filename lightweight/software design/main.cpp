float maxVoltage = 0;
int maxIndex = 0;

void loop() {
  for (int i = 0; i < 24; i = i + 1) {
    float voltage = analogRead(irPins[i]);
    if (voltage > maxVoltage) {
      maxVoltage = voltage;
      maxIndex = i;
    }
  }
}

ballBearing = (360 / 24) * maxIndex;
ballDist = 1 / maxVoltage //tune constant

struct STMTxData {
int ballBearing; 
int ballDist;
}

union STMTxDataUnion {
    STMTxData data; 
    byte bytes[sizeof(STMTxData)]; 

    STMTxDataUnion(): data() {} 
};

STMTxDataUnion tx_data; 