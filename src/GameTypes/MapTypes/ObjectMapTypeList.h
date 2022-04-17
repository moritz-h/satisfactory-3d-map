#pragma once

#include <vector>

#include "../ObjectReference.h"
#include "MapTypeList.h"

namespace Satisfactory3DMap {

    class ObjectMapTypeList : public MapTypeList {
    public:
        using MapTypeList::MapTypeList;

        void accept(MapTypeListVisitor& v) override;

        void serializeEntry(Archive& ar, std::size_t i) override;

        [[nodiscard]] std::size_t listSize() const override {
            return list_.size();
        }

        [[nodiscard]] const std::vector<ObjectReference>& list() const {
            return list_;
        }

    protected:
        std::vector<ObjectReference> list_;
    };
} // namespace Satisfactory3DMap
