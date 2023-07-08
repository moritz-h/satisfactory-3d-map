#include "Int64Array.h"

#include "ArrayVisitor.h"

void Satisfactory3DMap::Int64Array::serialize(Archive& ar) {
    ar << array_;
}

void Satisfactory3DMap::Int64Array::accept(Satisfactory3DMap::ArrayVisitor& v) {
    v.visit(*this);
}
