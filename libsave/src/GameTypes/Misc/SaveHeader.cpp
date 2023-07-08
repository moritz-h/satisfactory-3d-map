#include "GameTypes/Misc/SaveHeader.h"

#include <algorithm>
#include <chrono>
#include <ctime>
#include <iostream>
#include <sstream>
#include <stdexcept>

void Satisfactory3DMap::SaveHeader::serialize(Satisfactory3DMap::Archive& ar) {
    ar << save_header_version_;
    if (save_header_version_ != 13) {
        throw std::runtime_error("Unknown Save-Header Version: " + std::to_string(save_header_version_));
    }
    ar << save_version_;
    if (save_version_ < 41) {
        throw std::runtime_error(
            "Save version must be at least 41 (Update 8). Found old version: " + std::to_string(save_version_));
    }
    ar << build_version_;
    ar << map_name_;
    ar << map_options_;
    ar << session_name_;
    ar << play_duration_;
    ar << save_date_time_;
    ar << session_visibility_;
    ar << editor_object_version_;
    ar << mod_metadata_;
    ar << is_modded_save_;
    ar << save_identifier_;
    ar << is_partitioned_world_;
    ar << save_data_hash_;
    ar << is_creative_mode_enabled_;
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
    s << "Save Identifier:         " << save_identifier_ << std::endl;
    s << "Is Partitioned World:    " << is_partitioned_world_ << std::endl;
    s << "Save Data Hash:          " << save_data_hash_.toString() << std::endl;
    s << "Is Creative Mode:        " << is_creative_mode_enabled_ << std::endl;

    return s.str();
}
