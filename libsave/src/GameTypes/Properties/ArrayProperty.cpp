#include "GameTypes/Properties/ArrayProperty.h"

#include "GameTypes/Properties/PropertyVisitor.h"

void Satisfactory3DMap::ArrayProperty::serialize(Satisfactory3DMap::Archive& ar) {
    if (ar.isIArchive()) {
        array_ = Array::create(tag_.InnerType, ar);
    } else {
        ar << *array_;
    }
}

void Satisfactory3DMap::ArrayProperty::accept(Satisfactory3DMap::PropertyVisitor& v) {
    v.visit(*this);
}
