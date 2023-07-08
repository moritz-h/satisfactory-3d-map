#pragma once

#include "Property.h"

namespace SatisfactorySave {

    class UInt32Property : public Property {
    public:
        using Property::Property;

        void serialize(Archive& ar) override;

        void accept(PropertyVisitor& v) override;

        [[nodiscard]] uint32_t value() const {
            return value_;
        }

    protected:
        uint32_t value_ = 0;
    };
} // namespace SatisfactorySave
