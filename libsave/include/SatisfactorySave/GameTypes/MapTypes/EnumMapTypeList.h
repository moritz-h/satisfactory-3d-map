#pragma once

#include <string>
#include <vector>

#include "MapTypeList.h"

namespace Satisfactory3DMap {

    class EnumMapTypeList : public MapTypeList {
    public:
        using MapTypeList::MapTypeList;

        void accept(MapTypeListVisitor& v) override;

        void serializeEntry(Archive& ar, std::size_t i) override;

        [[nodiscard]] std::size_t listSize() const override {
            return list_.size();
        }

        [[nodiscard]] const std::vector<std::string>& list() const {
            return list_;
        }

    protected:
        std::vector<std::string> list_;
    };
} // namespace Satisfactory3DMap
