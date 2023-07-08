#include "BoolArray.h"

#include "ArrayVisitor.h"

void Satisfactory3DMap::BoolArray::serialize(Archive& ar) {
    ar << array_;
}

void Satisfactory3DMap::BoolArray::accept(Satisfactory3DMap::ArrayVisitor& v) {
    v.visit(*this);
}
