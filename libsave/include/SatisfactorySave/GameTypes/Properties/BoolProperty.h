#pragma once

#include "Base/Property.h"

namespace SatisfactorySave {

    class BoolProperty : public Property {
    public:
        static constexpr std::string_view TypeName = "BoolProperty";

        using Property::Property;

        void serialize([[maybe_unused]] Archive& ar) override{};

        void accept(PropertyVisitor& v) override;

        [[nodiscard]] uint8_t value() const {
            return Tag.BoolVal;
        }
    };
} // namespace SatisfactorySave
