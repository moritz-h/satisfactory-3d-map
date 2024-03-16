#include "GameTypes/UE/Satisfactory/SaveHeader.h"

#include <algorithm>
#include <chrono>
#include <ctime>
#include <iostream>
#include <sstream>
#include <stdexcept>

void SatisfactorySave::FSaveHeader::serialize(Archive& ar) {
    ar << SaveHeaderVersion;
    ar << SaveVersion;
    ar << BuildVersion;
    ar << MapName;
    ar << MapOptions;

    if (SaveHeaderVersion >= FSaveHeader::AddedSessionId &&
        SaveHeaderVersion < FSaveHeader::SessionIDStringAndSaveTimeAdded && ar.isIArchive()) {
        int32_t sessionIDInt = 0;
        ar << sessionIDInt;
        SessionName = std::to_string(sessionIDInt);
    } else if (SaveHeaderVersion >= FSaveHeader::SessionIDStringAndSaveTimeAdded) {
        ar << SessionName;
    }

    if (SaveHeaderVersion >= FSaveHeader::AddedPlayDuration) {
        ar << PlayDurationSeconds;
    }

    if (SaveHeaderVersion >= FSaveHeader::SessionIDStringAndSaveTimeAdded) {
        ar << SaveDateTime;
    }

    if (SaveHeaderVersion >= FSaveHeader::AddedSessionVisibility) {
        ar << SessionVisibility;
    }

    if (SaveHeaderVersion >= FSaveHeader::UE425EngineUpdate) {
        ar << EditorObjectVersion;
    }

    if (SaveHeaderVersion >= FSaveHeader::AddedModdingParams) {
        ar << ModMetadata;
        ar << IsModdedSave;
    }

    if (SaveHeaderVersion >= FSaveHeader::AddedSaveIdentifier) {
        ar << SaveIdentifier;
    }

    if (SaveHeaderVersion >= FSaveHeader::AddedWorldPartitionSupport) {
        ar << IsPartitionedWorld;
    }

    if (SaveHeaderVersion >= FSaveHeader::AddedSaveModificationChecksum) {
        ar << SaveDataHash;
    }

    if (SaveHeaderVersion >= FSaveHeader::AddedIsCreativeModeEnabled) {
        ar << IsCreativeModeEnabled;
    }
}

std::string SatisfactorySave::FSaveHeader::toString() const {
    // save_date_time is integer ticks since 0001-01-01 00:00, where 1 tick is 100 nano seconds.
    // See: https://docs.unrealengine.com/en-US/API/Runtime/Core/Misc/FDateTime/index.html
    // Satisfactory seems to use UTC.
    // Convert to unix timestamp:
    // Python: (datetime.datetime(1970, 1, 1) - datetime.datetime(1, 1, 1)).total_seconds()
    //   => 62135596800.0 seconds
    std::time_t save_date_time = (SaveDateTime - 621355968000000000) / 10000000;

    // Convert values
    std::string save_date_str(20, '\0');
    std::strftime(save_date_str.data(), save_date_str.size(), "%F %T", std::localtime(&save_date_time));
    save_date_str.erase(std::find(save_date_str.begin(), save_date_str.end(), '\0'), save_date_str.end());

    // Build string
    std::stringstream s;
    s << "SaveHeaderVersion:     " << SaveHeaderVersion << std::endl;
    s << "SaveVersion:           " << SaveVersion << std::endl;
    s << "BuildVersion:          " << BuildVersion << std::endl;
    s << "MapName:               " << MapName << std::endl;
    s << "MapOptions:            " << MapOptions << std::endl;
    s << "SessionName:           " << SessionName << std::endl;
    s << "PlayDurationSeconds:   " << PlayDurationSeconds << " (" << PlayDurationSeconds / 3600.0 << " h)" << std::endl;
    s << "SaveDateTime:          " << save_date_time << " (" << save_date_str << ")" << std::endl;
    s << "SessionVisibility:     " << static_cast<bool>(SessionVisibility) << std::endl;
    s << "EditorObjectVersion:   " << EditorObjectVersion << std::endl;
    s << "ModMetadata:           " << ModMetadata << std::endl;
    s << "IsModdedSave:          " << IsModdedSave << std::endl;
    s << "SaveIdentifier:        " << SaveIdentifier << std::endl;
    s << "IsPartitionedWorld:    " << IsPartitionedWorld << std::endl;
    s << "SaveDataHash:          " << SaveDataHash.toString() << std::endl;
    s << "IsCreativeModeEnabled: " << IsCreativeModeEnabled << std::endl;

    return s.str();
}
