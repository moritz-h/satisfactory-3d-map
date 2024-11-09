#pragma once

#include "../../../../IO/Archive/Archive.h"

namespace SatisfactorySave {

    template<typename T>
    struct TIntPoint {
    public:
        T X = 0;
        T Y = 0;

        void serialize(Archive& ar) {
            ar << X;
            ar << Y;
        }
    };

    using FInt32Point = TIntPoint<int32_t>;
    using FIntPoint = FInt32Point;
} // namespace SatisfactorySave
