#ifndef HIRES_TIMER_H
#define HIRES_TIMER_H

#include <chrono>

class HiResTimer {
private:
    std::chrono::high_resolution_clock::time_point start_time;
    double tickTime_sec;
    double tickTime_us;

public:
    HiResTimer();
    double GetTime_seconds();
    void SleepFor(double intervalInSeconds);
    void SleepUntil(double target);
};

#endif
