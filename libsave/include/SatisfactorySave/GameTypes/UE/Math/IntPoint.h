#pragma once

#include "../../../IO/Archive/Archive.h"

namespace SatisfactorySave {

    // FIntPoint
    struct FIntPoint {
    public:
        int32_t X = 0;
        int32_t Y = 0;

        void serialize(Archive& ar) {
            ar << X;
            ar << Y;
        }
    };
} // namespace SatisfactorySave
