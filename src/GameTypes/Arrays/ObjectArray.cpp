#include "ObjectArray.h"

#include "ArrayVisitor.h"

void Satisfactory3DMap::ObjectArray::serialize(Archive& ar) {
    int32_t count = static_cast<int32_t>(array_.size());
    ar << count;
    if (ar.isIArchive()) {
        array_.resize(count);
    }

    for (auto& objRef : array_) {
        ar << objRef;
    }
}

void Satisfactory3DMap::ObjectArray::accept(Satisfactory3DMap::ArrayVisitor& v) {
    v.visit(*this);
}
