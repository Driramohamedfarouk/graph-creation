// Copyright (c) 2015, The Regents of the University of California (Regents)
// This Timer class is taking from GAP benchmark suite

#ifndef GRAPH_PROCESSING_FRAMEWORK_TIMER_H
#define GRAPH_PROCESSING_FRAMEWORK_TIMER_H

#include <chrono>

class Timer {

public:
    Timer() {}

    void Start() {
        elapsed_time_ = start_time_ = std::chrono::high_resolution_clock::now();
    }

    void Stop() {
        elapsed_time_ = std::chrono::high_resolution_clock::now();
    }

    double Seconds() const {
        return std::chrono::duration_cast<std::chrono::duration<double>>(elapsed_time_ - start_time_).count();
    }

    double Millisecs() const {
        return std::chrono::duration_cast<std::chrono::duration<double, std::milli>>(elapsed_time_ - start_time_).count();
    }

    double Microsecs() const {
        return std::chrono::duration_cast<std::chrono::duration<double, std::micro>>(elapsed_time_ - start_time_).count();
    }

private:
    std::chrono::high_resolution_clock::time_point start_time_, elapsed_time_;
};

#endif //GRAPH_PROCESSING_FRAMEWORK_TIMER_H
