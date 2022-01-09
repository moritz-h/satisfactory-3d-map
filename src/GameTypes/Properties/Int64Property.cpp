#include "Int64Property.h"

#include "PropertyVisitor.h"

void Satisfactory3DMap::Int64Property::serialize(Archive& ar) {
    Property::serialize(ar);
    ar << value_;
}

void Satisfactory3DMap::Int64Property::accept(Satisfactory3DMap::PropertyVisitor& v) {
    v.visit(*this);
}
