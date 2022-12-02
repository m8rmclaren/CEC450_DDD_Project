//
// Created by Hayden Roszell on 12/1/22.
//

#include "ReadValues.h"

#include <utility>

ReadValues::ReadValues(std::shared_ptr<std::vector<ADCValues>> adcvals) { // : RealTimeTask(10ms)
    this->adcvals = std::move(adcvals);
}

void ReadValues::run() {
    ADCValues adc;
    get_6_axis_motion(&adc);
    int16_t x,y,z;
    get_acceleration(&x,&y,&z);
    printf("Accel - x: %d y: %d z: %d\n", x,y,z);

    //adcvals->push_back(temp);
}