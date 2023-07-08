#pragma once

#include <vector>

#include "../Text.h"
#include "Property.h"

namespace Satisfactory3DMap {

    class TextProperty : public Property {
    public:
        using Property::Property;

        void serialize(Archive& ar) override;

        void accept(PropertyVisitor& v) override;

        [[nodiscard]] const std::string& textString() const {
            return text_.string();
        }

    protected:
        FText text_;
    };
} // namespace Satisfactory3DMap
