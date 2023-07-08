#pragma once

#include "Property.h"

namespace SatisfactorySave {

    class IntProperty : public Property {
    public:
        using Property::Property;

        void serialize(Archive& ar) override;

        void accept(PropertyVisitor& v) override;

        [[nodiscard]] int32_t value() const {
            return value_;
        }

    protected:
        int32_t value_ = 0;
    };
} // namespace SatisfactorySave
