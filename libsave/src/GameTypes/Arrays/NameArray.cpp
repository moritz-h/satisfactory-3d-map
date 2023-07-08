#include "NameArray.h"

#include "ArrayVisitor.h"

void Satisfactory3DMap::NameArray::serialize(Archive& ar) {
    ar << array_;
}

void Satisfactory3DMap::NameArray::accept(Satisfactory3DMap::ArrayVisitor& v) {
    v.visit(*this);
}
