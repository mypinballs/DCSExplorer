#include <chrono>
#include <thread>
#include "HiResTimer.h"

HiResTimer::HiResTimer() {
    // Calculate tick time in seconds and microseconds per tick

    tickTime_sec = 1.0 / std::chrono::high_resolution_clock::period::den;
    tickTime_us = 1.0e6 / std::chrono::high_resolution_clock::period::den;
}

double HiResTimer::GetTime_seconds() {
    auto now = std::chrono::high_resolution_clock::now();
    auto elapsed_time = std::chrono::duration_cast<std::chrono::duration<double>>(now - start_time);
    return elapsed_time.count();
}

void HiResTimer::SleepFor(double intervalInSeconds) {
    double target = GetTime_seconds() + intervalInSeconds;
    SleepUntil(target);
}

void HiResTimer::SleepUntil(double target) {
    while (GetTime_seconds() < target) {
        std::this_thread::sleep_for(std::chrono::microseconds(10));
    }
}
