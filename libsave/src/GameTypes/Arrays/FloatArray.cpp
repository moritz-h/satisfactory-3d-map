#include "GameTypes/Arrays/FloatArray.h"

#include "GameTypes/Arrays/ArrayVisitor.h"

void Satisfactory3DMap::FloatArray::serialize(Archive& ar) {
    ar << array_;
}

void Satisfactory3DMap::FloatArray::accept(Satisfactory3DMap::ArrayVisitor& v) {
    v.visit(*this);
}
