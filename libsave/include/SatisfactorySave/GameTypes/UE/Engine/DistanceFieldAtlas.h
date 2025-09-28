#pragma once

#include <array>
#include <cstdint>
#include <vector>

#include "../../../IO/Archive/Archive.h"
#include "../Core/Math/Box.h"
#include "../Core/Math/IntVector.h"
#include "../Core/Math/Vector2D.h"
#include "../CoreUObject/Serialization/BulkData.h"
#include "satisfactorysave_export.h"

namespace SatisfactorySave {
    class SATISFACTORYSAVE_API FSparseDistanceFieldMip {
    public:
        FIntVector IndirectionDimensions;
        int32_t NumDistanceFieldBricks = 0;
        FVector VolumeToVirtualUVScale;
        FVector VolumeToVirtualUVAdd;
        FVector2D DistanceFieldToVolumeScaleBias;
        uint32_t BulkOffset = 0;
        uint32_t BulkSize = 0;

        void serialize(Archive& ar) {
            ar << IndirectionDimensions;
            ar << NumDistanceFieldBricks;
            ar << VolumeToVirtualUVScale;
            ar << VolumeToVirtualUVAdd;
            ar << DistanceFieldToVolumeScaleBias;
            ar << BulkOffset;
            ar << BulkSize;
        }
    };

    class SATISFACTORYSAVE_API FDistanceFieldVolumeData {
    public:
        FBox LocalSpaceMeshBounds;
        bool bMostlyTwoSided;
        std::array<FSparseDistanceFieldMip, /*DistanceField::NumMips*/ 3> Mips;
        std::vector<uint8_t> AlwaysLoadedMip;
        FByteBulkData StreamableMips;

        void serialize(Archive& ar) {
            ar << LocalSpaceMeshBounds;
            ar << bMostlyTwoSided;
            ar << Mips;
            ar << AlwaysLoadedMip;
            ar << StreamableMips;
        }
    };
} // namespace SatisfactorySave
