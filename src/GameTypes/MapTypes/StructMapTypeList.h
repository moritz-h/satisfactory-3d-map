#pragma once

#include <vector>

#include "../Structs/PropertyStruct.h"
#include "MapTypeList.h"

namespace Satisfactory3DMap {

    class StructMapTypeList : public MapTypeList {
    public:
        using MapTypeList::MapTypeList;

        void accept(MapTypeListVisitor& v) override;

        void serializeEntry(Archive& ar, std::size_t i) override;

        [[nodiscard]] std::size_t listSize() const override {
            return list_.size();
        }

        [[nodiscard]] const std::vector<std::unique_ptr<PropertyStruct>>& list() const {
            return list_;
        }

    protected:
        std::vector<std::unique_ptr<PropertyStruct>> list_;
    };
} // namespace Satisfactory3DMap
