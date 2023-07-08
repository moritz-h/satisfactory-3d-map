#include "GameTypes/Properties/UInt64Property.h"

#include "GameTypes/Properties/PropertyVisitor.h"

void Satisfactory3DMap::UInt64Property::serialize(Archive& ar) {
    ar << value_;
}

void Satisfactory3DMap::UInt64Property::accept(Satisfactory3DMap::PropertyVisitor& v) {
    v.visit(*this);
}
