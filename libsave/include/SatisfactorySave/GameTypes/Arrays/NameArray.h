#pragma once

#include <vector>

#include "../Misc/Name.h"
#include "Array.h"

namespace SatisfactorySave {

    class NameArray : public Array {
    public:
        using Array::Array;

        void serialize(Archive& ar) override;

        void accept(ArrayVisitor& v) override;

        [[nodiscard]] const std::vector<FName>& array() const {
            return array_;
        }

    protected:
        std::vector<FName> array_;
    };
} // namespace SatisfactorySave
