#pragma once

#include <vector>

#include "MapTypeList.h"

namespace Satisfactory3DMap {

    class IntMapTypeList : public MapTypeList {
    public:
        using MapTypeList::MapTypeList;

        void accept(MapTypeListVisitor& v) override;

        void serializeEntry(Archive& ar, std::size_t i) override;

        [[nodiscard]] std::size_t listSize() const override {
            return list_.size();
        }

        [[nodiscard]] const std::vector<int32_t>& list() const {
            return list_;
        }

    protected:
        std::vector<int32_t> list_;
    };
} // namespace Satisfactory3DMap
