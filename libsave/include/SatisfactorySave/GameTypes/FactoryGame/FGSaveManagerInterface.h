#pragma once

#include <cstdint>
#include <string>

#include "../../../IO/Archive/Archive.h"
#include "../Misc/Hash.h"
#include "satisfactorysave_export.h"

namespace SatisfactorySave {

    // FSaveHeader - FGSaveManagerInterface.h
    struct SATISFACTORYSAVE_API FSaveHeader {
        enum Type : int32_t {
            InitialVersion = 0,
            PrepareForLoadingMaps,
            AddedSessionId,
            AddedPlayDuration,
            SessionIDStringAndSaveTimeAdded,
            AddedSessionVisibility,
            LookAtTheComment,
            UE425EngineUpdate,
            AddedModdingParams,
            UE426EngineUpdate,
            AddedSaveIdentifier,
            AddedWorldPartitionSupport,
            AddedSaveModificationChecksum,
            AddedIsCreativeModeEnabled,

            VersionPlusOne,
            LatestVersion = VersionPlusOne - 1,
        };

        int32_t SaveHeaderVersion = 0;
        int32_t SaveVersion = 0;
        int32_t BuildVersion = 0;
        std::string MapName;
        std::string MapOptions;
        std::string SessionName;
        int32_t PlayDurationSeconds = 0;
        int64_t SaveDateTime = 0;
        int8_t SessionVisibility = 0;
        int32_t EditorObjectVersion = 0;
        std::string ModMetadata;
        bool IsModdedSave = false;
        std::string SaveIdentifier;
        bool IsPartitionedWorld = false;
        FMD5Hash SaveDataHash;
        bool IsCreativeModeEnabled = false;

        void serialize(Archive& ar);

        [[nodiscard]] std::string toString() const;
    };
} // namespace SatisfactorySave
