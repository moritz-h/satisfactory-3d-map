#include "StrProperty.h"

#include "PropertyVisitor.h"

void Satisfactory3DMap::StrProperty::serialize(Archive& ar) {
    ar << value_;
}

void Satisfactory3DMap::StrProperty::accept(Satisfactory3DMap::PropertyVisitor& v) {
    v.visit(*this);
}
