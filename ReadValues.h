//
// Created by Hayden Roszell on 12/1/22.
//

#ifndef DDD_READVALUES_H
#define DDD_READVALUES_H

#include "RealTimeTask.h"
#include "Constants.h"

#include <utility>
#include <vector>
#include <mutex>
#include "Serial.h"
#include "MPU6050.h"

using namespace std::chrono_literals;

class ReadValues : private MPU6050, public RealTimeTask {
    std::shared_ptr<std::vector<IMUValues>> adcvals;
    std::shared_ptr<std::mutex> mutex;
public:
    explicit ReadValues(std::shared_ptr<std::vector<IMUValues>> adcvals, std::shared_ptr<std::mutex> mutex) : RealTimeTask(10ms), adcvals(std::move(adcvals)), mutex(std::move(mutex)) {};
    void run() override;
};


#endif //DDD_READVALUES_H
