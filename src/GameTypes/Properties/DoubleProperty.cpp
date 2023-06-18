#include "DoubleProperty.h"

#include "PropertyVisitor.h"

void Satisfactory3DMap::DoubleProperty::serialize(Archive& ar) {
    ar << value_;
}

void Satisfactory3DMap::DoubleProperty::accept(Satisfactory3DMap::PropertyVisitor& v) {
    v.visit(*this);
}
