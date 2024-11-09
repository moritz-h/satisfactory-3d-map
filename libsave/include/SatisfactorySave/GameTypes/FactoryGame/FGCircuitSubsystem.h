#pragma once

#include "../../IO/Archive/Archive.h"
#include "../UE/Core/Containers/Map.h"
#include "../UE/Engine/GameFramework/Actor.h"
#include "FGObjectReference.h"
#include "satisfactorysave_export.h"

namespace SatisfactorySave {

    class SATISFACTORYSAVE_API AFGCircuitSubsystem : public AActor {
    public:
        TMap<int32_t, FObjectReferenceDisc> mCircuits;

        void serialize(Archive& ar) override {
            AActor::serialize(ar);
            ar << mCircuits;
        }
    };

} // namespace SatisfactorySave
