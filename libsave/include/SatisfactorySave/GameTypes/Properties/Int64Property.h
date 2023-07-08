#pragma once

#include "Property.h"

namespace SatisfactorySave {

    class Int64Property : public Property {
    public:
        using Property::Property;

        void serialize(Archive& ar) override;

        void accept(PropertyVisitor& v) override;

        [[nodiscard]] int64_t value() const {
            return value_;
        }

    protected:
        int64_t value_ = 0;
    };
} // namespace SatisfactorySave
