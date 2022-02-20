#include "RotatorStruct.h"

#include "StructVisitor.h"

void Satisfactory3DMap::RotatorStruct::serialize(Archive& ar) {
    ar << pitch_;
    ar << yaw_;
    ar << roll_;
}

void Satisfactory3DMap::RotatorStruct::accept(Satisfactory3DMap::StructVisitor& v) {
    v.visit(*this);
}
