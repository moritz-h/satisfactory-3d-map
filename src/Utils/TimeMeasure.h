#pragma once

#ifndef MEASURE_TIME

#define TIME_MEASURE_CLEAR()
#define TIME_MEASURE_START(N)
#define TIME_MEASURE_END(N)
#define TIME_MEASURE_PRINT()

#else

#define TIME_MEASURE_CLEAR() TimeMeasure::getInstance().clear()
#define TIME_MEASURE_START(N) TimeMeasure::getInstance().start(N)
#define TIME_MEASURE_END(N) TimeMeasure::getInstance().end(N)
#define TIME_MEASURE_PRINT() TimeMeasure::getInstance().print()

#include <algorithm>
#include <chrono>
#include <iostream>
#include <memory>
#include <string>
#include <unordered_map>
#include <vector>

class TimeMeasure {
public:
    static TimeMeasure& getInstance() {
        if (instance_ == nullptr) {
            instance_ = std::unique_ptr<TimeMeasure>(new TimeMeasure());
        }
        return *instance_;
    }

    void clear() {
        names_.clear();
        start_.clear();
        end_.clear();
    }

    void start(const std::string& name) {
        names_.push_back(name);
        start_[name] = clock_type::now();
    }

    void end(const std::string& name) {
        end_[name] = clock_type::now();
    }

    void print() {
        auto strLen = std::max_element(names_.begin(), names_.end(), [](const auto& a, const auto& b) {
            return a.size() < b.size();
        })->size();
        std::string sep(strLen + 3 + 8 + 3, '=');
        std::cout << sep << std::endl;
        for (const auto& name : names_) {
            std::cout << std::left << std::setw(strLen) << name << " : " << std::right << std::setw(8)
                      << std::chrono::duration_cast<std::chrono::microseconds>(end_.at(name) - start_.at(name)).count()
                      << " ms" << std::endl;
        }
        std::cout << sep << std::endl;
    }

private:
    typedef std::chrono::high_resolution_clock clock_type;

    TimeMeasure() {
        clear();
    }

    static inline std::unique_ptr<TimeMeasure> instance_ = nullptr;
    std::unordered_map<std::string, clock_type::time_point> start_;
    std::unordered_map<std::string, clock_type::time_point> end_;
    std::vector<std::string> names_;
};

#endif
