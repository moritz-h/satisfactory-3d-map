#pragma once

#include "../UE/Satisfactory/InventoryItem.h"
#include "Base/StructImpl.h"

namespace SatisfactorySave {

    class SATISFACTORYSAVE_API InventoryItemStruct : public StructImpl<InventoryItemStruct, FInventoryItem> {
    public:
        using StructImpl<InventoryItemStruct, FInventoryItem>::StructImpl;
    };
} // namespace SatisfactorySave
