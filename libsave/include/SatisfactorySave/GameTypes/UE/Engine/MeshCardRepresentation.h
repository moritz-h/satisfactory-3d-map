#pragma once

#include "../../../IO/Archive/Archive.h"
#include "../Core/Math/Vector.h"

namespace SatisfactorySave {
    template<typename T>
    class TLumenCardOBB {
    public:
        TVector<T> Origin;
        TVector<T> AxisX;
        TVector<T> AxisY;
        TVector<T> AxisZ;
        TVector<T> Extent;

        void serialize(Archive& ar) {
            ar << AxisX;
            ar << AxisY;
            ar << AxisZ;
            ar << Origin;
            ar << Extent;
        }
    };

    using FLumenCardOBBf = TLumenCardOBB<float>;
    using FLumenCardOBBd = TLumenCardOBB<double>;
} // namespace SatisfactorySave
