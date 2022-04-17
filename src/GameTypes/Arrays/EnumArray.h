#pragma once

#include <string>
#include <vector>

#include "Array.h"

namespace Satisfactory3DMap {

    class EnumArray : public Array {
    public:
        using Array::Array;

        void serialize(Archive& ar) override;

        void accept(ArrayVisitor& v) override;

        [[nodiscard]] const std::vector<std::string>& array() const {
            return array_;
        }

    protected:
        std::vector<std::string> array_;
    };
} // namespace Satisfactory3DMap
