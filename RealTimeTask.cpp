//
// Created by Hayden Roszell on 12/1/22.
//

#include "RealTimeTask.h"

#include <chrono>

[[noreturn]] void RealTimeTask::start() {
    while (true) {
        auto start = std::chrono::high_resolution_clock::now();
        run();
        auto end = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double, std::milli> elapsed = end-start;
        std::this_thread::sleep_for(cycleTime-elapsed);
    }
}