#pragma once

#include <vector>

#include "../../../IO/Archive/Archive.h"
#include "../GameFramework/Actor.h"
#include "ObjectReference.h"
#include "satisfactorysave_export.h"

namespace SatisfactorySave {

    class SATISFACTORYSAVE_API AFGGameMode : public AActor {
    public:
        std::vector<FObjectReferenceDisc> rawPlayerStatePointers;

        void serialize(Archive& ar) override {
            AActor::serialize(ar);
            ar << rawPlayerStatePointers;
        }
    };

} // namespace SatisfactorySave
