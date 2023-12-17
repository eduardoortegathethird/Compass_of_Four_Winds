// Author | Eduardo Ortega
#include "BLEDevice.h"

// From | https://www.uuidgenerator.net/
// RED SERVER
static BLEUUID SERVICE_UUID_RED  ("4fafc201-1fb5-459e-8fcc-c5c9c331914b");
#define bleServerName_RED "ESP32_RED"
// YELLOW SERVER
// BLUE SERVER
//WHITE SERVER

//FLAGS for if we should begin connecting, and if connection is up already
static boolean doConnect = false;
static boolean connected = false;
static BLEAddress *pServerAddress;
int num_connect = 1;
bool seekingServer;

bool connectToServer(BLEAddress pAddress) {
  BLEClient* pClient = BLEDevice::createClient(); 
  pClient->connect(pAddress);
  Serial.println(" - Connected to server");
  BLERemoteService* pRemoteService = pClient->getService(SERVICE_UUID_RED);
  pClient->disconnect();
  if (pRemoteService == nullptr) {
    Serial.print("Failed to find our service UUID: ");
    Serial.println(SERVICE_UUID_RED.toString().c_str());
    return false;
  }
  return true;
}

// Callback from advertisements
class MyAdvertisedDeviceCallbacks: public BLEAdvertisedDeviceCallbacks {
  void onResult(BLEAdvertisedDevice advertisedDevice) {
    if (advertisedDevice.getName() == bleServerName_RED) {
        advertisedDevice.getScan()->stop();
        pServerAddress = new BLEAddress(advertisedDevice.getAddress());
        doConnect = true;
        Serial.println("Device found. Connecting!");
    }
  }
};

void setup() {
  Serial.begin(115200);
}

void loop() {
  BLEDevice::init("");
  BLEScan* pBLEScan = BLEDevice::getScan();
  pBLEScan->setAdvertisedDeviceCallbacks(new MyAdvertisedDeviceCallbacks());
  pBLEScan->setActiveScan(true);
  pBLEScan->start(30);
  seekingServer = true;
  Serial.println("Looking for Server to connect to...");
  while (seekingServer){
    if (doConnect == true) {
      if (connectToServer(*pServerAddress)) {
        Serial.println("We are now connected to the BLE Server.");
        connected = true;
      } else {
        Serial.println("We have failed to connect to the server; Restart your device to scan for nearby BLE server again.");
      }
      doConnect = false;
    }
    if (connected == true){
      Serial.println("Connected to the Server, spin up new client!!!");
      Serial.println(num_connect);
      num_connect += 1;
      } else {
        Serial.println("Could not connect to the Server, making new client");
    }
    //pBLEScan->clearResults();
    //delete pServerAddress;
    //BLEDevice::deinit(false);
    seekingServer = false;
    connected == false;
    delay(200);
  }
}
