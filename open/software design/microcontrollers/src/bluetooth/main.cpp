/**
 * A BLE client example that is rich in capabilities.
 * There is a lot new capabilities implemented.
 * author unknown
 * updated by chegewara and MoThunderz
 */

#include "main.h"

// TRANSFER to teensy
BtTxDataUnion tx_data;

// receive from other robot
BtRxDataUnion external_rx_data;

// transfer to other robot; RECEIVE from teensy
BtRxDataUnion rx_data;

PacketSerial TeensySerial;

#define BOT1

void onTeensyReceived(const byte *buf, size_t size)
{
    // Read the payload
    BtRxDataUnion data_received;
    // Don't continue if the payload is invalid
    if (size != sizeof(data_received))
        return;
    std::copy(buf, buf + size, std::begin(data_received.bytes));

    rx_data.data.ball_detected = data_received.data.ball_detected;
    rx_data.data.ball_pose = data_received.data.ball_pose;
    rx_data.data.robot_pose = data_received.data.robot_pose;

    // Serial.print("Robot Pose: ");
    // Serial.print(rx_data.data.robot_pose.x);
    // Serial.print(", ");
    // Serial.print(rx_data.data.robot_pose.y);
    // Serial.print(", ");
    // Serial.println(rx_data.data.robot_pose.bearing);
}

#ifdef BOT1
#include "BLEDevice.h"
// #include "BLEScan.h"

// Define UUIDs:
static BLEUUID serviceUUID("d9c3070a-96b8-4053-8115-e3af0af0207f");
static BLEUUID charUUID_1("1ad3eb69-dbe5-496d-a66e-5c3c9cf44d56");
static BLEUUID charUUID_2("66857cb2-b396-44da-99bc-029e6b4731d4");

// Some variables to keep track on device connected
static boolean doConnect = false;
static boolean connected = false;
static boolean doScan = false;
int last_connect_time = 99999999;

// Define pointer for the BLE connection
static BLEAdvertisedDevice *myDevice;
BLERemoteCharacteristic *pRemoteChar_1;
BLERemoteCharacteristic *pRemoteChar_2;

BtTxData txData;
BtRxData rxData;

// Callback function for Notify function
static void notifyCallback(BLERemoteCharacteristic *pBLERemoteCharacteristic,
                           uint8_t *pData,
                           size_t length,
                           bool isNotify)
{
    if (pBLERemoteCharacteristic->getUUID().toString() == charUUID_1.toString())
    {

        // convert received bytes to integer
        uint32_t counter = pData[0];
        for (int i = 1; i < length; i++)
        {
            counter = counter | (pData[i] << i * 8);
        }

        // print to Serial
        Serial.print("Characteristic 1 (Notify) from server: ");
        Serial.println(counter);
    }
}

// Callback function that is called whenever a client is connected or disconnected
class MyClientCallback : public BLEClientCallbacks
{
    void onConnect(BLEClient *pclient)
    {
    }

    void onDisconnect(BLEClient *pclient)
    {
        connected = false;
    }
};

// Function to chech Characteristic
bool connectCharacteristic(BLERemoteService *pRemoteService, BLERemoteCharacteristic *l_BLERemoteChar)
{
    // Obtain a reference to the characteristic in the service of the remote BLE server.
    if (l_BLERemoteChar == nullptr)
    {
        Serial.print("Failed to find one of the characteristics");
        Serial.print(l_BLERemoteChar->getUUID().toString().c_str());
        return false;
    }
    Serial.println(" - Found characteristic: " + String(l_BLERemoteChar->getUUID().toString().c_str()));

    if (l_BLERemoteChar->canNotify())
        l_BLERemoteChar->registerForNotify(notifyCallback);

    return true;
}

