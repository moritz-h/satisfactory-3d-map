#pragma once

#include <vector>

#include "MapTypeList.h"

namespace SatisfactorySave {

    class FloatMapTypeList : public MapTypeList {
    public:
        using MapTypeList::MapTypeList;

        void accept(MapTypeListVisitor& v) override;

        void serializeEntry(Archive& ar, std::size_t i) override;

        [[nodiscard]] std::size_t listSize() const override {
            return list_.size();
        }

        [[nodiscard]] const std::vector<float>& list() const {
            return list_;
        }

    protected:
        std::vector<float> list_;
    };
} // namespace SatisfactorySave