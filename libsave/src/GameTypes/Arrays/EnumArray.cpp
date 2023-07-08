#include "GameTypes/Arrays/EnumArray.h"

#include "GameTypes/Arrays/ArrayVisitor.h"

void Satisfactory3DMap::EnumArray::serialize(Archive& ar) {
    ar << array_;
}

void Satisfactory3DMap::EnumArray::accept(Satisfactory3DMap::ArrayVisitor& v) {
    v.visit(*this);
}
