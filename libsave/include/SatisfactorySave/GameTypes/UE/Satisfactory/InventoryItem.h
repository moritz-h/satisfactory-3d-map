#pragma once

#include "../../../IO/Archive/Archive.h"
#include "ObjectReference.h"
#include "satisfactorysave_export.h"

namespace SatisfactorySave {

    // FInventoryItem
    struct SATISFACTORYSAVE_API FInventoryItem {
    public:
        FObjectReferenceDisc ItemClass;
        FObjectReferenceDisc ItemState;

        void serialize(Archive& ar) {
            ar << ItemClass;
            ar << ItemState;
        }
    };
} // namespace SatisfactorySave
