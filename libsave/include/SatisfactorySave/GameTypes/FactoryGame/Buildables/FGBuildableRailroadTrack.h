#pragma once

#include "../../../IO/Archive/Archive.h"
#include "../FGObjectReference.h"
#include "satisfactorysave_export.h"

namespace SatisfactorySave {

    struct SATISFACTORYSAVE_API FRailroadTrackPosition {
    public:
        FObjectReferenceDisc Track;
        float Offset = 0.0f;
        float Forward = 0.0f;

        void serialize(Archive& ar) {
            ar << Track;
            ar << Offset;
            ar << Forward;
        }
    };
} // namespace SatisfactorySave
