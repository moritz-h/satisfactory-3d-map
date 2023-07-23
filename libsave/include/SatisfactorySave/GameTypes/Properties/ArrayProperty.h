#pragma once

#include "../Arrays/Base/Array.h"
#include "Base/Property.h"

namespace SatisfactorySave {

    class ArrayProperty : public Property {
    public:
        static constexpr std::string_view TypeName = "ArrayProperty";

        using Property::Property;

        void serialize(Archive& ar) override;

        void accept(PropertyVisitor& v) override;

        [[nodiscard]] const FName& arrayType() const {
            return Tag.InnerType;
        }

        [[nodiscard]] const std::unique_ptr<Array>& array() const {
            return array_;
        }

    protected:
        std::unique_ptr<Array> array_;
    };
} // namespace SatisfactorySave
