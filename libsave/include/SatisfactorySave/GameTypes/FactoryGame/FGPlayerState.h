#pragma once

#include "../../IO/Archive/Archive.h"
#include "../UE/Engine/GameFramework/Actor.h"
#include "../UE/Engine/GameFramework/OnlineReplStructs.h"
#include "satisfactorysave_export.h"

namespace SatisfactorySave {

    class SATISFACTORYSAVE_API AFGPlayerState : public AActor {
    public:
        FUniqueNetIdRepl Id;

        void serialize(Archive& ar) override {
            AActor::serialize(ar);
            ar << Id;
        }
    };

} // namespace SatisfactorySave
