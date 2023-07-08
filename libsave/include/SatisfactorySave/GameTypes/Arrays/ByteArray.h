#pragma once

#include <vector>

#include "Array.h"

namespace Satisfactory3DMap {

    class ByteArray : public Array {
    public:
        using Array::Array;

        void serialize(Archive& ar) override;

        void accept(ArrayVisitor& v) override;

        [[nodiscard]] const std::vector<int8_t>& array() const {
            return array_;
        }

    protected:
        std::vector<int8_t> array_;
    };
} // namespace Satisfactory3DMap