// Function that is run whenever the server is connected
bool connectToServer()
{
    Serial.print("Forming a connection to ");
    Serial.println(myDevice->getAddress().toString().c_str());

    BLEClient *pClient = BLEDevice::createClient();
    Serial.println(" - Created client");

    pClient->setClientCallbacks(new MyClientCallback());

    // Connect to the remove BLE Server.
    pClient->connect(myDevice); // if you pass BLEAdvertisedDevice instead of address, it will be recognized type of peer device address (public or private)
    Serial.println(" - Connected to server");

    // Obtain a reference to the service we are after in the remote BLE server.
    BLERemoteService *pRemoteService = pClient->getService(serviceUUID);
    if (pRemoteService == nullptr)
    {
        Serial.print("Failed to find our service UUID: ");
        Serial.println(serviceUUID.toString().c_str());
        pClient->disconnect();
        return false;
    }
    Serial.println(" - Found our service");

    connected = true;
    pRemoteChar_1 = pRemoteService->getCharacteristic(charUUID_1);
    pRemoteChar_2 = pRemoteService->getCharacteristic(charUUID_2);
    if (connectCharacteristic(pRemoteService, pRemoteChar_1) == false)
        connected = false;
    else if (connectCharacteristic(pRemoteService, pRemoteChar_2) == false)
        connected = false;

    if (connected == false)
    {
        pClient->disconnect();
        Serial.println("At least one characteristic UUID not found");
        return false;
    }
    return true;
}

// Scan for BLE servers and find the first one that advertises the service we are looking for.
class MyAdvertisedDeviceCallbacks : public BLEAdvertisedDeviceCallbacks
{
    // Called for each advertising BLE server.
    void onResult(BLEAdvertisedDevice advertisedDevice)
    {
        Serial.print("BLE Advertised Device found: ");
        Serial.println(advertisedDevice.toString().c_str());
        tx_data.data.robot_detected = false;

        // We have found a device, let us now see if it contains the service we are looking for.
        if (advertisedDevice.haveServiceUUID() && advertisedDevice.isAdvertisingService(serviceUUID))
        {

            BLEDevice::getScan()->stop();
            myDevice = new BLEAdvertisedDevice(advertisedDevice);
            doConnect = true;
            doScan = true;

        } // Found our server
    }     // onResult
};        // MyAdvertisedDeviceCallbacks

void setup()
{
#ifdef SERIAL_DEBUG
    Serial.begin(serial_monitor_baud);
    Serial.println("Starting Arduino BLE Client application...");
    BLEDevice::init("");
#endif

    Serial0.begin(bt_serial_baud);
    // Serial0.setTxBufferSize(120);
    // Serial0.setRxBufferSize(120);
    while (!Serial0)
    {
    }
    TeensySerial.setStream(&Serial0);
    TeensySerial.setPacketHandler(&onTeensyReceived);

    // Retrieve a Scanner and set the callback we want to use to be informed when we
    // have detected a new device.  Specify that we want active scanning and start the
    // scan to run for 5 seconds.
    BLEScan *pBLEScan = BLEDevice::getScan();
    pBLEScan->setAdvertisedDeviceCallbacks(new MyAdvertisedDeviceCallbacks());
    pBLEScan->setInterval(1349);
    pBLEScan->setWindow(449);
    pBLEScan->setActiveScan(true);
    pBLEScan->start(5, false);
} // End of setup.

