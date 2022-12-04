//
// Created by Hayden Roszell on 12/1/22.
//

#include <chrono>
#include <thread>
#include <cmath>
#include "MPU6050.h"

using namespace std::this_thread; // sleep_for, sleep_until
using namespace std::chrono; // nanoseconds, system_clock, seconds

#define NAMEOF(name) #name

void print(const IMUValues& adc);
int16_t map(int16_t x, int16_t in_min, int16_t in_max, int16_t out_min, int16_t out_max);

// I2C adapter 1 == rpi GPIO I2C port
// Run i2cdetect -l for more info
// 0x68 is the address of Alice's IMU
MPU6050::MPU6050() : Serial(1, 0x68) {
    if (initialize() != 0)
        throw std::runtime_error("Failed to initialize IMU");

    if (!test_imu_connection())
        throw std::runtime_error("IMU didn't send back expected ID");

    //calibrate_gyro(6);
    //calibrate_accel(6);
}

int MPU6050::get_6_axis_motion(IMUValues * adc) {
    auto imu_buf = new uint8_t[14];
    if (0 != read_serial_uc(MPU6050_RA_ACCEL_XOUT_H, imu_buf, 14))
        return -1;

    int64_t validate = 0;
    for (int i = 0; i < 14; i++) {
        validate += imu_buf[i];
    }
    if (!validate) {
        initialize();
        if (0 != read_serial_uc(MPU6050_RA_ACCEL_XOUT_H, imu_buf, 14))
            return -1;
    }

    adc->accelX = (int16_t)(imu_buf[0] << 8 | imu_buf[1]);
    adc->accelY = (int16_t)(imu_buf[2] << 8 | imu_buf[3]);
    adc->accelZ = (int16_t)(imu_buf[4] << 8 | imu_buf[5]);
    adc->temp = (imu_buf[6] << 8 | imu_buf[7]) == 0 ? 0.0 : (imu_buf[6] << 8 | imu_buf[7])/340.00+36.53;
    adc->gyroX = (int16_t)(imu_buf[8] << 8 | imu_buf[9]);
    adc->gyroY = (int16_t)(imu_buf[10] << 8 | imu_buf[11]);
    adc->gyroZ = (int16_t)(imu_buf[12] << 8 | imu_buf[13]);

    free(imu_buf);
    return 0;
}

int MPU6050::initialize() {
    // Select PWR_MGMT_1 register and write a 0 to it
    uint8_t wake_buf[1];
    wake_buf[0] = 0;
    if (0 != write_serial_uc(0x6B, wake_buf, 1)) {
        std::cerr << "failed to wake up IMU" << std::endl;
        return -1;
    }

    return 0;
}

uint8_t MPU6050::get_device_id() {
    uint8_t who;
    read_serial_bits(MPU6050_RA_WHO_AM_I, MPU6050_WHO_AM_I_BIT, MPU6050_WHO_AM_I_LENGTH, &who);
    printf("IMU device ID: 0x%02X\n", who);
    return who;
}

bool MPU6050::test_imu_connection() {
    return get_device_id() == 0x34;
}

int MPU6050::get_acceleration(int16_t *x, int16_t *y, int16_t *z) {
    auto imu_buf = new uint8_t[14];
    if (0 != read_serial_uc(MPU6050_RA_ACCEL_XOUT_H, imu_buf, 6))
        return -1;

    int64_t validate = 0;
    for (int i = 0; i < 14; i++) {
        validate += imu_buf[i];
    }
    if (!validate) {
        initialize();
        if (0 != read_serial_uc(MPU6050_RA_ACCEL_XOUT_H, imu_buf, 6))
            return -1;
    }

    *x = (int16_t)(((imu_buf[0]) << 8) | imu_buf[1]);
    *y = (int16_t)(((imu_buf[2]) << 8) | imu_buf[3]);
    *z = (int16_t)(((imu_buf[4]) << 8) | imu_buf[5]);
    return 0;
}

