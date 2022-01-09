#include "Int8Property.h"

#include "PropertyVisitor.h"

void Satisfactory3DMap::Int8Property::serialize(Archive& ar) {
    Property::serialize(ar);
    ar << value_;
}

void Satisfactory3DMap::Int8Property::accept(Satisfactory3DMap::PropertyVisitor& v) {
    v.visit(*this);
}
