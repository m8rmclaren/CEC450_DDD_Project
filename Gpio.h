//
// Created by Hayden Roszell on 12/1/22.
//

#ifndef DDD_GPIO_H
#define DDD_GPIO_H

#include <cstdint>

class Gpio {
    uint8_t gpio;
public:
    explicit Gpio(uint8_t gpio);
};


#endif //DDD_GPIO_H
