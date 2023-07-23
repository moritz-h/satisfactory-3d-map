#pragma once

#include <vector>

#include "Base/Property.h"

namespace SatisfactorySave {

    class ByteProperty : public Property {
    public:
        static constexpr std::string_view TypeName = "ByteProperty";

        using Property::Property;

        void serialize(Archive& ar) override;

        void accept(PropertyVisitor& v) override;

        [[nodiscard]] const FName& byteType() const {
            return Tag.EnumName;
        }

        [[nodiscard]] const FName& valueName() const {
            return value_name_;
        }

        [[nodiscard]] int8_t valueByte() const {
            return value_byte_;
        }

    protected:
        FName value_name_;
        int8_t value_byte_ = 0;
    };
} // namespace SatisfactorySave
