//
// Created by Hayden Roszell on 12/1/22.
//

#ifndef DDD_CONSTANTS_H
#define DDD_CONSTANTS_H

#include <iostream>
#include <vector>

struct IMUValues {
    int16_t accelX;
    int16_t accelY;
    int16_t accelZ;
//    std::vector<uint16_t> vect_accelX; // ReadValues.cpp should put the accel values into this FIFO queue
//    std::vector<uint16_t> vect_accelY; // ReadValues.cpp should put the accel values into this FIFO queue
//    std::vector<uint16_t> vect_accelZ; // ReadValues.cpp should put the accel values into this FIFO queue
    int16_t gyroX;
    int16_t gyroY;
    int16_t gyroZ;
//    std::vector<uint16_t> vect_gyroX; // ReadValues.cpp should put the gyro values into this FIFO queue
//    std::vector<uint16_t> vect_gyroY; // ReadValues.cpp should put the gyro values into this FIFO queue
//    std::vector<uint16_t> vect_gyroZ; // ReadValues.cpp should put the gyro values into this FIFO queue
    double temp;
};

struct compValues {
    int16_t avg_accelX;
    int16_t avg_accelY;
    int16_t avg_accelZ;
    int16_t avg_gyroX;
    int16_t avg_gyroY;
    int16_t avg_gyroZ;
    int16_t avg_temp;
};

struct nominalModel {
    int16_t nom_accelX;
    int16_t nom_accelY;
    int16_t nom_accelZ;
    int16_t nom_gyroX;
    int16_t nom_gyroY;
    int16_t nom_gyroZ;
    int16_t nom_temp;
};

#endif //DDD_CONSTANTS_H
