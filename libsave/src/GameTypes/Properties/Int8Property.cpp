#include "GameTypes/Properties/Int8Property.h"

#include "GameTypes/Properties/PropertyVisitor.h"

void Satisfactory3DMap::Int8Property::serialize(Archive& ar) {
    ar << value_;
}

void Satisfactory3DMap::Int8Property::accept(Satisfactory3DMap::PropertyVisitor& v) {
    v.visit(*this);
}
