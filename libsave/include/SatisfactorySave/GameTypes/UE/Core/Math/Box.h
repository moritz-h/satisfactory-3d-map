#pragma once

#include "../../../../IO/Archive/Archive.h"
#include "Vector.h"

namespace SatisfactorySave {

    template<typename T>
    struct TBox {
    public:
        TVector<T> Min;
        TVector<T> Max;
        uint8_t IsValid = 0;

        void serialize(Archive& ar) {
            ar << Min;
            ar << Max;
            ar << IsValid;
        }
    };

    using FBox = TBox<double>;
} // namespace SatisfactorySave