void loop()
{
    TeensySerial.update();
    // If the flag "doConnect" is true then we have scanned for and found the desired
    // BLE Server with which we wish to connect.  Now we connect to it.  Once we are
    // connected we set the connected flag to be true.
    if (doConnect == true)
    {
        if (connectToServer())
        {
            Serial.println("We are now connected to the BLE Server.");
        }
        else
        {
            Serial.println("We have failed to connect to the server; there is nothin more we will do.");
        }
        doConnect = false;
    }

    // If we are connected to a peer BLE Server, update the characteristic each time we are reached
    // with the current time since boot.
    if (connected)
    {
        // std::string rxValue = pRemoteChar_2->readValue();
        uint8_t buff = pRemoteChar_2->readUInt8();
        uint8_t *buf = &buff;

        std::copy(buf, buf + sizeof(buf), std::begin(external_rx_data.bytes));

        tx_data.data.robot_detected = true;
        tx_data.data.ball_detected = external_rx_data.data.ball_detected;
        tx_data.data.ball_pose = external_rx_data.data.ball_pose;
        tx_data.data.robot_pose = external_rx_data.data.robot_pose;

        // Serial.print("Characteristic 2 (readValue): ");
        // Serial.println(rxValue.c_str());
        // Serial.print(tx_data.data.robot_pose.x);
        // Serial.print(", ");
        // Serial.println(tx_data.data.robot_pose.y);

        // String txValue = "String with random value from client: " + String(-random(1000));
        // Serial.println("Characteristic 2 (writeValue): " + txValue);

        // Set the characteristic's value to be the array of bytes that is actually a string.
        pRemoteChar_2->writeValue(rx_data.bytes, sizeof(rx_data.bytes));
    }
    else if (doScan)
    {
        Serial.println("missing data");
        BLEDevice::getScan()->start(0); // this is just example to start scan after disconnect, most likely there is better way to do it in arduino
        tx_data.data.robot_detected = false;
    }

    // tx_data.data.robot_pose.x = 10;
    // tx_data.data.robot_pose.y = 30;

    // Serial.print("Robot Pose: ");
    // Serial.print(tx_data.data.robot_pose.x);
    // Serial.print(", ");
    // Serial.println(tx_data.data.robot_pose.y);
    Serial.println(tx_data.data.robot_detected);
    TeensySerial.send(tx_data.bytes, sizeof(tx_data.bytes));

    // In this example "delay" is used to delay with one second. This is of course a very basic
    // implementation to keep things simple. I recommend to use millis() for any production code
}

#else
#include <BLEDevice.h>
#include <BLEServer.h>
#include <BLEUtils.h>
#include <BLE2902.h>

// Initialize all pointers
BLEServer *pServer = NULL;                   // Pointer to the server
BLECharacteristic *pCharacteristic_1 = NULL; // Pointer to Characteristic 1
BLECharacteristic *pCharacteristic_2 = NULL; // Pointer to Characteristic 2
BLEDescriptor *pDescr_1;                     // Pointer to Descriptor of Characteristic 1
BLE2902 *pBLE2902_1;                         // Pointer to BLE2902 of Characteristic 1
BLE2902 *pBLE2902_2;                         // Pointer to BLE2902 of Characteristic 2

// Some variables to keep track on device connected
bool deviceConnected = false;
bool oldDeviceConnected = false;
int last_write_time = 999999999;

// Variable that will continuously be increased and written to the client
uint32_t value = 0;

// See the following for generating UUIDs:
// https://www.uuidgenerator.net/
// UUIDs used in this example:
#define SERVICE_UUID "d9c3070a-96b8-4053-8115-e3af0af0207f"
#define CHARACTERISTIC_UUID_1 "1ad3eb69-dbe5-496d-a66e-5c3c9cf44d56"
#define CHARACTERISTIC_UUID_2 "66857cb2-b396-44da-99bc-029e6b4731d4"

// Callback function that is called whenever a client is connected or disconnected
class MyServerCallbacks : public BLEServerCallbacks
{
    void onConnect(BLEServer *pServer)
    {
        Serial.println("connected");
        deviceConnected = true;
    };

    void onDisconnect(BLEServer *pServer)
    {
        deviceConnected = false;
        Serial.println("disconnected");
        tx_data.data.robot_detected = false;
        TeensySerial.send(tx_data.bytes, sizeof(tx_data.bytes));
    }
};

