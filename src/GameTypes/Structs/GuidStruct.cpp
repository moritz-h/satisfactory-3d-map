#include "GuidStruct.h"

#include "StructVisitor.h"

void Satisfactory3DMap::GuidStruct::serialize(Archive& ar) {
    ar << guid_;
}

void Satisfactory3DMap::GuidStruct::accept(Satisfactory3DMap::StructVisitor& v) {
    v.visit(*this);
}
