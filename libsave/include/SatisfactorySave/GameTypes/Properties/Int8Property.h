#pragma once

#include "Property.h"

namespace SatisfactorySave {

    class Int8Property : public Property {
    public:
        using Property::Property;

        void serialize(Archive& ar) override;

        void accept(PropertyVisitor& v) override;

        [[nodiscard]] int8_t value() const {
            return value_;
        }

    protected:
        int8_t value_ = 0;
    };
} // namespace SatisfactorySave
