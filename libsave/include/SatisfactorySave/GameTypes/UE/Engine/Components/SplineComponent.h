#pragma once

#include "../../../../IO/Archive/Archive.h"
#include "../../Core/Math/Vector.h"
#include "satisfactorysave_export.h"

namespace SatisfactorySave {

    struct SATISFACTORYSAVE_API FSplinePointData {
    public:
        FVector Location;
        FVector ArriveTangent;
        FVector LeaveTangent;

        void serialize(Archive& ar) {
            ar << Location;
            ar << ArriveTangent;
            ar << LeaveTangent;
        }
    };

} // namespace SatisfactorySave
