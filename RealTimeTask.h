//
// Created by Hayden Roszell on 12/1/22.
//

#ifndef DDD_REALTIMETASK_H
#define DDD_REALTIMETASK_H

#include <iostream>
#include <thread>
#include <chrono>

class RealTimeTask : private std::thread {
public:
    std::chrono::duration<double, std::milli> cycleTime;
    explicit RealTimeTask(std::chrono::duration<double, std::milli> cycleTime) : cycleTime(cycleTime), std::thread(&RealTimeTask::start, this) {};
    virtual void run();

private:
    [[noreturn]] void start();

};

#endif //DDD_REALTIMETASK_H
