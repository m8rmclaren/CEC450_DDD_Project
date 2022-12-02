project(DDD)
cmake_minimum_required(VERSION 3.18)

set(
        src
        main.cpp
        #RealTimeTask.cpp
        Serial.cpp
        ReadValues.cpp
        #CompareValues.cpp
)

set(
        include_directories
        ${CMAKE_CURRENT_SOURCE_DIR}
)

add_executable(DDD ${src})

target_include_directories(
        DDD
        PUBLIC
        ${estts_include_directories}
)