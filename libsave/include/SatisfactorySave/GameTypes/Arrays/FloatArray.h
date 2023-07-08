#pragma once

#include <vector>

#include "Array.h"

namespace SatisfactorySave {

    class FloatArray : public Array {
    public:
        using Array::Array;

        void serialize(Archive& ar) override;

        void accept(ArrayVisitor& v) override;

        [[nodiscard]] const std::vector<float>& array() const {
            return array_;
        }

    protected:
        std::vector<float> array_;
    };
} // namespace SatisfactorySave
