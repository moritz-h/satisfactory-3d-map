#pragma once

#include "../../../IO/Archive/Archive.h"

namespace SatisfactorySave {

    // FIntVector
    struct FIntVector {
    public:
        int32_t X = 0;
        int32_t Y = 0;
        int32_t Z = 0;

        void serialize(Archive& ar) {
            ar << X;
            ar << Y;
            ar << Z;
        }
    };
} // namespace SatisfactorySave
