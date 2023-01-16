#include "Vector2DStruct.h"

#include "StructVisitor.h"

void Satisfactory3DMap::Vector2DStruct::serialize(Archive& ar) {
    ar << value_;
}

void Satisfactory3DMap::Vector2DStruct::accept(Satisfactory3DMap::StructVisitor& v) {
    v.visit(*this);
}
