#include "SoftObjectArray.h"

#include "ArrayVisitor.h"

void Satisfactory3DMap::SoftObjectArray::serialize(Archive& ar) {
    ar << array_;
}

void Satisfactory3DMap::SoftObjectArray::accept(Satisfactory3DMap::ArrayVisitor& v) {
    v.visit(*this);
}
