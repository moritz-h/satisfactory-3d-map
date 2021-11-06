#include "SaveHeader.h"

#include <algorithm>
#include <chrono>
#include <ctime>
#include <iostream>
#include <sstream>
#include <stdexcept>

#include "Utils/StreamUtils.h"

Satisfactory3DMap::SaveHeader::SaveHeader(std::istream& stream) {
    save_header_version_ = read<int32_t>(stream);
    save_version_ = read<int32_t>(stream);
    if (save_header_version_ != 8 || save_version_ != 25) {
        throw std::runtime_error("Unknown Save Version!");
    }
    build_version_ = read<int32_t>(stream);
    map_name_ = read_length_string(stream);
    map_options_ = read_length_string(stream);
    session_name_ = read_length_string(stream);
    play_duration_ = read<int32_t>(stream);
    save_date_time_ = read<int64_t>(stream);
    session_visibility_ = read<int8_t>(stream);
    editor_object_version_ = read<int32_t>(stream);
    mod_metadata_ = read_length_string(stream);
    is_modded_save_ = read<int32_t>(stream);
}

void Satisfactory3DMap::SaveHeader::serialize(std::ostream& stream) const {
    write(stream, save_header_version_);
    write(stream, save_version_);
    write(stream, build_version_);
    write_length_string(stream, map_name_);
    write_length_string(stream, map_options_);
    write_length_string(stream, session_name_);
    write(stream, play_duration_);
    write(stream, save_date_time_);
    write(stream, session_visibility_);
    write(stream, editor_object_version_);
    write_length_string(stream, mod_metadata_);
    write(stream, is_modded_save_);
}

std::string Satisfactory3DMap::SaveHeader::toString() const {
    // save_date_time is integer ticks since 0001-01-01 00:00, where 1 tick is 100 nano seconds.
    // See: https://docs.unrealengine.com/en-US/API/Runtime/Core/Misc/FDateTime/index.html
    // Satisfactory seems to use UTC.
    // Convert to unix timestamp:
    // Python: (datetime.datetime(1970, 1, 1) - datetime.datetime(1, 1, 1)).total_seconds()
    //   => 62135596800.0 seconds
    std::time_t save_date_time = (save_date_time_ - 621355968000000000) / 10000000;

    // Convert values
    std::string save_date_str(20, '\0');
    std::strftime(save_date_str.data(), save_date_str.size(), "%F %T", std::localtime(&save_date_time));
    save_date_str.erase(std::find(save_date_str.begin(), save_date_str.end(), '\0'), save_date_str.end());

    // Build string
    std::stringstream s;
    s << "Save Header Version:     " << save_header_version_ << std::endl;
    s << "Save Version:            " << save_version_ << std::endl;
    s << "Build Version:           " << build_version_ << std::endl;
    s << "Map Name:                " << map_name_ << std::endl;
    s << "Map Options:             " << map_options_ << std::endl;
    s << "Session Name:            " << session_name_ << std::endl;
    s << "Play Duration (seconds): " << play_duration_ << " (" << play_duration_ / 60.0 / 60.0 << " h)" << std::endl;
    s << "Save Date Time (ticks):  " << save_date_time << " (" << save_date_str << ")" << std::endl;
    s << "Session Visibility:      " << static_cast<bool>(session_visibility_) << std::endl;
    s << "Editor Object Version:   " << editor_object_version_ << std::endl;
    s << "Mod Metadata:            " << mod_metadata_ << std::endl;
    s << "Is Modded Save:          " << is_modded_save_ << std::endl;

    return s.str();
}

void Satisfactory3DMap::SaveHeader::print() const {
    std::cout << toString() << std::endl;
}
