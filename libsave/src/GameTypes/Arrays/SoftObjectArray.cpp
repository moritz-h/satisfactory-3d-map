#include "GameTypes/Arrays/SoftObjectArray.h"

#include "GameTypes/Arrays/ArrayVisitor.h"

void Satisfactory3DMap::SoftObjectArray::serialize(Archive& ar) {
    ar << array_;
}

void Satisfactory3DMap::SoftObjectArray::accept(Satisfactory3DMap::ArrayVisitor& v) {
    v.visit(*this);
}
