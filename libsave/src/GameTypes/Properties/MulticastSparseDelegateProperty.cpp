#include "GameTypes/Properties/MulticastSparseDelegateProperty.h"

#include "GameTypes/Properties/PropertyVisitor.h"

void Satisfactory3DMap::MulticastSparseDelegateProperty::serialize(Archive& ar) {
    ar << InvocationList_;
}

void Satisfactory3DMap::MulticastSparseDelegateProperty::accept(Satisfactory3DMap::PropertyVisitor& v) {
    v.visit(*this);
}
