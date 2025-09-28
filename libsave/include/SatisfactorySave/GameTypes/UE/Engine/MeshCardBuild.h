#pragma once

#include <vector>

#include "../../../IO/Archive/Archive.h"
#include "../Core/Math/Box.h"
#include "MeshCardRepresentation.h"
#include "satisfactorysave_export.h"

namespace SatisfactorySave {
    class SATISFACTORYSAVE_API FLumenCardBuildData {
    public:
        FLumenCardOBBf OBB;
        uint8_t AxisAlignedDirectionIndex = 0;

        void serialize(Archive& ar) {
            ar << OBB;
            ar << AxisAlignedDirectionIndex;
        }
    };

    class SATISFACTORYSAVE_API FMeshCardsBuildData {
    public:
        FBox Bounds;
        bool bMostlyTwoSided;
        std::vector<FLumenCardBuildData> CardBuildData;

        void serialize(Archive& ar) {
            ar << Bounds;
            ar << bMostlyTwoSided;
            ar << CardBuildData;
        }
    };

    class SATISFACTORYSAVE_API FCardRepresentationData {
    public:
        FMeshCardsBuildData MeshCardsBuildData;

        void serialize(Archive& ar) {
            ar << MeshCardsBuildData;
        }
    };
} // namespace SatisfactorySave
