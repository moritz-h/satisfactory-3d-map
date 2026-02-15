#pragma once

#include <cstdint>
#include <optional>
#include <vector>

#include "../../IO/Archive/Archive.h"
#include "../Save/SaveObjectFwd.h"
#include "../UE/Core/Containers/Map.h"
#include "../UE/Core/Misc/EngineVersion.h"
#include "../UE/Core/Serialization/CustomVersion.h"
#include "../UE/Core/UObject/ObjectVersion.h"
#include "FGObjectReference.h"
#include "satisfactorysave_export.h"

namespace SatisfactorySave {

    struct SATISFACTORYSAVE_API FSaveObjectVersionData {
        uint32_t SaveObjectVersionDataVersion = 0;
        FPackageFileVersion PackageFileVersion;
        int32_t LicenseeVersion = 0;
        FEngineVersion EngineVersion;
        FCustomVersionContainer CustomVersionContainer;

        void serialize(Archive& ar) {
            ar << SaveObjectVersionDataVersion;
            ar << PackageFileVersion;
            ar << LicenseeVersion;
            ar << EngineVersion;
            ar << CustomVersionContainer;
        }
    };

    struct SATISFACTORYSAVE_API FPerStreamingLevelSaveData {
        // The original struct in the game files actually uses TOCBlob64 and DataBlob64 buffers here, but instead we
        // are directly serializing the data types within these buffers.
        SaveObjectList SaveObjects;
        std::optional<std::vector<FObjectReferenceDisc>> TOC_DestroyedActors;

        int32_t SaveVersion = 53; // TODO use FSaveCustomVersion::LatestVersion
        std::vector<FObjectReferenceDisc> DestroyedActors;

        std::optional<FSaveObjectVersionData> StreamableLevelObjectVersionData;

        void serialize(Archive& ar);
    };

    struct SATISFACTORYSAVE_API FPersistentAndRuntimeSaveData {
        // The original struct in the game files actually uses TOCBlob64 and DataBlob64 buffers here, but instead we
        // are directly serializing the data types within these buffers.
        SaveObjectList SaveObjects;
        std::optional<TMap<std::string, std::vector<FObjectReferenceDisc>>> TOC_LevelToDestroyedActorsMap;

        TMap<std::string, std::vector<FObjectReferenceDisc>> LevelToDestroyedActorsMap;

        void serialize(Archive& ar);
    };

    struct SATISFACTORYSAVE_API FUnresolvedWorldSaveData {
        std::vector<FObjectReferenceDisc> DestroyedActors;

        inline void serialize(Archive& ar) {
            ar << DestroyedActors;
        }
    };

} // namespace SatisfactorySave
