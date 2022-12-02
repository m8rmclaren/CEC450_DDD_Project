//
// Created by Hayden Roszell on 12/1/22.
//

#include <chrono>
#include <thread>
#include <iostream>
#include "CompareValues.h"
#include "ReadValues.h"
#include "Gpio.h"

using namespace std::this_thread; // sleep_for, sleep_until
using namespace std::chrono; // nanoseconds, system_clock, seconds

int main() {
    std::cout << "Hello" << std::endl;

    Gpio(1);

    std::shared_ptr<std::vector<ADCValues>> adcvals;
    ReadValues readTask = ReadValues(adcvals);

    for (;;) {
        auto start = std::chrono::high_resolution_clock::now();
        readTask.run();
        std::cout << duration_cast<std::chrono::microseconds>(high_resolution_clock::now() - start).count() << "us" << std::endl<< std::endl;
        //sleep_until(system_clock::now() + seconds (1));
    }



//    CompareValues compareTask = CompareValues(adcvals);
//
//    readTask.join();
//    compareTask.join();

    std::cout << "Goodbye" << std::endl;

    return 0;
}