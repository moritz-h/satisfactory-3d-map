#pragma once

#include "../../../../IO/Archive/Archive.h"

namespace SatisfactorySave {

    template<typename T>
    struct TVector2 {
        T X = 0.0;
        T Y = 0.0;

        void serialize(Archive& ar) {
            ar << X;
            ar << Y;
        }
    };

    using FVector2D = TVector2<double>;
} // namespace SatisfactorySave
