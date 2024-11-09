#pragma once

#include <array>

#include "../../../IO/Archive/Archive.h"
#include "../../UE/Engine/GameFramework/Actor.h"
#include "../FGObjectReference.h"
#include "satisfactorysave_export.h"

namespace SatisfactorySave {

    class SATISFACTORYSAVE_API AFGBuildableWire : public AActor {
    public:
        std::array<FObjectReferenceDisc, 2> mConnections;

        void serialize(Archive& ar) override {
            AActor::serialize(ar);
            ar << mConnections[0];
            ar << mConnections[1];
        }
    };

} // namespace SatisfactorySave
