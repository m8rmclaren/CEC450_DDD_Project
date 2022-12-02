//
// Created by Hayden Roszell on 12/1/22.
//

#ifndef DDD_SERIAL_H
#define DDD_SERIAL_H

#include <cstdio>
#include <cstdint>

class Serial {
    int serial_port;

public:
    Serial(uint8_t adapter, uint8_t address);
    int write_serial_uc(uint8_t reg, const uint8_t *data, uint8_t length) const;
    int read_serial_uc(uint8_t reg, uint8_t * data, uint8_t size) const;
};


#endif //DDD_SERIAL_H
