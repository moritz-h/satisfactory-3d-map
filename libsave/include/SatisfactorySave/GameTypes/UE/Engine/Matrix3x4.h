#pragma once

#include <array>

#include "../../../IO/Archive/Archive.h"
#include "satisfactorysave_export.h"

namespace SatisfactorySave {
    struct SATISFACTORYSAVE_API FMatrix3x4 {
    public:
        std::array<float, 12> M;

        void serialize(Archive& ar) {
            ar << M;
        }
    };
} // namespace SatisfactorySave
