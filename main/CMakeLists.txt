cmake_minimum_required(VERSION 3.16)

set(EXCLUDE_COMPONENTS freemodbus)

set(COMPONENT_REQUIRES M5Unified M5GFX esp-modbus)

FILE(GLOB_RECURSE app_sources ${CMAKE_SOURCE_DIR}/main/*.*)

idf_component_register(SRCS ${app_sources} INCLUDE_DIRS ".")
