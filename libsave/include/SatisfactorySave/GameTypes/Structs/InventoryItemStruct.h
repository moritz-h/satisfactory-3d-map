#pragma once

#include "../UE/Satisfactory/InventoryItem.h"
#include "Base/StructImpl.h"

namespace SatisfactorySave {

    class SATISFACTORYSAVE_API InventoryItemStruct : public StructImpl<InventoryItemStruct, FInventoryItem> {
    public:
        static constexpr std::string_view TypeName = "InventoryItem";

        using StructImpl<InventoryItemStruct, FInventoryItem>::StructImpl;
    };
} // namespace SatisfactorySave
