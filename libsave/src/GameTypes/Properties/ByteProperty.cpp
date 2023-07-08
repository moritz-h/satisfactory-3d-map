#include "GameTypes/Properties/ByteProperty.h"

#include "GameTypes/Properties/PropertyVisitor.h"

void SatisfactorySave::ByteProperty::serialize(Archive& ar) {
    if (tag_.EnumName == "None") {
        ar << value_byte_;
    } else {
        ar << value_name_;
    }
}

void SatisfactorySave::ByteProperty::accept(SatisfactorySave::PropertyVisitor& v) {
    v.visit(*this);
}
