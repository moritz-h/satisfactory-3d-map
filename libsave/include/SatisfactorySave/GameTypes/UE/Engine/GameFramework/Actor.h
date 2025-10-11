#pragma once

#include <vector>

#include "../../../../IO/Archive/Archive.h"
#include "../../../FactoryGame/FGObjectReference.h"
#include "../../CoreUObject/UObject/Object.h"
#include "satisfactorysave_export.h"

namespace SatisfactorySave {

    class SATISFACTORYSAVE_API AActor : public UObject {
    public:
        FObjectReferenceDisc Owner;
        std::vector<FObjectReferenceDisc> Components;
        std::string ActorLabel;

        void serialize(Archive& ar) override {
            if (!ar.IsPersistent()) {
                ar << Owner;
                ar << Components;
            }
            UObject::serialize(ar);
            if (ar.IsPersistent()) {
                bool bIsCooked = true;
                ar << bIsCooked;
                if (bIsCooked) {
                    ar << ActorLabel;
                }
            }
        }
    };

} // namespace SatisfactorySave
