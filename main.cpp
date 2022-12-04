//
// Created by Hayden Roszell on 12/1/22.
//

#include <thread>
#include <iostream>
#include "CompareValues.h"
#include "ReadValues.h"
#include <gpiod.hpp>
#include <mutex>

using namespace std::this_thread; // sleep_for, sleep_until
using namespace std::chrono; // nanoseconds, system_clock, seconds

int main() {
    std::cout << "Hello" << std::endl;

    std::shared_ptr<std::vector<IMUValues>> adcvals = std::make_shared<std::vector<IMUValues>>();
    std::shared_ptr<std::mutex> mutex = std::make_shared<std::mutex>();
    adcvals->push_back({});
    adcvals->push_back({});
    adcvals->push_back({});
    adcvals->push_back({});
    ReadValues readTask = ReadValues(adcvals, mutex);
    CompareValues compareTask = CompareValues(adcvals, mutex);

    readTask.start();
    compareTask.start();


    readTask.join();
    compareTask.join();

    std::cout << "Goodbye" << std::endl;

    return 0;
}