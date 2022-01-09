#include "PropertyStruct.h"

#include "StructVisitor.h"

void Satisfactory3DMap::PropertyStruct::serialize(Archive& ar) {
    ar << properties_;
}

void Satisfactory3DMap::PropertyStruct::accept(Satisfactory3DMap::StructVisitor& v) {
    v.visit(*this);
}
