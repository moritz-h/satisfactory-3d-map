#pragma once

#include "../../../IO/Archive/Archive.h"
#include "Vector.h"

namespace SatisfactorySave {

    // FBox
    struct FBox {
    public:
        FVector Min;
        FVector Max;
        uint8_t IsValid = 0;

        void serialize(Archive& ar) {
            ar << Min;
            ar << Max;
            ar << IsValid;
        }
    };
} // namespace SatisfactorySave