void MPU6050::pid(uint8_t ReadAddress, float kP, float kI, uint8_t Loops) {
    uint8_t SaveAddress = (ReadAddress == 0x3B)?((get_device_id() < 0x38 )? 0x06:0x77):0x13;

    int16_t  Data;
    float Reading;
    int16_t BitZero[3];
    uint8_t shift =(SaveAddress == 0x77)?3:2;
    float Error, PTerm, ITerm[3];
    int16_t eSample;
    uint32_t eSum;
    uint16_t gravity = 8192; // prevent uninitialized compiler warning
    if (ReadAddress == 0x3B) gravity = 16384 >> get_full_scale_accel_range();
    printf(">");
    for (int i = 0; i < 3; i++) {
        read_serial_uc(SaveAddress + (i * shift), (uint8_t *)&Data, 2);
        Reading = Data;
        if(SaveAddress != 0x13){
            BitZero[i] = Data & 1;										 // Capture Bit Zero to properly handle Accelerometer calibration
            ITerm[i] = ((float)Reading) * 8;
        } else {
            ITerm[i] = Reading * 4;
        }
    }
    for (int L = 0; L < Loops; L++) {
        eSample = 0;
        for (int c = 0; c < 100; c++) {// 100 PI Calculations
            eSum = 0;
            for (int i = 0; i < 3; i++) {
                read_serial_uc(ReadAddress + (i * 2), (uint8_t *)&Data, 2);
                Reading = Data;
                if ((ReadAddress == 0x3B)&&(i == 2)) Reading -= gravity;	//remove Gravity
                Error = -Reading;
                eSum += abs(Reading);
                PTerm = kP * Error;
                ITerm[i] += (Error * 0.001) * kI;				// Integral term 1000 Calculations a second = 0.001
                if(SaveAddress != 0x13){
                    Data = round((PTerm + ITerm[i] ) / 8);		//Compute PID Output
                    Data = ((Data)&0xFFFE) |BitZero[i];			// Insert Bit0 Saved at beginning
                } else Data = round((PTerm + ITerm[i] ) / 4);	//Compute PID Output
                write_serial_uc(SaveAddress + (i * shift), (uint8_t *)&Data, 2);

            }
            if((c == 99) && eSum > 1000){						// Error is still to great to continue
                c = 0;
                printf("*");
            }
            if((eSum * ((ReadAddress == 0x3B)?.05: 1)) < 5) eSample++;	// Successfully found offsets prepare to  advance
            if((eSum < 100) && (c > 10) && (eSample >= 10)) break;		// Advance to next Loop
            sleep_until(system_clock::now() + microseconds (100));
        }
        printf(".");
        kP *= .75;
        kI *= .75;
        for (int i = 0; i < 3; i++){
            if(SaveAddress != 0x13) {
                Data = round((ITerm[i] ) / 8);		//Compute PID Output
                Data = ((Data)&0xFFFE) |BitZero[i];	// Insert Bit0 Saved at beginning
            } else Data = round((ITerm[i]) / 4);
            write_serial_uc(SaveAddress + (i * shift), (uint8_t *)&Data, 2);
        }
    }
    reset_fifo();
    reset_dmp();
}

uint8_t MPU6050::get_full_scale_accel_range() {
    uint8_t range;
    read_serial_bits(MPU6050_RA_ACCEL_CONFIG, MPU6050_ACONFIG_AFS_SEL_BIT, MPU6050_ACONFIG_AFS_SEL_LENGTH, &range);
    return range;
}

void MPU6050::calibrate_gyro(uint8_t Loops) {
    double kP = 0.3;
    double kI = 90;
    float x;
    x = (100 - map(Loops, 1, 5, 20, 0)) * .01;
    kP *= x;
    kI *= x;

    pid( 0x43,  kP, kI,  Loops);
}

void MPU6050::calibrate_accel(uint8_t Loops) {
    float kP = 0.3;
    float kI = 20;
    float x;
    x = (100 - map(Loops, 1, 5, 20, 0)) * .01;
    kP *= x;
    kI *= x;
    pid( 0x3B, kP, kI,  Loops);
}

void MPU6050::reset_fifo() {
    write_serial_bit(MPU6050_RA_USER_CTRL, MPU6050_USERCTRL_FIFO_RESET_BIT, true);
}

void MPU6050::reset_dmp() {
    write_serial_bit(MPU6050_RA_USER_CTRL, MPU6050_USERCTRL_DMP_RESET_BIT, true);
}

int16_t map(int16_t x, int16_t in_min, int16_t in_max, int16_t out_min, int16_t out_max) {
    return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

void print(const IMUValues& adc) {
    std::cout << NAMEOF(adc.accelX) << ": " << adc.accelX << std::endl;
    std::cout << NAMEOF(adc.accelY) << ": " << adc.accelY << std::endl;
    std::cout << NAMEOF(adc.accelZ) << ": " << adc.accelZ << std::endl;
    std::cout << NAMEOF(adc.temp) << ": " << adc.temp << std::endl;
    std::cout << NAMEOF(adc.gyroX) << ": " << adc.gyroX << std::endl;
    std::cout << NAMEOF(adc.gyroY) << ": " << adc.gyroY << std::endl;
    std::cout << NAMEOF(adc.gyroZ) << ": " << adc.gyroZ << std::endl;
}
