#pragma once

#include <vector>

#include "../../IO/Archive/Archive.h"
#include "../UE/Engine/GameFramework/Actor.h"
#include "FGObjectReference.h"
#include "satisfactorysave_export.h"

namespace SatisfactorySave {

    class SATISFACTORYSAVE_API AFGGameState : public AActor {
    public:
        std::vector<FObjectReferenceDisc> rawPlayerStatePointers;

        void serialize(Archive& ar) override {
            AActor::serialize(ar);
            ar << rawPlayerStatePointers;
        }
    };

} // namespace SatisfactorySave
