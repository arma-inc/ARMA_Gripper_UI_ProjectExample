#!/bin/bash
# Install components from m5stack

if [ -d './components' ]; then
    echo 'components already exists'
else
    echo 'creating component folder'
    mkdir ./components
fi
cd ./components

if [ -d './M5Unified' ]; then
    echo 'M5Unified already exists'
else
    echo 'creating M5Unified'
    git clone --depth 1 --branch "0.1.16" https://github.com/m5stack/M5Unified.git
fi

if [ -d './M5GFX' ]; then
    echo 'M5GFX already exists'
else
    echo 'creating M5GFX'
    git clone --depth 1 --branch "0.1.16" https://github.com/m5stack/M5GFX.git
fi

if [ -d './esp-modbus' ]; then
    echo 'esp-modbus already exists'
else
    echo 'creating esp-modbus'
    git clone --depth 1 --branch "v1.0.15" https://github.com/espressif/esp-modbus.git
fi

