#pragma once

#include "../Arrays/Array.h"
#include "Property.h"

namespace SatisfactorySave {

    class ArrayProperty : public Property {
    public:
        using Property::Property;

        void serialize(Archive& ar) override;

        void accept(PropertyVisitor& v) override;

        [[nodiscard]] const FName& arrayType() const {
            return tag_.InnerType;
        }

        [[nodiscard]] const std::unique_ptr<Array>& array() const {
            return array_;
        }

    protected:
        std::unique_ptr<Array> array_;
    };
} // namespace SatisfactorySave
