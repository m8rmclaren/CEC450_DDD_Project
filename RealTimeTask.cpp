//
// Created by Hayden Roszell on 12/1/22.
//

#include "RealTimeTask.h"

#include <chrono>

[[noreturn]] void RealTimeTask::threadrunner() {
    // Keep track of average jitter
    long long last_execution_period_ns = 0;
    long long avg_jitter = 0;
    while (true) {
        auto start = std::chrono::high_resolution_clock::now();
        run();
        auto end = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double, std::milli> elapsed = end-start;
        std::this_thread::sleep_for(cycleTime-elapsed);

        //std::cout << parent_thread_name << ": cycle time: " << std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock::now() - start).count() << "ms" << std::endl;

        // Output parent thread name and cycle time in CSV format
        //std::cout << parent_thread_name << "," << std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock::now() - start).count() << std::endl;

        auto current_execution_period = std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::high_resolution_clock::now() - start).count();
        avg_jitter = (avg_jitter + abs((int)(last_execution_period_ns - current_execution_period))) / 2;
        last_execution_period_ns = current_execution_period;

        // Print CSV containing parent thread name, cycle time, and average jitter
        std::cout << parent_thread_name << "," << std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock::now() - start).count() << "," << avg_jitter << std::endl;
    }
}