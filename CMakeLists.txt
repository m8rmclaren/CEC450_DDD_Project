cmake_minimum_required(VERSION 3.18)

set(CMAKE_CXX_STANDARD 14)

project(DDD)

find_package(Eigen3 REQUIRED)
find_package(Glog REQUIRED)

set(
        src
        main.cpp
        RealTimeTask.cpp
        Serial.cpp
        ReadValues.cpp
        CompareValues.cpp
        MPU6050.cpp
)

set(
        include_directories
        ${CMAKE_CURRENT_SOURCE_DIR}
        CppGPIO/src
)

add_executable(DDD ${src})

target_include_directories(
        DDD
        PUBLIC
        ${estts_include_directories}
        ${EIGEN3_INCLUDE_DIR}
        ${GLOG_INCLUDE_DIRS}
)

target_link_libraries(DDD pigpio)
target_link_libraries(DDD pthread)
target_link_libraries(DDD gpiodcxx)