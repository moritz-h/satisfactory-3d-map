#pragma once

#include "../../../../IO/Archive/Archive.h"
#include "Quat.h"
#include "Vector.h"

namespace SatisfactorySave {

    template<typename T>
    struct TTransform {
        TQuat<T> Rotation;
        TVector<T> Translation;
        TVector<T> Scale3D;

        void serialize(Archive& ar) {
            ar << Rotation;
            ar << Translation;
            ar << Scale3D;
        }
    };

    using FTransform = TTransform<double>;
    using FTransform3f = TTransform<float>;
} // namespace SatisfactorySave
