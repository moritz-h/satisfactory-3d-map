#pragma once

#include <vector>

#include "../Misc/Name.h"
#include "MapTypeList.h"

namespace Satisfactory3DMap {

    class NameMapTypeList : public MapTypeList {
    public:
        using MapTypeList::MapTypeList;

        void accept(MapTypeListVisitor& v) override;

        void serializeEntry(Archive& ar, std::size_t i) override;

        [[nodiscard]] std::size_t listSize() const override {
            return list_.size();
        }

        [[nodiscard]] const std::vector<FName>& list() const {
            return list_;
        }

    protected:
        std::vector<FName> list_;
    };
} // namespace Satisfactory3DMap
