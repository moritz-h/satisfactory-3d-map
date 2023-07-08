#pragma once

#include <vector>

#include "Array.h"

namespace SatisfactorySave {

    class IntArray : public Array {
    public:
        using Array::Array;

        void serialize(Archive& ar) override;

        void accept(ArrayVisitor& v) override;

        [[nodiscard]] const std::vector<int32_t>& array() const {
            return array_;
        }

    protected:
        std::vector<int32_t> array_;
    };
} // namespace SatisfactorySave
