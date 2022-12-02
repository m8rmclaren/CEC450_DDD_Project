//
// Created by Hayden Roszell on 12/1/22.
//

#include <chrono>
#include <thread>
#include <iostream>
#include "CompareValues.h"
#include "ReadValues.h"
using namespace std::this_thread; // sleep_for, sleep_until
using namespace std::chrono; // nanoseconds, system_clock, seconds

int main() {
    std::cout << "Hello" << std::endl;



    std::shared_ptr<std::vector<ADCValues>> adcvals;
    ReadValues readTask = ReadValues(adcvals);

    for (;;) {
        readTask.run();
        sleep_until(system_clock::now() + seconds (1));
    }



//    CompareValues compareTask = CompareValues(adcvals);
//
//    readTask.join();
//    compareTask.join();

    std::cout << "Goodbye" << std::endl;

    return 0;
}