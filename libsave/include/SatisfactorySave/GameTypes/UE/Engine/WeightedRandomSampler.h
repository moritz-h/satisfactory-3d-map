#pragma once

#include <cstdint>
#include <vector>

#include "../../../IO/Archive/Archive.h"
#include "satisfactorysave_export.h"

namespace SatisfactorySave {
    struct SATISFACTORYSAVE_API FWeightedRandomSampler {
        std::vector<float> Prob;
        std::vector<int32_t> Alias;
        float TotalWeight = 0.0f;

        void serialize(Archive& ar) {
            ar << Prob;
            ar << Alias;
            ar << TotalWeight;
        }
    };
} // namespace SatisfactorySave
