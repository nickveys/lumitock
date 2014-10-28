cmake_minimum_required(VERSION 3.0)

set(CMAKE_TOOLCHAIN_FILE ${CMAKE_SOURCE_DIR}/cmake/ArduinoToolchain.cmake)

project(lumitock)

set(ARDUINO_DEFAULT_BOARD uno)
set(ARDUINO_DEFAULT_PORT /dev/tty.usbmodemfa1341)

add_subdirectory(ds1337)
add_subdirectory(tlc5940)
add_subdirectory(main)
