// ARMA Gripper M5 Init

/*
  Software License Agreement (BSD-1-Clause)

  Copyright (c) 2024 ARMA Inc. All rights reserved.

  Redistribution and use in source and binary forms, with or without
  modification, are permitted provided that the following conditions
  are met:
  1. Redistributions of source code must retain the above copyright
     notice, this list of conditions and the following disclaimer.

  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
  AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
  IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
  DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDERS AND CONTRIBUTORS BE LIABLE
  FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
  DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
  SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
  CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
  OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
  OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

#include "arma_m5_init.h"

#include <M5Unified.h>

#ifdef ARDUINO_M5STACK_Core2

void arma_m5_init() {
    auto cfg = M5.config();
    M5.begin(cfg);  // initialize M5 device
    // disable BUS5V Output
    uint8_t r12 = M5.Power.Axp192.readRegister8(0x12);
    M5.Power.Axp192.writeRegister8(0x12, r12 & 0xBF);
    // M5.Power.setChargeCurrent(0);
    M5.Power.setBatteryCharge(false);
    // disable charging for sure
    uint8_t r33 = M5.Power.Axp192.readRegister8(0x33);
    M5.Power.Axp192.writeRegister8(0x33, r33 & 0x7F);
    // disable Battery monitoring
    M5.Power.Axp192.writeRegister8(0x32, 0b00000010);
    M5.Power.setLed(0);
}

#elif ARDUINO_M5STACK_CORES3

static constexpr const uint32_t i2c_freq = 100000;
static constexpr uint8_t i2c_address_aw9523 = 0x58;

void usbOtgDisable() {
    uint8_t reg = ~0b00100000 & M5.In_I2C.readRegister8(i2c_address_aw9523, 0x02, i2c_freq);
    M5.In_I2C.writeRegister8(i2c_address_aw9523, 0x02, reg, i2c_freq);
}

void bus5VOutDisable() {
    // disable boost
    uint8_t value = ~0b10000000 & M5.In_I2C.readRegister8(i2c_address_aw9523, 0x03, i2c_freq);
    M5.In_I2C.writeRegister8(i2c_address_aw9523, 0x03, value, i2c_freq);
    delay(250);  // wait to avoid reverse current (VBUS -> BUS_OUT)
    // disable BUS 5V Output
    uint8_t reg = ~0b00000010 & M5.In_I2C.readRegister8(i2c_address_aw9523, 0x02, i2c_freq);
    M5.In_I2C.writeRegister8(i2c_address_aw9523, 0x02, reg, i2c_freq);
}

void arma_m5_init() {
    auto cfg = M5.config();
    M5.begin(cfg);  // initialize M5 device

    // POWER_MODE_USB_IN_BUS_IN
    usbOtgDisable();
    bus5VOutDisable();
    M5.Power.setBatteryCharge(false);
    M5.Power.setLed(0);
}

#endif
