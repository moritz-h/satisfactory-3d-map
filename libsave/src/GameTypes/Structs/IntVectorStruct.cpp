#include "IntVectorStruct.h"

#include "StructVisitor.h"

void Satisfactory3DMap::IntVectorStruct::serialize(Archive& ar) {
    ar << value_;
}

void Satisfactory3DMap::IntVectorStruct::accept(Satisfactory3DMap::StructVisitor& v) {
    v.visit(*this);
}
