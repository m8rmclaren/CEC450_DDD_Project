//
// Created by Hayden Roszell on 12/1/22.
//

#ifndef DDD_REALTIMETASK_H
#define DDD_REALTIMETASK_H

#include <iostream>
#include <thread>
#include <chrono>
#include <utility>

class RealTimeTask {
    private:
        std::thread thread;
        [[noreturn]] void threadrunner();
        std::string parent_thread_name;
    public:
        std::chrono::duration<double, std::milli> cycleTime;
        explicit RealTimeTask(std::chrono::duration<double, std::milli> cycleTime) : cycleTime(cycleTime) {};
        virtual void run() = 0;
        void start() {
            thread = std::thread(&RealTimeTask::threadrunner, this);
        }
        void join() {
            thread.join();
        }

        void set_parent_thread_name(std::string name) {
            parent_thread_name = std::move(name);
        }
};

#endif //DDD_REALTIMETASK_H
