#include "UInt32Property.h"

#include "PropertyVisitor.h"

void Satisfactory3DMap::UInt32Property::serialize(Archive& ar) {
    ar << value_;
}

void Satisfactory3DMap::UInt32Property::accept(Satisfactory3DMap::PropertyVisitor& v) {
    v.visit(*this);
}
