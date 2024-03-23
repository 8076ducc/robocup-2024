// #include <Arduino.h>
// // Server Code
// #include <BLEDevice.h>
// #include <BLEUtils.h>
// #include <BLEServer.h>

// #define ROBOT1

// #ifdef ROBOT1

// #define SERVICE_UUID "31914bad-e95b-4227-b843-fb68f20c8149"
// #define CHARACTERISTIC_UUID "17966e80-63d4-49d2-a03e-2f705e8784c2"

// // The remote service we wish to connect to.
// static BLEUUID serviceUUID("b869caa5-21e2-4ec5-94c5-3287b1e69343");
// // The characteristic of the remote service we are interested in.
// static BLEUUID charUUID("f565bd56-6236-4417-a726-db6e1179ab7b");

// #endif

// #ifdef ROBOT2

// #define SERVICE_UUID "b869caa5-21e2-4ec5-94c5-3287b1e69343"
// #define CHARACTERISTIC_UUID "f565bd56-6236-4417-a726-db6e1179ab7b"

// // The remote service we wish to connect to.
// static BLEUUID serviceUUID("31914bad-e95b-4227-b843-fb68f20c8149");
// // The characteristic of the remote service we are interested in.
// static BLEUUID charUUID("17966e80-63d4-49d2-a03e-2f705e8784c2");

// #endif

// static boolean doConnect = false;
// static boolean connected = false;
// static boolean doScan = false;
// static BLERemoteCharacteristic *pRemoteCharacteristic;
// static BLEAdvertisedDevice *myDevice;

// static void notifyCallback(
//     BLERemoteCharacteristic *pBLERemoteCharacteristic,
//     uint8_t *pData,
//     size_t length,
//     bool isNotify)
// {
//   Serial.print("Notify callback for characteristic ");
//   Serial.print(pBLERemoteCharacteristic->getUUID().toString().c_str());
//   Serial.print(" of data length ");
//   Serial.println(length);
//   Serial.print("data: ");
//   Serial.write(pData, length);
//   Serial.println();
// }

// class MyClientCallback : public BLEClientCallbacks
// {
//   void onConnect(BLEClient *pclient)
//   {
//   }

//   void onDisconnect(BLEClient *pclient)
//   {
//     connected = false;
//     Serial.println("onDisconnect");
//   }
// };

// bool connectToServer()
// {
//   Serial.print("Forming a connection to ");
//   Serial.println(myDevice->getAddress().toString().c_str());

//   BLEClient *pClient = BLEDevice::createClient();
//   Serial.println(" - Created client");

//   pClient->setClientCallbacks(new MyClientCallback());

//   // Connect to the remove BLE Server.
//   pClient->connect(myDevice); // if you pass BLEAdvertisedDevice instead of address, it will be recognized type of peer device address (public or private)
//   Serial.println(" - Connected to server");
//   pClient->setMTU(517); // set client to request maximum MTU from server (default is 23 otherwise)

//   // Obtain a reference to the service we are after in the remote BLE server.
//   BLERemoteService *pRemoteService = pClient->getService(serviceUUID);
//   if (pRemoteService == nullptr)
//   {
//     Serial.print("Failed to find our service UUID: ");
//     Serial.println(serviceUUID.toString().c_str());
//     pClient->disconnect();
//     return false;
//   }
//   Serial.println(" - Found our service");

//   // Obtain a reference to the characteristic in the service of the remote BLE server.
//   pRemoteCharacteristic = pRemoteService->getCharacteristic(charUUID);
//   if (pRemoteCharacteristic == nullptr)
//   {
//     Serial.print("Failed to find our characteristic UUID: ");
//     Serial.println(charUUID.toString().c_str());
//     pClient->disconnect();
//     return false;
//   }
//   Serial.println(" - Found our characteristic");

//   // Read the value of the characteristic.
//   if (pRemoteCharacteristic->canRead())
//   {
//     std::string value = pRemoteCharacteristic->readValue();
//     Serial.print("The characteristic value was: ");
//     Serial.println(value.c_str());
//   }

