#pragma once

#include "../../../IO/Archive/Archive.h"

namespace SatisfactorySave {

    // FFluidBox
    struct FFluidBox {
    public:
        float Value;

        void serialize(Archive& ar) {
            ar << Value;
        }
    };
} // namespace SatisfactorySave
