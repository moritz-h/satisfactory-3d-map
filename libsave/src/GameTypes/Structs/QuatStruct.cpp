#include "GameTypes/Structs/QuatStruct.h"

#include "GameTypes/Structs/StructVisitor.h"

void Satisfactory3DMap::QuatStruct::serialize(Archive& ar) {
    ar << x_;
    ar << y_;
    ar << z_;
    ar << w_;
}

void Satisfactory3DMap::QuatStruct::accept(Satisfactory3DMap::StructVisitor& v) {
    v.visit(*this);
}
