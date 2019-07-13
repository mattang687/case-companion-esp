#include <BLEDevice.h>;
#include <BLEUtils.h>;
#include <BLEServer.h>;
#include <BLE2902.h> ;
#include <Wire.h>;
#include <Arduino.h>;

// define si7021 address
#define SI7021_ADDRESS 0x40

// define services, characteristics, and their properties
#define BatteryService BLEUUID((uint16_t)0x180F) 
BLECharacteristic BatteryLevelCharacteristic(BLEUUID((uint16_t)0x2A19), BLECharacteristic::PROPERTY_READ);

#define EnvironmentalService BLEUUID((uint16_t)0x181A)
BLECharacteristic TemperatureCharacteristic(BLEUUID((uint16_t)0x2A6E), BLECharacteristic::PROPERTY_READ);
BLECharacteristic HumidityCharacteristic(BLEUUID((uint16_t)0x2A6F), BLECharacteristic::PROPERTY_READ);

void InitBLE() {
  BLEDevice::init("Case Companion");
  // create the server
  BLEServer *pServer = BLEDevice::createServer();

  // add services
  BLEService *pBattery = pServer->createService(BatteryService);
  BLEService *pEnviro = pServer->createService(EnvironmentalService);

  // add characteristics
  pBattery->addCharacteristic(&BatteryLevelCharacteristic);
  pEnviro->addCharacteristic(&TemperatureCharacteristic);
  pEnviro->addCharacteristic(&HumidityCharacteristic);

  // add service uuids to advertisement
  pServer->getAdvertising()->addServiceUUID(BatteryService);
  pServer->getAdvertising()->addServiceUUID(EnvironmentalService);

  // start services
  pBattery->start();
  pEnviro->start();

  // start advertising
  pServer->getAdvertising()->start();
}

void setup() {
  // specify pins for i2c (depends on the board layout)
  Serial.begin(115200);
  Wire.begin(27, 33);
  InitBLE();
}

void loop()
{
  unsigned int data[2];
  
  Wire.beginTransmission(SI7021_ADDRESS);
  // send humidity measurement command
  Wire.write(0xF5);
  Wire.endTransmission();
  delay(50);
     
  // request 2 bytes
  Wire.requestFrom(SI7021_ADDRESS, 2);

  // wait until data is ready
  while(Wire.available() < 2);

  // read data
  data[0] = Wire.read();
  data[1] = Wire.read();
     
  // convert into humidity (conversion described in si7021 docs)
  float hum  = ((data[0] * 256.0) + data[1]);
  hum = ((125 * hum) / 65536.0) - 6;
 
  Wire.beginTransmission(SI7021_ADDRESS);
  // send temperature measurement command
  Wire.write(0xF3);
  Wire.endTransmission();
  delay(50);

  // request 2 bytes
  Wire.requestFrom(SI7021_ADDRESS, 2);

  // wait until data is ready
  while(Wire.available() < 2);

  // read data
  data[0] = Wire.read();
  data[1] = Wire.read();
 
  // convert into temperature (conversion described in si7021 docs)
  float temp  = ((data[0] * 256.0) + data[1]);
  temp = ((175.72 * temp) / 65536.0) - 46.85;

  // convert into int
  int16_t bigTemp = temp * 100;
  uint16_t bigHum = hum * 100;

  // write to BLE characteristics
  TemperatureCharacteristic.setValue((uint8_t*)&bigTemp, 2);
  HumidityCharacteristic.setValue((uint8_t*)&bigHum, 2);

  uint8_t dummyBat = 12;
  BatteryLevelCharacteristic.setValue(&dummyBat, 1);
  
  delay(1000);
}
