//
// Created by Hayden Roszell on 12/1/22.
//

#include "ReadValues.h"



void ReadValues::run() {
    IMUValues adc{};
    get_6_axis_motion(&adc);
    int16_t x,y,z;
    //get_acceleration(&x,&y,&z);
    mutex->lock();
    adcvals->push_back(adc);
    mutex->unlock();
//    printf("Accel - x: %d y: %d z: %d\n", x,y,z);
}