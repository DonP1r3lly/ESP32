# ESP32-S2
The code was written by DAVID MARTINEZ.  STABLE SOFTWARE, NO MORE UPDATES

The software provided for ESP32-S2 is designed to detect and classify Wi-Fi networks, being able to identify and categorize them based on the channel used and signal strength. It can assist in locating the position of a router and the signal strength at different points in your home, allowing you to verify the coverage. Please note that this software is still in development.

This software is designed for the ESP32-S2 and should be loaded using the Arduino program. 

The installation instructions are as follows:

1. Open the program and select the preferences option, then within the additional board manager, paste the following link to obtain the board drivers:
https://raw.githubusercontent.com/espressif/arduino-esp32/gh-pages/package_esp32_index.json

2. Download the .ino file from the repository into your Arduino application.

3.Once loaded, compile it, verify that there are no errors, and select the port where your board is connected. When connecting the board, press the right button for a few seconds.

4. After completing these steps, you can load the software onto the board.

5. Disconnect the board from the computer and reconnect it to the computer or a power source.

6. From your mobile phone, turn on the Wi-Fi and connect to the network named "ESP32_AP" with the password: password123. Once connected, open the mobile browser and enter the following address: 192.168.4.1. You will establish communication with the device, allowing you to perform network scans.

Enjoy.

[![esp32-s2-mini.jpg](https://i.postimg.cc/wxRQnwHS/esp32-s2-mini.jpg)](https://postimg.cc/ThGbDqK9)
