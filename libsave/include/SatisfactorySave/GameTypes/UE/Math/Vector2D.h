#pragma once

#include "../../../IO/Archive/Archive.h"

namespace SatisfactorySave {

    // FVector2D
    struct FVector2D {
    public:
        double X = 0;
        double Y = 0;

        void serialize(Archive& ar) {
            ar << X;
            ar << Y;
        }
    };
} // namespace SatisfactorySave
