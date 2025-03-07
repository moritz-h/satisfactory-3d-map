#pragma once

#include "Base/Property.h"

namespace SatisfactorySave {

    class SATISFACTORYSAVE_API BoolProperty final : public Property {
    public:
        static constexpr std::string_view TypeName = "BoolProperty";

        using value_type = bool;

        BoolProperty() : Property(FName(std::string(TypeName))) {}
        explicit BoolProperty(FName name) : Property(FName(std::string(TypeName)), std::move(name)) {}
        BoolProperty(FName name, bool value) : Property(FName(std::string(TypeName)), std::move(name)) {
            setValue(value);
        }
        explicit BoolProperty(PropertyTag tag) : Property(std::move(tag)) {}

        [[nodiscard]] std::shared_ptr<Property> clone() const override;

        void serialize([[maybe_unused]] Archive& ar) override {}

        void accept(PropertyVisitor& v) override;

        [[nodiscard]] inline bool getValue() const {
            return !!tag_.BoolVal;
        }

        inline void setValue(bool v) {
            tag_.BoolVal = v ? 1 : 0;
        }
    };
} // namespace SatisfactorySave
