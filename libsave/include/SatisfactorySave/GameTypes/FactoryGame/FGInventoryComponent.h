#pragma once

#include "../../IO/Archive/Archive.h"
#include "FGDynamicStruct.h"
#include "FGObjectReference.h"
#include "satisfactorysave_export.h"

namespace SatisfactorySave {

    struct SATISFACTORYSAVE_API FInventoryItem {
    public:
        FObjectReferenceDisc ItemClass;
        FFGDynamicStruct ItemState;
        FObjectReferenceDisc LegacyItemStateActor;

        void serialize(Archive& ar) {
            ar << ItemClass;
            if (ar.getSaveVersion() >= 43) {
                ar << ItemState;
            } else {
                ar << LegacyItemStateActor;
            }
        }
    };
} // namespace SatisfactorySave
