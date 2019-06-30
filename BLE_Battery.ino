#include <BLEDevice.h>
#include <BLEUtils.h>
#include <BLEServer.h>
#include <BLE2902.h> 
#include <Arduino.h>

 

bool _BLEClientConnected = false;

#define BatteryService BLEUUID((uint16_t)0x180F) 
BLECharacteristic BatteryLevelCharacteristic(BLEUUID((uint16_t)0x2A19), BLECharacteristic::PROPERTY_READ | BLECharacteristic::PROPERTY_NOTIFY);
BLEDescriptor BatteryLevelDescriptor(BLEUUID((uint16_t)0x2901));

#define EnvironmentalService BLEUUID((uint16_t)0x181A)
BLECharacteristic TemperatureCharacteristic(BLEUUID((uint16_t)0x2A6E), BLECharacteristic::PROPERTY_READ);
BLECharacteristic HumidityCharacteristic(BLEUUID((uint16_t)0x2A6F), BLECharacteristic::PROPERTY_READ);

class MyServerCallbacks : public BLEServerCallbacks {
    void onConnect(BLEServer* pServer) {
      _BLEClientConnected = true;
    };

    void onDisconnect(BLEServer* pServer) {
      _BLEClientConnected = false;
    }
};

void InitBLE() {
  BLEDevice::init("BLE Battery");
  // Create the BLE Server
  BLEServer *pServer = BLEDevice::createServer();
  pServer->setCallbacks(new MyServerCallbacks());

  // Create the BLE Service
  BLEService *pBattery = pServer->createService(BatteryService);
  BLEService *pEnviro = pServer->createService(EnvironmentalService);

  pBattery->addCharacteristic(&BatteryLevelCharacteristic);

  pEnviro->addCharacteristic(&TemperatureCharacteristic);

  pEnviro->addCharacteristic(&HumidityCharacteristic);

  pServer->getAdvertising()->addServiceUUID(BatteryService);
  pServer->getAdvertising()->addServiceUUID(EnvironmentalService);

  pBattery->start();
  pEnviro->start();
  // Start advertising
  pServer->getAdvertising()->start();
}

void setup() {
  Serial.begin(115200);
  Serial.println("Battery Level Indicator - BLE");
  InitBLE();
}

  uint8_t level = 57;
  double temp = -12.01;
  double hum = 40.03;
  
void loop() {

  BatteryLevelCharacteristic.setValue(&level, 1);
  BatteryLevelCharacteristic.notify();

  int16_t bigTemp = temp * 100;
  uint16_t bigHum = hum * 100;

  TemperatureCharacteristic.setValue((uint8_t*)&bigTemp, 2);
  HumidityCharacteristic.setValue((uint8_t*)&bigHum, 2);


  delay(5000);

  level++;
  Serial.println(int(level));

  if (int(level)==100)
  level=0;

}
