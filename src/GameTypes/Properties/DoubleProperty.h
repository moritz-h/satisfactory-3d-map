#pragma once

#include "Property.h"

namespace Satisfactory3DMap {

    class DoubleProperty : public Property {
    public:
        using Property::Property;

        void serialize(Archive& ar) override;

        void accept(PropertyVisitor& v) override;

        [[nodiscard]] double value() const {
            return value_;
        }

    protected:
        double value_ = 0.0f;
    };
} // namespace Satisfactory3DMap
