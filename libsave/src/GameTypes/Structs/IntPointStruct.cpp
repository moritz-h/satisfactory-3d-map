#include "GameTypes/Structs/IntPointStruct.h"

#include "GameTypes/Structs/StructVisitor.h"

void Satisfactory3DMap::IntPointStruct::serialize(Archive& ar) {
    ar << x_;
    ar << y_;
}

void Satisfactory3DMap::IntPointStruct::accept(Satisfactory3DMap::StructVisitor& v) {
    v.visit(*this);
}
