#include "GameTypes/Properties/ByteProperty.h"

#include "GameTypes/Properties/PropertyVisitor.h"

void Satisfactory3DMap::ByteProperty::serialize(Archive& ar) {
    if (tag_.EnumName == "None") {
        ar << value_byte_;
    } else {
        ar << value_name_;
    }
}

void Satisfactory3DMap::ByteProperty::accept(Satisfactory3DMap::PropertyVisitor& v) {
    v.visit(*this);
}
