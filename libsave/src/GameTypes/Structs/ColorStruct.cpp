#include "ColorStruct.h"

#include "StructVisitor.h"

void Satisfactory3DMap::ColorStruct::serialize(Archive& ar) {
    ar << b_;
    ar << g_;
    ar << r_;
    ar << a_;
}

void Satisfactory3DMap::ColorStruct::accept(Satisfactory3DMap::StructVisitor& v) {
    v.visit(*this);
}
