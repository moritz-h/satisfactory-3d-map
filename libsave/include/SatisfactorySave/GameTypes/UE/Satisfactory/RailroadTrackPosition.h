#pragma once

#include "../../../IO/Archive/Archive.h"
#include "ObjectReference.h"

namespace SatisfactorySave {

    // FRailroadTrackPosition
    struct FRailroadTrackPosition {
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
