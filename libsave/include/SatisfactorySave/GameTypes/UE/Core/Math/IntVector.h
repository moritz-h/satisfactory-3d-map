#pragma once

#include "../../../../IO/Archive/Archive.h"

namespace SatisfactorySave {

    template<typename T>
    struct TIntVector3 {
    public:
        T X = 0;
        T Y = 0;
        T Z = 0;

        void serialize(Archive& ar) {
            ar << X;
            ar << Y;
            ar << Z;
        }
    };

    using FInt32Vector3 = TIntVector3<int32_t>;
    using FIntVector = FInt32Vector3;
} // namespace SatisfactorySave