class CharacteristicCallBack : public BLECharacteristicCallbacks
{
    void onWrite(BLECharacteristic *pChar) override
    {
        // std::string pChar2_value_stdstr = pChar->getValue();
        // String pChar2_value_string = String(pChar2_value_stdstr.c_str());

        uint8_t *buf = pChar->getData();
        std::copy(buf, buf + sizeof(buf), std::begin(external_rx_data.bytes));

        tx_data.data.robot_detected = true;
        TeensySerial.send(tx_data.bytes, sizeof(tx_data.bytes));
        tx_data.data.ball_detected = external_rx_data.data.ball_detected;
        tx_data.data.ball_pose = external_rx_data.data.ball_pose;
        tx_data.data.robot_pose = external_rx_data.data.robot_pose;

        // Serial.print("Characteristic 2 (readValue): ");
        // Serial.println(rxValue.c_str());
        Serial.println(tx_data.data.robot_detected);

        String txValue = "hello from server";
        uint8_t identify = 36;
        uint8_t *identifier = &identify;
        pCharacteristic_2->setValue(identifier, sizeof(identifier));
        last_write_time = millis();
    }
};

void setup()
{
#ifdef SERIAL_DEBUG
    Serial.begin(serial_monitor_baud);
#endif

    Serial0.begin(bt_serial_baud);
    // Serial0.setTxBufferSize(120);
    // Serial0.setRxBufferSize(120);
    while (!Serial0)
    {
    }
    TeensySerial.setStream(&Serial0);
    TeensySerial.setPacketHandler(&onTeensyReceived);

    // Create the BLE Device
    BLEDevice::init("ESP32");

    // Create the BLE Server
    pServer = BLEDevice::createServer();
    pServer->setCallbacks(new MyServerCallbacks());

    // Create the BLE Service
    BLEService *pService = pServer->createService(SERVICE_UUID);

    // Create a BLE Characteristic
    pCharacteristic_1 = pService->createCharacteristic(
        CHARACTERISTIC_UUID_1,
        BLECharacteristic::PROPERTY_NOTIFY);

    pCharacteristic_2 = pService->createCharacteristic(
        CHARACTERISTIC_UUID_2,
        BLECharacteristic::PROPERTY_READ |
            BLECharacteristic::PROPERTY_WRITE |
            BLECharacteristic::PROPERTY_NOTIFY);

    // Create a BLE Descriptor
    pDescr_1 = new BLEDescriptor((uint16_t)0x2901);
    pDescr_1->setValue("A very interesting variable");
    pCharacteristic_1->addDescriptor(pDescr_1);

    // Add the BLE2902 Descriptor because we are using "PROPERTY_NOTIFY"
    pBLE2902_1 = new BLE2902();
    pBLE2902_1->setNotifications(true);
    pCharacteristic_1->addDescriptor(pBLE2902_1);

    pBLE2902_2 = new BLE2902();
    pBLE2902_2->setNotifications(true);
    pCharacteristic_2->addDescriptor(pBLE2902_2);

    // add callback functions here:
    pCharacteristic_2->setCallbacks(new CharacteristicCallBack());

    // Start the service
    pService->start();

    // Start advertising
    BLEAdvertising *pAdvertising = BLEDevice::getAdvertising();
    pAdvertising->addServiceUUID(SERVICE_UUID);
    pAdvertising->setScanResponse(false);
    pAdvertising->setMinPreferred(0x0); // set value to 0x00 to not advertise this parameter
    BLEDevice::startAdvertising();
    Serial.println("Waiting a client connection to notify...");
}

void loop()
{
    TeensySerial.update();

    // The code below keeps the connection status uptodate:
    // Disconnecting
    if (!deviceConnected && oldDeviceConnected)
    {
        delay(500);                  // give the bluetooth stack the chance to get things ready
        pServer->startAdvertising(); // restart advertising
        // Serial.println("start advertising");
        oldDeviceConnected = deviceConnected;
    }
    // Connecting
    if (deviceConnected && !oldDeviceConnected)
    {
        // do stuff here on connecting
        oldDeviceConnected = deviceConnected;
    }

    tx_data.data.ball_pose.x = 10;
    tx_data.data.ball_pose.y = 30;

    if (millis() - last_write_time > 1000)
    {
        tx_data.data.robot_detected = false;
        TeensySerial.send(tx_data.bytes, sizeof(tx_data.bytes));
        // Serial.println("No data received from client");
    }
}
#endif