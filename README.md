# Case Companion
An instrument monitoring system created with Flutter and Arduino

## Introduction
Bad weather can be pretty dangerous to musical instruments (my viola cracked in the winter a few years ago). Many solutions exist to combat unfriendly weather, like Dampits, humidifiers, covers, and expensive cases, but there's very little useful evidence of their effectiveness, since everyone's setup is so different. The Case Companion is my solution to this problem. It consists of this device and a [Flutter app](https://github.com/mattang687/case-companion.git), and it allows musicians to record and view the temperature and humidity in their case over time.

## Key Features
* Read sensor data over I2C from an SI7021 temperature and humidity sensor
* Send sensor data to a smartphone over Bluetooth Low Energy

## Technologies
* Arduino 1.8.9
    * Arduino ESP32 1.0.2
    * Bluetooth Low Energy 4.2
    * I2C 6.0

## Installation
* First, you'll need an ESP32, an SI7021, and a battery of some kind. If your ESP32 can't run off batteries on its own, you'll need some additional circuitry to do so.
* Connect the SI7021 to your ESP32. Since we're using I2C, you should only need 4 wires for 3V, GND, SDA, and SCL. My SI7021 already had pull-up resistors installed, so I didn't have to worry about them. If yours doesn't, two 4.7k resistors, one from SDA to 3V and one from SCL to 3V, should do the trick.
* Install the Arduino IDE and the ESP32 library (instructions found [here](https://github.com/espressif/arduino-esp32/blob/master/docs/arduino-ide/boards_manager.md))
* Then, clone this repo or copy my code over and upload it. You should now be able to scan for the device, connect to it, and see the temperature and humidity in the [app](https://github.com/mattang687/case-companion.git).
* For some additional power savings, you can turn the CPU frequency down to 80MHz under "tools" before uploading.

## Future Plans
* Add smart charging to accurately measure battery life and prevent over-charge/discharge
* Use a different SoC such as the nRF52840, as the ESP32 isn't nearly as power efficient as those specifically designed for BLE (though it is much cheaper).