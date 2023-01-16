#pragma once

#include "../SoftObjectPath.h"
#include "Property.h"

namespace Satisfactory3DMap {

    class SoftObjectProperty : public Property {
    public:
        using Property::Property;

        void serialize(Archive& ar) override;

        void accept(PropertyVisitor& v) override;

        [[nodiscard]] const SoftObjectPath& value() const {
            return value_;
        }

    protected:
        SoftObjectPath value_;
    };
} // namespace Satisfactory3DMap
