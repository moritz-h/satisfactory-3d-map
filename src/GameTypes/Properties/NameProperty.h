#pragma once

#include "Property.h"

namespace Satisfactory3DMap {

    class NameProperty : public Property {
    public:
        using Property::Property;

        void serialize(Archive& stream) override;

        void accept(PropertyVisitor& v) override;

        [[nodiscard]] const FName& value() const {
            return value_;
        }

    protected:
        FName value_;
    };
} // namespace Satisfactory3DMap
