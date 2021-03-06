#include "VectorStruct.h"

#include "StructVisitor.h"

void Satisfactory3DMap::VectorStruct::serialize(Archive& ar) {
    ar << value_;
}

void Satisfactory3DMap::VectorStruct::accept(Satisfactory3DMap::StructVisitor& v) {
    v.visit(*this);
}
