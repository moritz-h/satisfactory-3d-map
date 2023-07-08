#include "ObjectProperty.h"

#include "PropertyVisitor.h"

void Satisfactory3DMap::ObjectProperty::serialize(Archive& ar) {
    ar << value_;
}

void Satisfactory3DMap::ObjectProperty::accept(Satisfactory3DMap::PropertyVisitor& v) {
    v.visit(*this);
}
