//
// Created by Hayden Roszell on 12/1/22.
//

#ifndef DDD_COMPAREVALUES_H
#define DDD_COMPAREVALUES_H

#include <utility>
#include <vector>
#include "RealTimeTask.h"
#include "Constants.h"
#include <gpiod.hpp>
#include <mutex>

using namespace std::chrono_literals;

class CompareValues : public RealTimeTask {
    std::shared_ptr<std::vector<IMUValues>> adcvals;
    std::shared_ptr<std::mutex> mutex;
    IMUValues last = {0, 0, 0, 0, 0, 0, 0};


    int16_t YELLOW = 15000;
    int16_t RED = 26000;

    gpiod::chip gpio {"pinctrl-bcm2711", gpiod::chip::OPEN_BY_LABEL};
    gpiod::line red_pin = gpio.get_line(14);
    gpiod::line yellow_pin = gpio.get_line(15);
    gpiod::line green_pin = gpio.get_line(18);


public:
        explicit CompareValues(std::shared_ptr<std::vector<IMUValues>> adcvals, std::shared_ptr<std::mutex> mutex) : RealTimeTask(50ms), adcvals(std::move(adcvals)), mutex(std::move(mutex)) {
            red_pin.request({"LED", gpiod::line_request::DIRECTION_OUTPUT});
            yellow_pin.request({"LED", gpiod::line_request::DIRECTION_OUTPUT});
            green_pin.request({"LED", gpiod::line_request::DIRECTION_OUTPUT});
        }
        void run() override;
};


#endif //DDD_COMPAREVALUES_H
