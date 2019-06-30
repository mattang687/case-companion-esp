### Case Companion (ESP32)

This is the ESP32 code for the Case Companion, an open-source instrument case monitor. The Flutter app for this device can be found [here](https://github.com/mattang687/case-companion-flutter.git). I am currently focusing my work on the Flutter app. For now, this code allows the ESP to expose a Battery service with a Battery Level characteristic, which is incremented by 1% every 5 seconds, and an Environmental Sensing service with Temperature and Humidity characteristics that are kept constant.

#### TODO
[ ] Read data from an SI7021 sensor and update the characteristics accordingly
[ ] Read battery level from a LiPo and update the characteristic accordingly
[ ] Store data in the NVS while disconnected, and transfer it to the app when reconnected