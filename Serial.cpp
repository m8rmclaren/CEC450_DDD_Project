//
// Created by Hayden Roszell on 12/1/22.
//

#include "Serial.h"
#include <linux/i2c-dev.h>
#include <i2c/smbus.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <unistd.h>
#include <stdexcept>
#include <iostream>
#include <cstring>

Serial::Serial(uint8_t adapter, uint8_t address) {
    char port[20];

    snprintf(port, 19, "/dev/i2c-%d", adapter);
    serial_port = open(port, O_RDWR);
    if (serial_port < 0)
        throw std::runtime_error("failed to open I2C port");

    // Configure I2C slave address as the value from constructor
    // ioctl automatically shaves MSB off for 7-bit addressing
    if (ioctl(serial_port, I2C_SLAVE, address) < 0)
        throw std::runtime_error("failed to configure I2C");
}

int Serial::write_serial_uc(uint8_t reg, const uint8_t *data, uint8_t length) const {
    struct i2c_smbus_ioctl_data ioctl_data{};
    union i2c_smbus_data smbus_data{};

    if (I2C_SMBUS_BLOCK_MAX < length) {
        std::cerr << "Requested length greater than SMBUS block length (" << length << "/" << I2C_SMBUS_BLOCK_MAX << ")" << std::endl;
        return -1;
    }

    // Byte 0 of smbus block is the length of transmission
    smbus_data.block[0] = length;

    // The rest is the data
    for (int i = 0; i < length; i++) {
        smbus_data.block[i + 1] = data[i];
    }

    ioctl_data.read_write = I2C_SMBUS_WRITE;
    ioctl_data.command = reg;
    ioctl_data.size = I2C_SMBUS_I2C_BLOCK_DATA;
    ioctl_data.data = &smbus_data;

    // There is a way to use the i2c_smbus API instead of issuing ioctl system call, but I couldn't get it to work
    if (ioctl(serial_port, I2C_SMBUS, &ioctl_data) < 0) {
        std::cerr << "Failed to access I2C write. Message: " << strerror(errno) << " [Errno " << errno << "]" << std::endl;
    }

    return 0;
}

int Serial::read_serial_uc(uint8_t reg, uint8_t * data, uint8_t length) const {
    struct i2c_smbus_ioctl_data ioctl_data{};
    union i2c_smbus_data smbus_data{};

    if (I2C_SMBUS_BLOCK_MAX < length) {
        std::cerr << "Requested length greater than SMBUS block length (" << length << "/" << I2C_SMBUS_BLOCK_MAX << ")" << std::endl;
        return -1;
    }

    // Byte 0 of smbus block is the length of transmission
    smbus_data.block[0] = length;

    ioctl_data.read_write = I2C_SMBUS_READ;
    ioctl_data.command = reg;
    ioctl_data.size = I2C_SMBUS_I2C_BLOCK_DATA;
    ioctl_data.data = &smbus_data;

    // There is a way to use the i2c_smbus API instead of issuing ioctl system call, but I couldn't get it to work
    if (ioctl(serial_port, I2C_SMBUS, &ioctl_data) < 0) {
        std::cerr << "Failed to access I2C read. Message: " << strerror(errno) << " [Errno " << errno << "]" << std::endl;
        return -1;
    }

    for(int i = 0; i < length; i++) {
        // Skip the first byte, which is the length of the rest of the block.
        data[i] = smbus_data.block[i+1];
    }

    return 0;
}