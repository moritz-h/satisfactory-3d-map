#pragma once

#include <vector>

#include "../Misc/SoftObjectPath.h"
#include "Array.h"

namespace Satisfactory3DMap {

    class SoftObjectArray : public Array {
    public:
        using Array::Array;

        void serialize(Archive& ar) override;

        void accept(ArrayVisitor& v) override;

        [[nodiscard]] const std::vector<SoftObjectPath>& array() const {
            return array_;
        }

    protected:
        std::vector<SoftObjectPath> array_;
    };
} // namespace Satisfactory3DMap
