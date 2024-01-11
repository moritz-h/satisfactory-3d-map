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
#include <numeric>
#include <sstream>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>

#include <spdlog/spdlog.h>

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
        names_set_.clear();
        start_.clear();
        end_.clear();
    }

    void start(const std::string& name) {
        if (names_set_.insert(name).second) {
            names_.push_back(name);
        }
        start_[name].push_back(clock_type::now());
    }

    void end(const std::string& name) {
        end_[name].push_back(clock_type::now());
    }

    std::string toString() {
        std::stringstream s;
        auto strLen = std::max_element(names_.begin(), names_.end(), [](const auto& a, const auto& b) {
            return a.size() < b.size();
        })->size();
        std::string sep(strLen + 3 + 8 + 3, '=');
        s << sep << std::endl;
        for (const auto& name : names_) {
            const auto& start = start_.at(name);
            const auto& end = end_.at(name);
            if (start.size() != end.size() || start.empty()) {
                throw std::runtime_error("Invalid time measure!");
            }
            std::vector<int64_t> durations(start.size());
            for (std::size_t i = 0; i < start.size(); i++) {
                durations[i] = std::chrono::duration_cast<std::chrono::microseconds>(end[i] - start[i]).count();
            }
            const auto sum = std::reduce(durations.begin(), durations.end());
            s << std::left << std::setw(strLen) << name << " : " << std::right << std::setw(8) << sum << " us";
            if (start.size() > 1) {
                s << " (sum of " << start.size() << " runs)";
            }
            s << std::endl;
        }
        s << sep << std::endl;
        return s.str();
    }

    void print() {
        spdlog::info("Time Measure:\n{}", toString());
    }

private:
    typedef std::chrono::high_resolution_clock clock_type;

    TimeMeasure() {
        clear();
    }

    static inline std::unique_ptr<TimeMeasure> instance_ = nullptr;
    std::unordered_map<std::string, std::vector<clock_type::time_point>> start_;
    std::unordered_map<std::string, std::vector<clock_type::time_point>> end_;
    std::vector<std::string> names_;
    std::unordered_set<std::string> names_set_;
};

#endif
