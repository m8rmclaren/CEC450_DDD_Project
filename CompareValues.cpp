//
// Created by Hayden Roszell on 12/1/22.
//

#include "CompareValues.h"

void CompareValues::run() {
    //TODO

    // Colton's Psuedocode for the logic flow

    // assuming that ReadValues has already populated the ADCValues struct
    uint16_t sum = 0;
    for (uint16_t n : ADCValues.vect_accelX)
        sum += n;
    avg_accelX = sum / ADCValues.vect_accelX.size();
}