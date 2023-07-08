#include "EnumProperty.h"

#include "PropertyVisitor.h"

void Satisfactory3DMap::EnumProperty::serialize(Archive& ar) {
    ar << value_;
}

void Satisfactory3DMap::EnumProperty::accept(Satisfactory3DMap::PropertyVisitor& v) {
    v.visit(*this);
}
