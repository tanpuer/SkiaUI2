#pragma once

#include "string"
#include <chrono>
#include "native_log.h"

class MeasureTime {

public:
    MeasureTime() = delete;

    MeasureTime(const std::string& name, int minvalue = 0)
            : _name(name), _start(std::chrono::high_resolution_clock::now()), minValue(minvalue) {}

    ~MeasureTime() {
        auto stop = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(stop - _start).count();
        if (duration > minValue) {
            ALOGD("MeasureTime %s: %lld ms\n", _name.c_str(), duration)
        }
    }

private:
    std::string _name;
    int minValue = -1;
    std::chrono::time_point<std::chrono::steady_clock> _start;

};
