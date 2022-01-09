#include "NameProperty.h"

#include "PropertyVisitor.h"

void Satisfactory3DMap::NameProperty::serialize(Archive& ar) {
    Property::serialize(ar);
    ar << value_;
}

void Satisfactory3DMap::NameProperty::accept(Satisfactory3DMap::PropertyVisitor& v) {
    v.visit(*this);
}
