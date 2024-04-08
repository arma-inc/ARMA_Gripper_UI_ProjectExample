# ARMA Gripper UI - Project Example

## Description

This project provides a user interface example for the ARMA Gripper, a tool designed to enhance the capabilities of collaborative robots. The interface facilitates communication between the ARMA Gripper's MCU (serving as a Modbus Server (Slave)) and the UI Module (acting as a Modbus Client (Master)). Developed with a focus on ease of use and flexibility, this example is intended as a starting point for custom user interface development for the ARMA Gripper.

The project includes two distinct implementations:

  1. **esp-idf** branch: Utilizes the native ESP-IDF framework for a more granular control and optimization of the ESP32's capabilities.
  2. **arduinoespressif32** branch: Leverages the simplicity and convenience of the arduinoespressif32 framework within the PlatformIO environment, ideal for rapid development and prototyping.

This software runs exclusively on M5Core2 and M5CoreS3 devices. 

## Installation

To get started with this project, follow these steps:

1. Clone this repository to your local machine.  
  `git clone https://github.com/arma-inc/ARMA_Gripper_UI_ProjectExample.git`  
  `cd ARMA_Gripper_UI_ProjectExample`
2. To access a specific implementation, switch to the relevant branch
    - ESP-IDF Version  
      `git checkout esp-idf`
    - PlatformIO + arduinoespressif32 Version  
      `git checkout arduinoespressif32`
3. Follow instructions in the README.md of the selected branch.

## Support Disclaimer

Please be advised that no support services are offered for this project. The code is provided on an "AS-IS" basis without warranties or guarantees of any kind. We encourage users to consult the license file associated with this project for a full understanding of the terms and conditions under which the software is made available.
