//
// Created by Hayden Roszell on 12/1/22.
//

#ifndef DDD_COMPAREVALUES_H
#define DDD_COMPAREVALUES_H

#include <vector>
#include "RealTimeTask.h"
#include "Constants.h"

using namespace std::chrono_literals;

class CompareValues : public RealTimeTask {
    std::shared_ptr<std::vector<ADCValues>> adcvals;
    public:
        explicit CompareValues(std::shared_ptr<std::vector<ADCValues>> adcvals) : RealTimeTask(500ms), adcvals(std::move(adcvals)) {}
        void run() override;
};


#endif //DDD_COMPAREVALUES_H
