//
// Created by Hayden Roszell on 12/1/22.
//

#include "ReadValues.h"

#include <utility>

#define NAMEOF(name) #name

ReadValues::ReadValues(std::shared_ptr<std::vector<ADCValues>> adcvals) { // : RealTimeTask(10ms)
    this->adcvals = std::move(adcvals);
    // I2C adapter 1 == rpi GPIO I2C port
    // Run i2cdetect -l for more info
    // 0x68 is the address of Alice's IMU
    serial = new Serial(1, 0x68);

    wakey_wakey();
}

void ReadValues::wakey_wakey() {
    // Select PWR_MGMT_1 register and write a 0 to it
    uint8_t wake_buf[1];
    wake_buf[0] = 0;
    if (0 != serial->write_serial_uc(0x6B, wake_buf, 1))
        throw std::runtime_error("failed to wake up IMU");
}

void ReadValues::run() {
    auto imu_buf = new uint8_t[14];
    if (0 != serial->read_serial_uc(0x3B, imu_buf, 14))
        return;
    std::cout << "Raw: ";
    for (int i = 0; i < 14; i++)
        printf("%02X", imu_buf[i]);
    std::cout << std::endl;

    int64_t validate = 0;
    for (int i = 0; i < 14; i++) {
        validate += imu_buf[i];
    }
    if (!validate)
        return wakey_wakey();

    ADCValues temp;
    temp.accelX = (int16_t)(imu_buf[0] << 8 | imu_buf[1]);
    temp.accelY = (int16_t)(imu_buf[2] << 8 | imu_buf[3]);
    temp.accelZ = (int16_t)(imu_buf[4] << 8 | imu_buf[5]);
    temp.temp = (imu_buf[6] << 8 | imu_buf[7]) == 0 ? 0.0 : (imu_buf[6] << 8 | imu_buf[7])/340.00+36.53;
    temp.gyroX = (int16_t)(imu_buf[8] << 8 | imu_buf[9]);
    temp.gyroY = (int16_t)(imu_buf[10] << 8 | imu_buf[11]);
    temp.gyroZ = (int16_t)(imu_buf[12] << 8 | imu_buf[13]);

    std::cout << NAMEOF(temp.accelX) << ": " << temp.accelX << std::endl;
    std::cout << NAMEOF(temp.accelY) << ": " << temp.accelY << std::endl;
    std::cout << NAMEOF(temp.accelZ) << ": " << temp.accelZ << std::endl;
    std::cout << NAMEOF(temp.temp) << ": " << temp.temp << std::endl;
    std::cout << NAMEOF(temp.gyroX) << ": " << temp.gyroX << std::endl;
    std::cout << NAMEOF(temp.gyroY) << ": " << temp.gyroY << std::endl;
    std::cout << NAMEOF(temp.gyroZ) << ": " << temp.gyroZ << std::endl;

    std::cout << std::endl;

    free(imu_buf);
    //adcvals->push_back(temp);
}
