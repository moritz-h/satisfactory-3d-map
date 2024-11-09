#pragma once

#include <vector>

#include "../../IO/Archive/Archive.h"
#include "FGInventoryComponent.h"
#include "satisfactorysave_export.h"

namespace SatisfactorySave {

    struct SATISFACTORYSAVE_API FConveyorBeltItem {
    public:
        FInventoryItem Item;
        float Offset = 0.0f;

        void serialize(Archive& ar) {
            ar << Item;
            ar << Offset;
        }
    };

    struct SATISFACTORYSAVE_API FConveyorBeltItems {
    public:
        std::vector<FConveyorBeltItem> Items;

        void serialize(Archive& ar) {
            ar << Items;
        }
    };

} // namespace SatisfactorySave
