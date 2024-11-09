#pragma once

#include "../../../IO/Archive/Archive.h"
#include "../../UE/Engine/GameFramework/Actor.h"
#include "../FGConveyorItem.h"
#include "satisfactorysave_export.h"

namespace SatisfactorySave {

    class SATISFACTORYSAVE_API AFGBuildableConveyorBase : public AActor {
    public:
        FConveyorBeltItems mItems;

        void serialize(Archive& ar) override {
            AActor::serialize(ar);
            ar << mItems;
        }
    };

} // namespace SatisfactorySave
