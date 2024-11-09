#pragma once

#include "../../../IO/Archive/Archive.h"
#include "../Containers/Map.h"
#include "../GameFramework/Actor.h"
#include "ObjectReference.h"
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
