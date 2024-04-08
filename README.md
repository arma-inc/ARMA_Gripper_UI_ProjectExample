# ARMA Gripper UI - ESP-IDF Project Example

## Description

This project provides a user interface example for the ARMA Gripper, a tool designed to enhance the capabilities of collaborative robots. The interface facilitates communication between the ARMA Gripper's MCU (serving as a Modbus Server (Slave)) and the UI Module (acting as a Modbus Client (Master)). Developed with a focus on ease of use and flexibility, this example is intended as a starting point for custom user interface development for the ARMA Gripper.

This software runs exclusively on M5Core2 and M5CoreS3 devices and is built using the ESP-IDF. 

## Installation

To get started with this project, follow these steps:

1. Clone this repository to your local machine.
2. Run ./init_components.sh to install required components
    - M5Unified and M5GFX from [m5stack](https://github.com/m5stack)
    - esp-modbus from [espressif](https://github.com/espressif/esp-modbus)
3. Use ESP-IDF: SDK Configuration editor (Menuconfig) to configure your environment (ARMA UI Configuration) for either M5Core2 or M5CoreS3, as per your hardware.
4. Select a correct target device esp32 (for M5Core2) or esp32s3 (for M5CoreS3)
5. Build and upload the project to your device.

## Usage

After successful installation, the UI module will communicate with the ARMA Gripper MCU via Modbus-RTU. You can customize the interface as needed for your application.

For comprehensive details and in-depth information about the ARMA Gripper Control, please refer to section "2.2 CONTROL BY MODBUS-RTU" of the user's manual.

## Support Disclaimer

Please be advised that no support services are offered for this project. The code is provided on an "AS-IS" basis without warranties or guarantees of any kind. We encourage users to consult the license file associated with this project for a full understanding of the terms and conditions under which the software is made available.


## Library Dependencies

The project utilizes the following libraries, essential for its functionality.

<table>
  <tr><th colspan="2"></th></tr>
  <tr><th colspan="2">ESP-IDF</th></tr>
  <tr><td>Copyright</td><td>Espressif Systems</td></tr>
  <tr><td>License</td><td><a href="https://github.com/espressif/esp-idf/blob/master/LICENSE">Apache-2.0</a></td></tr>
  <tr><td>Repository</td><td><a href="https://github.com/espressif/esp-idf.git">https://github.com/espressif/esp-idf.git</a></td></tr>
  <tr><th colspan="2"></th></tr>
  <tr><th colspan="2">M5Unified</th></tr>
  <tr><td>Copyright</td><td>Copyright (c) 2021 M5Stack</td></tr>
  <tr><td>License</td><td><a href="https://github.com/m5stack/M5Unified/blob/master/LICENSE">MIT License</a></td></tr>
  <tr><td>Repository</td><td><a href="https://github.com/m5stack/M5Unified">https://github.com/m5stack/M5Unified</a></td></tr>
  <tr><th colspan="2"></th></tr>
  <tr><th colspan="2">M5GFX</th></tr>
  <tr><td>Copyright</td><td>Copyright (c) 2021 M5Stack</td></tr>
  <tr><td>License</td><td><a href="https://github.com/m5stack/M5GFX/blob/master/LICENSE">MIT License</a></td></tr>
  <tr><td>Repository</td><td><a href="https://github.com/m5stack/M5GFX">https://github.com/m5stack/M5GFX</a></td></tr>
  <tr><th colspan="2"></th></tr>
  <tr><th colspan="2">ESP-Modbus</th></tr>
  <tr><td>Copyright</td><td>Copyright (c) 2016-2022 Espressif Systems (Shanghai) Co. Ltd.</td></tr>
  <tr><td>License</td><td><a href="https://github.com/espressif/esp-modbus/blob/master/LICENSE">Apache-2.0 license</a><br>
ESP-Modbus project is based on <a href="https://github.com/cwalter-at/freemodbus">FreeMODBUS library</a>, Copyright (c) 2006 Christian Walter and licensed under the BSD 3-clause license.

Modbus Master related code is Copyright (c) 2013 Armink and licensed under BSD 3-clause license.
</td></tr>
  <tr><td>Repository</td><td><a href="https://github.com/espressif/esp-modbus">https://github.com/espressif/esp-modbus</a></td></tr>
</table>
