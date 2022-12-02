//
// Created by Hayden Roszell on 12/1/22.
//

#ifndef DDD_READVALUES_H
#define DDD_READVALUES_H

#include "RealTimeTask.h"
#include "Constants.h"

#include <utility>
#include <vector>
#include "Serial.h"

using namespace std::chrono_literals;

class ReadValues  { // : public RealTimeTask
    std::shared_ptr<std::vector<ADCValues>> adcvals;
    Serial * serial;
    void wakey_wakey();
public:
    explicit ReadValues(std::shared_ptr<std::vector<ADCValues>> adcvals);
    void run(); // override
};


#endif //DDD_READVALUES_H
