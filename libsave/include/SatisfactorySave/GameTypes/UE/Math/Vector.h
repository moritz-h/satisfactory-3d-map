#pragma once

#include "../../../IO/Archive/Archive.h"

namespace SatisfactorySave {

    // FVector
    struct FVector {
    public:
        double X = 0;
        double Y = 0;
        double Z = 0;

        void serialize(Archive& ar) {
            ar << X;
            ar << Y;
            ar << Z;
        }
    };
} // namespace SatisfactorySave
