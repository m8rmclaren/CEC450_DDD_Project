//
// Created by Hayden Roszell on 12/1/22.
//

#include <pigpio.h>
#include <stdexcept>
#include "Gpio.h"

Gpio::Gpio(uint8_t gpio) {
    this->gpio = gpio;

    if (gpioInitialise() < 0)
        throw std::runtime_error("failed to initialize gpio");
}
