project(DS1337)

file(GLOB headers "${PROJECT_SOURCE_DIR}/src/*.cpp")
file(GLOB sources "${PROJECT_SOURCE_DIR}/include/*.h")

include_directories(${PROJECT_SOURCE_DIR}/include)

generate_arduino_library(ds1337
    HDRS ${headers}
    SRCS ${sources})
