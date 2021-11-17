#include "BoolProperty.h"

#include "PropertyVisitor.h"

void Satisfactory3DMap::BoolProperty::accept(Satisfactory3DMap::PropertyVisitor& v) {
    v.visit(*this);
}
