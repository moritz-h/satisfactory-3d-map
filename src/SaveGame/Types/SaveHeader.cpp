#include "SaveHeader.h"

#include <algorithm>
#include <chrono>
#include <ctime>
#include <iostream>
#include <stdexcept>

#include "../Utils/StreamUtils.h"

SatisfactorySaveGame::SaveHeader::SaveHeader(std::istream& stream) {
    save_header_version_ = read<int32_t>(stream);
    save_version_ = read<int32_t>(stream);
    if (save_header_version_ != 6 || save_version_ != 25) {
        throw std::runtime_error("Unknown Save Version!");
    }
    build_version_ = read<int32_t>(stream);
    map_name_ = read_length_string(stream);
    map_options_ = read_length_string(stream);
    session_name_ = read_length_string(stream);
    play_duration_ = read<int32_t>(stream);
    save_date_time_ = read<int64_t>(stream);
    session_visibility_ = read<int8_t>(stream);
}

void SatisfactorySaveGame::SaveHeader::print() const {
    // save_date_time is integer ticks since 0001-01-01 00:00, where 1 tick is 100 nano seconds.
    // See: https://docs.unrealengine.com/en-US/API/Runtime/Core/Misc/FDateTime/index.html
    // Satisfactory seems to use UTC.
    // Convert to unix timestamp:
    // Python: (datetime.datetime(1970, 1, 1) - datetime.datetime(1, 1, 1)).total_seconds()
    //   => 62135596800.0 seconds
    std::time_t save_date_time = (save_date_time_ - 621355968000000000) / 10000000;

    // To string
    std::string save_date_str(20, '\0');
    std::strftime(save_date_str.data(), save_date_str.size(), "%F %T", std::localtime(&save_date_time));
    save_date_str.erase(std::find(save_date_str.begin(), save_date_str.end(), '\0'), save_date_str.end());

    // Print
    std::cout << "Save Header Version:     " << save_header_version_ << std::endl;
    std::cout << "Save Version:            " << save_version_ << std::endl;
    std::cout << "Build Version:           " << build_version_ << std::endl;
    std::cout << "Map Name:                " << map_name_ << std::endl;
    std::cout << "Map Options:             " << map_options_ << std::endl;
    std::cout << "Session Name:            " << session_name_ << std::endl;
    std::cout << "Play Duration (seconds): " << play_duration_ << " (" << play_duration_ / 60.0 / 60.0 << " h)"
              << std::endl;
    std::cout << "Save Date Time (ticks):  " << save_date_time << " (" << save_date_str << ")" << std::endl;
    std::cout << "Session Visibility:      " << static_cast<bool>(session_visibility_) << std::endl;
}
