#pragma once

#include <cstdint>
#include <string>

#include "../../IO/Archive/Archive.h"
#include "../UE/Core/Misc/DateTime.h"
#include "../UE/Core/Misc/SecureHash.h"
#include "satisfactorysave_export.h"

namespace SatisfactorySave {

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
            AddedSaveName,

            VersionPlusOne,
            LatestVersion = VersionPlusOne - 1,
        };

        int32_t SaveHeaderVersion = 0;
        int32_t SaveVersion = 0;
        int32_t BuildVersion = 0;
        std::string SaveName;
        std::string MapName;
        std::string MapOptions;
        std::string SessionName;
        int32_t PlayDurationSeconds = 0;
        FDateTime SaveDateTime;
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
