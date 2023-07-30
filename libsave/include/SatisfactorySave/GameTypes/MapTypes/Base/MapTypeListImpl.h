#pragma once

#include <vector>

#include "MapTypeList.h"
#include "MapTypeListVisitor.h"

namespace SatisfactorySave {

    template<typename Impl, typename T>
    class MapTypeListImpl : public MapTypeList {
    public:
        void serializeEntry(Archive& ar, std::size_t i) override {
            if (List.size() <= i) {
                List.resize(i + 1);
            }
            ar << List[i];
        }

        void accept(MapTypeListVisitor& v) override {
            v.visit(static_cast<Impl&>(*this));
        }

        [[nodiscard]] std::size_t listSize() const override {
            return List.size();
        }

        std::vector<T> List;
    };
} // namespace SatisfactorySave
