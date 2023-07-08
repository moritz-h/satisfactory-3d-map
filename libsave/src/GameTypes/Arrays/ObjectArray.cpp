#include "ObjectArray.h"

#include "ArrayVisitor.h"

void Satisfactory3DMap::ObjectArray::serialize(Archive& ar) {
    ar << array_;
}

void Satisfactory3DMap::ObjectArray::accept(Satisfactory3DMap::ArrayVisitor& v) {
    v.visit(*this);
}
