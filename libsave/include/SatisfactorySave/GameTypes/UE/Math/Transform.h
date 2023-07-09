#pragma once

#include "../../../IO/Archive/Archive.h"
#include "Quat.h"
#include "Vector.h"

namespace SatisfactorySave {

    // TTransform
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

        static inline const TTransform<T> Identity{
            TQuat<T>::Identity,
            {0.0, 0.0, 0.0},
            {1.0, 1.0, 1.0},
        };
    };

    using FTransform3f = TTransform<float>;
} // namespace SatisfactorySave
