#pragma once

#include <optional>
#include <vector>

#include "../../IO/Archive/Archive.h"
#include "../UE/Core/Containers/Map.h"
#include "SatisfactorySave/GameTypes/FactoryGame/FGObjectReference.h"
#include "SatisfactorySave/GameTypes/Save/SaveObject.h"
#include "satisfactorysave_export.h"

namespace SatisfactorySave {

    struct SATISFACTORYSAVE_API FPerStreamingLevelSaveData {
        // The original struct in the game files actually uses TOCBlob64 and DataBlob64 buffers here, but instead we
        // are directly serializing the data types within these buffers.
        SaveObjectList SaveObjects;
        std::optional<std::vector<FObjectReferenceDisc>> TOC_DestroyedActors;

        int32_t SaveVersion = 51; // TODO use FSaveCustomVersion::LatestVersion
        std::vector<FObjectReferenceDisc> DestroyedActors;

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
