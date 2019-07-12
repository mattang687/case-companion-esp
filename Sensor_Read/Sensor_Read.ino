#include <Wire.h>;

// read and convert sensor data
void setup()
{
  Wire.begin(27, 33);
  Serial.begin(9600);
  Wire.beginTransmission(0x40);
  Wire.endTransmission();
  delay(300);
}
 
void loop()
{
  unsigned int data[2];
   
  Wire.beginTransmission(0x40);
  // send humidity measurement command
  Wire.write(0xF5);
  Wire.endTransmission();
  delay(500);
     
  // request 2 bytes
  Wire.requestFrom(si7021Addr, 2);
  // read 2 bytes
  if(Wire.available() == 2)
  {
    data[0] = Wire.read();
    data[1] = Wire.read();
  }
     
  // convert into humidity
  float humidity  = ((data[0] * 256.0) + data[1]);
  humidity = ((125 * humidity) / 65536.0) - 6;
 
  Wire.beginTransmission(0x40);
  // send temperature measurement command
  Wire.write(0xF3);
  Wire.endTransmission();
  delay(500);
     
  // request 2 bytes
  Wire.requestFrom(0x40, 2);
   
  // read 2 bytes
  if(Wire.available() == 2)
  {
    data[0] = Wire.read();
    data[1] = Wire.read();
  }
 
  // convert into temperature
  float temp  = ((data[0] * 256.0) + data[1]);
  float celsTemp = ((175.72 * temp) / 65536.0) - 46.85;
  float fahrTemp = celsTemp * 1.8 + 32;

  // print to serial monitor for now
  Serial.print("Humidity : ");
  Serial.print(humidity);
  Serial.println(" % RH");
  Serial.print("Celsius : ");
  Serial.print(celsTemp);
  Serial.println(" C");
  Serial.print("Fahrenheit : ");
  Serial.print(fahrTemp);
  Serial.println(" F");
  delay(1000);
}
