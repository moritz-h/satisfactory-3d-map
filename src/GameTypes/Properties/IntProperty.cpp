#include "IntProperty.h"

#include "PropertyVisitor.h"

void Satisfactory3DMap::IntProperty::serialize(Archive& ar) {
    ar << value_;
}

void Satisfactory3DMap::IntProperty::accept(Satisfactory3DMap::PropertyVisitor& v) {
    v.visit(*this);
}
