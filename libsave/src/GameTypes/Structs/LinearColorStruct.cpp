#include "LinearColorStruct.h"

#include "StructVisitor.h"

void Satisfactory3DMap::LinearColorStruct::serialize(Archive& ar) {
    ar << r_;
    ar << g_;
    ar << b_;
    ar << a_;
}

void Satisfactory3DMap::LinearColorStruct::accept(Satisfactory3DMap::StructVisitor& v) {
    v.visit(*this);
}
