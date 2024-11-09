#pragma once

#include "../../../../IO/Archive/Archive.h"

namespace SatisfactorySave {

    template<typename T>
    struct TQuat {
        T X = 0.0;
        T Y = 0.0;
        T Z = 0.0;
        T W = 0.0;

        void serialize(Archive& ar) {
            ar << X;
            ar << Y;
            ar << Z;
            ar << W;
        }
    };

    using FQuat = TQuat<double>;
    using FQuat4f = TQuat<float>;
} // namespace SatisfactorySave
