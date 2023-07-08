#pragma once

#include <vector>

#include "../Misc/ObjectReference.h"
#include "Array.h"

namespace SatisfactorySave {

    class ObjectArray : public Array {
    public:
        using Array::Array;

        void serialize(Archive& ar) override;

        void accept(ArrayVisitor& v) override;

        [[nodiscard]] const std::vector<ObjectReference>& array() const {
            return array_;
        }

    protected:
        std::vector<ObjectReference> array_;
    };
} // namespace SatisfactorySave
