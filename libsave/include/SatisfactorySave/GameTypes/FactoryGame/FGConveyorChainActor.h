#pragma once

#include <vector>

#include "../../IO/Archive/Archive.h"
#include "../UE/Engine/GameFramework/Actor.h"
#include "FGConveyorChainTypes.h"
#include "FGConveyorItem.h"
#include "FGObjectReference.h"
#include "satisfactorysave_export.h"

namespace SatisfactorySave {

    class SATISFACTORYSAVE_API AFGConveyorChainActor : public AActor {
    public:
        FObjectReferenceDisc mFirstConveyor;
        FObjectReferenceDisc mLastConveyor;
        std::vector<FConveyorChainSplineSegment> mChainSplineSegments;
        float mTotalLength = 0.0f;
        int32_t mNumItems = 0;
        int32_t mLeadItemIndex = -1;
        int32_t mTailItemIndex = -1;
        std::vector<FConveyorBeltItem> mConveyorChainItems;

        void serialize(Archive& ar) override {
            AActor::serialize(ar);
            ar << mFirstConveyor;
            ar << mLastConveyor;
            ar << mChainSplineSegments;
            ar << mTotalLength;
            ar << mNumItems;
            ar << mLeadItemIndex;
            ar << mTailItemIndex;
            ar << mConveyorChainItems;
        }
    };

} // namespace SatisfactorySave
