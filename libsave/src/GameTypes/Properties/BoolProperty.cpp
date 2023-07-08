#include "GameTypes/Properties/BoolProperty.h"

#include "GameTypes/Properties/PropertyVisitor.h"

void Satisfactory3DMap::BoolProperty::accept(Satisfactory3DMap::PropertyVisitor& v) {
    v.visit(*this);
}
