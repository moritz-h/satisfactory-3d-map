#pragma once

#include "../../../IO/Archive/Archive.h"
#include "ObjectReference.h"

namespace SatisfactorySave {

    // FInventoryItem
    struct FInventoryItem {
    public:
        // TODO find names
        int32_t unk1_ = 0;
        std::string class_name_;
        ObjectReference ref_;

        void serialize(Archive& ar) {
            ar << unk1_;
            ar << class_name_;
            ar << ref_;
        }
    };
} // namespace SatisfactorySave
