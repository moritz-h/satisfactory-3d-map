#pragma once

#include <vector>

#include "Property.h"

namespace Satisfactory3DMap {

    class TextProperty : public Property {
    public:
        using Property::Property;

        void serialize(Archive& ar) override;

        void accept(PropertyVisitor& v) override;

        [[nodiscard]] const std::vector<char>& buf() const {
            return buf_;
        }

    protected:
        std::vector<char> buf_;
    };
} // namespace Satisfactory3DMap
