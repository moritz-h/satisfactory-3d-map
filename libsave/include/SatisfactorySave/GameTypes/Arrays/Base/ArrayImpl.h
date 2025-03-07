#pragma once

#include <vector>

#include "Array.h"
#include "ArrayVisitor.h"

namespace SatisfactorySave {

    template<typename Impl, typename T>
    class ArrayImpl : public Array {
    public:
        [[nodiscard]] std::shared_ptr<Array> clone() const override {
            return std::make_shared<Impl>(*dynamic_cast<const Impl*>(this));
        }

        void serialize(Archive& ar) override {
            ar << Values;
        }

        void accept(ArrayVisitor& v) override {
            v.visit(static_cast<Impl&>(*this));
        }

        std::vector<T> Values;
    };
} // namespace SatisfactorySave
