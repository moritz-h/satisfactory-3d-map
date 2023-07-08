#include "GameTypes/Properties/UnknownProperty.h"

#include "GameTypes/Properties/PropertyVisitor.h"

void Satisfactory3DMap::UnknownProperty::serialize(Archive& ar) {
    value_.resize(tag_.Size);
    ar.serializeRaw(value_.data(), tag_.Size);
}

void Satisfactory3DMap::UnknownProperty::accept(Satisfactory3DMap::PropertyVisitor& v) {
    v.visit(*this);
}
