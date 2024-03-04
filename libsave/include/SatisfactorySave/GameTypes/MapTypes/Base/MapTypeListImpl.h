#pragma once

#include <vector>

#include "MapTypeList.h"
#include "MapTypeListVisitor.h"

namespace SatisfactorySave {

    template<typename Impl, typename T>
    class MapTypeListImplBase : public MapTypeList {
    public:
        void accept(MapTypeListVisitor& v) override {
            v.visit(static_cast<Impl&>(*this));
        }

        [[nodiscard]] std::size_t size() const override {
            return List.size();
        }

        void resize(std::size_t s) override {
            List.resize(s);
        }

        std::vector<T> List;
    };

    template<typename Impl, typename T>
    class MapTypeListImpl : public MapTypeListImplBase<Impl, T> {
    public:
        void serializeEntry(Archive& ar, std::size_t i) override {
            if (this->List.size() <= i) {
                this->List.resize(i + 1);
            }
            ar << this->List[i];
        }
    };
} // namespace SatisfactorySave
