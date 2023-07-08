#pragma once

#include "Property.h"

namespace Satisfactory3DMap {

    class UInt64Property : public Property {
    public:
        using Property::Property;

        void serialize(Archive& ar) override;

        void accept(PropertyVisitor& v) override;

        [[nodiscard]] uint64_t value() const {
            return value_;
        }

    protected:
        uint64_t value_ = 0;
    };
} // namespace Satisfactory3DMap
