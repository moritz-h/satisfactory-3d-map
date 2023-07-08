#include "GameTypes/Arrays/Int64Array.h"

#include "GameTypes/Arrays/ArrayVisitor.h"

void Satisfactory3DMap::Int64Array::serialize(Archive& ar) {
    ar << array_;
}

void Satisfactory3DMap::Int64Array::accept(Satisfactory3DMap::ArrayVisitor& v) {
    v.visit(*this);
}
