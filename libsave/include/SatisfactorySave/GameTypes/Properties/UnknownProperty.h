#pragma once

#include <vector>

#include "Property.h"

namespace Satisfactory3DMap {

    class UnknownProperty : public Property {
    public:
        using Property::Property;

        void serialize(Archive& ar) override;

        void accept(PropertyVisitor& v) override;

        [[nodiscard]] const std::vector<char>& value() const {
            return value_;
        }

    protected:
        std::vector<char> value_;
    };
} // namespace Satisfactory3DMap
