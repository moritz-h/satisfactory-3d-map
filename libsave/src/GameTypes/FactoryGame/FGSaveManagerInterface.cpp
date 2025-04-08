#include "GameTypes/FactoryGame/FGSaveManagerInterface.h"

#include <iostream>
#include <sstream>
#include <stdexcept>
#include <string>

void SatisfactorySave::FSaveHeader::serialize(Archive& ar) {
    ar << SaveHeaderVersion;
    if (SaveHeaderVersion < FSaveHeader::InitialVersion || SaveHeaderVersion > FSaveHeader::LatestVersion) {
        throw std::runtime_error("Unknown SaveHeaderVersion: " + std::to_string(SaveHeaderVersion));
    }

    ar << SaveVersion;
    ar << BuildVersion;

    if (SaveHeaderVersion >= FSaveHeader::AddedSaveName) {
        ar << SaveName;
    }

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
    std::stringstream s;
    s << "SaveHeaderVersion:     " << SaveHeaderVersion << std::endl;
    s << "SaveVersion:           " << SaveVersion << std::endl;
    s << "BuildVersion:          " << BuildVersion << std::endl;
    s << "SaveName:              " << SaveName << std::endl;
    s << "MapName:               " << MapName << std::endl;
    s << "MapOptions:            " << MapOptions << std::endl;
    s << "SessionName:           " << SessionName << std::endl;
    s << "PlayDurationSeconds:   " << PlayDurationSeconds << " (" << PlayDurationSeconds / 3600.0 << " h)" << std::endl;
    s << "SaveDateTime:          " << SaveDateTime.toString() << std::endl;
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
