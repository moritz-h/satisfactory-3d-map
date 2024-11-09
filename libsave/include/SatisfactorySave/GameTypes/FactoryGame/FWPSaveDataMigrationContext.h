#pragma once

#include "../../IO/Archive/Archive.h"
#include "../UE/Core/Containers/Map.h"
#include "../UE/Core/UObject/NameTypes.h"
#include "satisfactorysave_export.h"

namespace SatisfactorySave {

    struct SATISFACTORYSAVE_API FWPGridValidationData {
        int32_t CellSize = 0;
        uint32_t GridHash = 0;
        TMap<FName, uint32_t> CellHashes;

        void serialize(Archive& ar) {
            ar << CellSize;
            ar << GridHash;
            ar << CellHashes;
        }
    };

    struct SATISFACTORYSAVE_API FWorldPartitionValidationData {
        TMap<FName, FWPGridValidationData> Grids;

        void serialize(Archive& ar) {
            ar << Grids;
        }
    };

} // namespace SatisfactorySave
