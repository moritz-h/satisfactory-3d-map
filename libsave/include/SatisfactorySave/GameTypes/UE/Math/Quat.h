#pragma once

#include "../../../IO/Archive/Archive.h"

namespace SatisfactorySave {

    // FQuat
    struct FQuat {
    public:
        double X = 0.0;
        double Y = 0.0;
        double Z = 0.0;
        double W = 0.0;

        void serialize(Archive& ar) {
            ar << X;
            ar << Y;
            ar << Z;
            ar << W;
        }
    };
} // namespace SatisfactorySave
