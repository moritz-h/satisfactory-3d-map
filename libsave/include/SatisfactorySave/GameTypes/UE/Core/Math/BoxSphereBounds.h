#pragma once

#include "../../../../IO/Archive/Archive.h"
#include "Vector.h"

namespace SatisfactorySave {

    template<typename T, typename TExtent>
    struct TBoxSphereBounds {
    public:
        TVector<T> Origin;
        TVector<TExtent> BoxExtent;
        TExtent SphereRadius = static_cast<TExtent>(0.0);

        void serialize(Archive& ar) {
            ar << Origin;
            ar << BoxExtent;
            ar << SphereRadius;
        }
    };

    using FBoxSphereBounds3f = TBoxSphereBounds<float, float>;
    using FBoxSphereBounds3d = TBoxSphereBounds<double, double>;
    using FBoxSphereBounds = FBoxSphereBounds3d;
} // namespace SatisfactorySave
