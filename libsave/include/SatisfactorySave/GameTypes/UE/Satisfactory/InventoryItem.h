#pragma once

#include "../../../IO/Archive/Archive.h"
#include "ObjectReference.h"
#include "satisfactorysave_export.h"

namespace SatisfactorySave {

    // FInventoryItem
    struct SATISFACTORYSAVE_API FInventoryItem {
    public:
        // TODO find names
        int32_t unk1_ = 0;
        std::string class_name_;
        FObjectReferenceDisc ref_;

        void serialize(Archive& ar) {
            ar << unk1_;
            ar << class_name_;
            ar << ref_;
        }
    };
} // namespace SatisfactorySave
