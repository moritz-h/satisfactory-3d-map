#include "GameTypes/Properties/FloatProperty.h"

#include "GameTypes/Properties/PropertyVisitor.h"

void Satisfactory3DMap::FloatProperty::serialize(Archive& ar) {
    ar << value_;
}

void Satisfactory3DMap::FloatProperty::accept(Satisfactory3DMap::PropertyVisitor& v) {
    v.visit(*this);
}
