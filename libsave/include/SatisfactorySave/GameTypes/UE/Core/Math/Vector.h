#pragma once

#include "../../../../IO/Archive/Archive.h"

namespace SatisfactorySave {

    template<typename T>
    struct TVector {
        T X = 0.0;
        T Y = 0.0;
        T Z = 0.0;

        void serialize(Archive& ar) {
            ar << X;
            ar << Y;
            ar << Z;
        }
    };

    using FVector = TVector<double>;
    using FVector3f = TVector<float>;
} // namespace SatisfactorySave
