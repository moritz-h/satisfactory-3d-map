#pragma once

#include "Base/Property.h"

namespace SatisfactorySave {

    class SATISFACTORYSAVE_API BoolProperty final : public Property {
    public:
        static constexpr std::string_view TypeName = "BoolProperty";

        BoolProperty() : Property(FName(std::string(TypeName))) {}
        explicit BoolProperty(PropertyTag tag) : Property(std::move(tag)) {}

        std::unique_ptr<Property> clone() override;

        void serialize([[maybe_unused]] Archive& ar) override{};

        void accept(PropertyVisitor& v) override;

        [[nodiscard]] inline bool getValue() const {
            return !!tag_.BoolVal;
        }

        inline void setValue(bool v) {
            tag_.BoolVal = v ? 1 : 0;
        }
    };
} // namespace SatisfactorySave
