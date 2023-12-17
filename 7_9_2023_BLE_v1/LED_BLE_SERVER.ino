// Author | Eduardo Ortega
#include <BLEDevice.h>
#include <BLEServer.h>
#include <BLEUtils.h>
#include <BLE2902.h>

// From | https://www.uuidgenerator.net/
#define SERVICE_UUID  "4fafc201-1fb5-459e-8fcc-c5c9c331914b"
#define bleServerName "ESP32_RED"
int num_connections = 1;
bool deviceConnected = false;
bool waitForClient;

class MyServerCallbacks: public BLEServerCallbacks {
  void onConnect(BLEServer* pServer) {
    deviceConnected = true;
  };
  void onDisconnect(BLEServer* pServer){
    deviceConnected = false;
  }
};


void setup() {
  Serial.begin(115200);
}

void loop() {
  BLEDevice::init(bleServerName);
  BLEServer *pServer = BLEDevice::createServer();
  pServer->setCallbacks(new MyServerCallbacks());
  BLEService *ledService = pServer->createService(SERVICE_UUID);
  ledService->start();
  BLEAdvertising *pAdvertising = BLEDevice::getAdvertising();
  pAdvertising->addServiceUUID(SERVICE_UUID);
  pServer->getAdvertising()->start();
  waitForClient = true;
  Serial.println("waiting for client to connect, on standby...");
  while (waitForClient){
    if (deviceConnected){
      Serial.println("Client is CONNECTED, 180 second delay!!!!!");
      Serial.println(num_connections);
      num_connections += 1;
      delay(60 * 1000);
      Serial.println("Restarting Server");
      waitForClient = false;
    }
    delay(500);
  }
}
