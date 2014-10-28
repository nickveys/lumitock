project(MAIN)

file(GLOB headers "${PROJECT_SOURCE_DIR}/src/*.cpp")
file(GLOB sources "${PROJECT_SOURCE_DIR}/include/*.h")

include_directories(${PROJECT_SOURCE_DIR}/include)
include_directories(${DS1337_SOURCE_DIR}/include)
include_directories(${TLC5940_SOURCE_DIR}/include)

generate_arduino_firmware(main
    HDRS ${headers}
    SRCS ${sources}
    LIBS ds1337 tlc5940)
