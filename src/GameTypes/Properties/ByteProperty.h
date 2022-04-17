#pragma once

#include <vector>

#include "Property.h"

namespace Satisfactory3DMap {

    class ByteProperty : public Property {
    public:
        using Property::Property;

        void serialize(Archive& ar) override;

        void accept(PropertyVisitor& v) override;

        [[nodiscard]] const FName& byteType() const {
            return tag_.EnumName;
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
} // namespace Satisfactory3DMap