//   if (pRemoteCharacteristic->canNotify())
//     pRemoteCharacteristic->registerForNotify(notifyCallback);

//   connected = true;
//   return true;
// }
// /**
//  * Scan for BLE servers and find the first one that advertises the service we are looking for.
//  */
// class MyAdvertisedDeviceCallbacks : public BLEAdvertisedDeviceCallbacks
// {
//   /**
//    * Called for each advertising BLE server.
//    */
//   void onResult(BLEAdvertisedDevice advertisedDevice)
//   {
//     Serial.print("BLE Advertised Device found: ");
//     Serial.println(advertisedDevice.toString().c_str());

//     // We have found a device, let us now see if it contains the service we are looking for.
//     if (advertisedDevice.haveServiceUUID() && advertisedDevice.isAdvertisingService(serviceUUID))
//     {

//       BLEDevice::getScan()->stop();
//       myDevice = new BLEAdvertisedDevice(advertisedDevice);
//       doConnect = true;
//       doScan = true;

//     } // Found our server
//   }   // onResult
// };    // MyAdvertisedDeviceCallbacks

// void setup()
// {
//   Serial.begin(115200);
//   while (!Serial)
//   {
//   }
//   Serial.println("Starting BLE work!");

// #ifdef ROBOT1
//   BLEDevice::init("ROBOT1");
// #endif

// #ifdef ROBOT2
//   BLEDevice::init("ROBOT2");
// #endif

//   BLEServer *pServer = BLEDevice::createServer();
//   BLEService *pService = pServer->createService(SERVICE_UUID);
//   BLECharacteristic *pCharacteristic = pService->createCharacteristic(
//       CHARACTERISTIC_UUID,
//       BLECharacteristic::PROPERTY_READ |
//           BLECharacteristic::PROPERTY_WRITE);

//   pCharacteristic->setValue("Hello World");
//   pService->start();
//   // BLEAdvertising *pAdvertising = pServer->getAdvertising();  // this still is working for backward compatibility
//   BLEAdvertising *pAdvertising = BLEDevice::getAdvertising();
//   pAdvertising->addServiceUUID(SERVICE_UUID);
//   pAdvertising->setScanResponse(true);
//   pAdvertising->setMinPreferred(0x06); // functions that help with iPhone connections issue
//   pAdvertising->setMinPreferred(0x12);
//   BLEDevice::startAdvertising();
//   Serial.println("Characteristic defined! Now you can read it in your phone!");

//   // Retrieve a Scanner and set the callback we want to use to be informed when we
//   // have detected a new device.  Specify that we want active scanning and start the
//   // scan to run for 5 seconds.
//   BLEScan *pBLEScan = BLEDevice::getScan();
//   pBLEScan->setAdvertisedDeviceCallbacks(new MyAdvertisedDeviceCallbacks());
//   pBLEScan->setInterval(1349);
//   pBLEScan->setWindow(449);
//   pBLEScan->setActiveScan(true);
//   pBLEScan->start(5, false);
// }

// void loop()
// {
//   // If the flag "doConnect" is true then we have scanned for and found the desired
//   // BLE Server with which we wish to connect.  Now we connect to it.  Once we are
//   // connected we set the connected flag to be true.
//   if (doConnect == true)
//   {
//     if (connectToServer())
//     {
//       Serial.println("We are now connected to the BLE Server.");
//     }
//     else
//     {
//       Serial.println("We have failed to connect to the server; there is nothin more we will do.");
//     }
//     doConnect = false;
//   }

//   // If we are connected to a peer BLE Server, update the characteristic each time we are reached
//   // with the current time since boot.
//   if (connected)
//   {
//     String newValue = "Time since boot: " + String(millis() / 1000);
//     Serial.println("Setting new characteristic value to \"" + newValue + "\"");

//     // Set the characteristic's value to be the array of bytes that is actually a string.
//     pRemoteCharacteristic->writeValue(newValue.c_str(), newValue.length());
//   }
//   else if (doScan)
//   {
//     BLEDevice::getScan()->start(0); // this is just example to start scan after disconnect, most likely there is better way to do it in arduino
//   }
// }