#pragma once

#include "Property.h"

namespace SatisfactorySave {

    class BoolProperty : public Property {
    public:
        using Property::Property;

        void serialize([[maybe_unused]] Archive& ar) override{};

        void accept(PropertyVisitor& v) override;

        [[nodiscard]] uint8_t value() const {
            return tag_.BoolVal;
        }
    };
} // namespace SatisfactorySave
