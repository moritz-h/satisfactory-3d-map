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

        void serialize(Archive& ar) override {
            ar << Owner;
            ar << Components;
            UObject::serialize(ar);
        }
    };

} // namespace SatisfactorySave
