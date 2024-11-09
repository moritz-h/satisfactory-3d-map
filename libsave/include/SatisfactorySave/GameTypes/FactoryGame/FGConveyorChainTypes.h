#pragma once

#include <vector>

#include "../../IO/Archive/Archive.h"
#include "../UE/Engine/Components/SplineComponent.h"
#include "FGObjectReference.h"
#include "satisfactorysave_export.h"

namespace SatisfactorySave {

    class SATISFACTORYSAVE_API FConveyorChainSplineSegment {
    public:
        FObjectReferenceDisc ChainActor;
        FObjectReferenceDisc ConveyorBase;
        std::vector<FSplinePointData> SplinePointData;
        float OffsetAtStart = 0.0f;
        float StartsAtLength = 0.0f;
        float EndsAtLength = 0.0f;
        int32_t FirstItemIndex = -1;
        int32_t LastItemIndex = -1;
        int32_t IndexInChainArray = -1;

        void serialize(Archive& ar) {
            ar << ChainActor;
            ar << ConveyorBase;
            ar << SplinePointData;
            ar << OffsetAtStart;
            ar << StartsAtLength;
            ar << EndsAtLength;
            ar << FirstItemIndex;
            ar << LastItemIndex;
            ar << IndexInChainArray;
        }
    };

} // namespace SatisfactorySave
