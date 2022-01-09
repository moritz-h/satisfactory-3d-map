#include "ByteProperty.h"

#include "PropertyVisitor.h"

void Satisfactory3DMap::ByteProperty::serialize(Archive& ar) {
    if (tag_.EnumName == "None") {
        if (ar.isIArchive()) {
            int8_t value;
            ar << value;
            value_ = static_cast<char>(value);
        } else {
            int8_t value = static_cast<int8_t>(value_[0]);
            ar << value;
        }
    } else {
        ar << value_;
    }
}

void Satisfactory3DMap::ByteProperty::accept(Satisfactory3DMap::PropertyVisitor& v) {
    v.visit(*this);
}
