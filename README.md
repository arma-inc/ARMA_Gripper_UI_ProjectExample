# ARMA Gripper UI - Project Example

## Description

This project provides a user interface example for the ARMA Gripper, a tool designed to enhance the capabilities of collaborative robots. The interface facilitates communication between the ARMA Gripper's MCU (serving as a Modbus Server (Slave)) and the UI Module (acting as a Modbus Client (Master)). Developed with a focus on ease of use and flexibility, this example is intended as a starting point for custom user interface development for the ARMA Gripper.

This software runs exclusively on M5Core2 and M5CoreS3 devices and is built using the PlatformIO ecosystem ([platformio.org](https://platformio.org)). 

## Installation

To get started with this project, follow these steps:

1. Ensure you have PlatformIO installed. Visit [platformio.org/platformio-ide](https://platformio.org/platformio-ide) for installation instructions.
2. Clone this repository to your local machine.
3. Open the project in Microsoft's Visual Studio Code (with PlatformIO IDE extension installed).
4. Configure your environment for either M5Core2 or M5CoreS3, as per your hardware.
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
  <tr><th colspan="2">platformio/framework-arduinoespressif32</th></tr>
  <tr><td>Copyright</td><td>Espressif Systems</td></tr>
  <tr><td>License</td><td><a href="https://github.com/espressif/arduino-esp32/blob/master/LICENSE.md">LGPL-2.1 license</a></td></tr>
  <tr><td>Source</td><td><a href="https://registry.platformio.org/tools/platformio/framework-arduinoespressif32">https://registry.platformio.org/tools/platformio/framework-arduinoespressif32</a></td></tr>
  <tr><td>Repository</td><td><a href="https://github.com/espressif/arduino-esp32.git">https://github.com/espressif/arduino-esp32.git</a></td></tr>
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
  <tr><th colspan="2">ModbusMaster</th></tr>
  <tr><td>Copyright</td><td>Copyright:: 2009-2016 Doc Walker</td></tr>
  <tr><td>License</td><td><a href="https://github.com/4-20ma/ModbusMaster/blob/master/LICENSE">Apache-2.0 license</a></td></tr>
  <tr><td>Repository</td><td><a href="https://github.com/4-20ma/ModbusMaster">https://github.com/4-20ma/ModbusMaster</a></td></tr>
</table>
