#include "GameTypes/Properties/Int64Property.h"

#include "GameTypes/Properties/PropertyVisitor.h"

void Satisfactory3DMap::Int64Property::serialize(Archive& ar) {
    ar << value_;
}

void Satisfactory3DMap::Int64Property::accept(Satisfactory3DMap::PropertyVisitor& v) {
    v.visit(*this);
}
