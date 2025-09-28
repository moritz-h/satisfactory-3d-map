#pragma once

#include <array>

#include "../../../IO/Archive/Archive.h"
#include "satisfactorysave_export.h"

namespace SatisfactorySave {
    struct SATISFACTORYSAVE_API FMeshUVChannelInfo {
    public:
        bool bInitialized = false;
        bool bOverrideDensities = false;
        std::array<float, /*MAX_TEXCOORDS*/ 4> LocalUVDensities{};

        void serialize(Archive& ar) {
            ar << bInitialized;
            ar << bOverrideDensities;

            for (auto& localUVDensity : LocalUVDensities) {
                ar << localUVDensity;
            }
        }
    };
} // namespace SatisfactorySave
