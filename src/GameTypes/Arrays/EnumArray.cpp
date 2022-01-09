#include "EnumArray.h"

#include "ArrayVisitor.h"

void Satisfactory3DMap::EnumArray::serialize(Archive& ar) {
    int32_t count = static_cast<int32_t>(array_.size());
    ar << count;
    if (ar.isIArchive()) {
        array_.resize(count);
    }

    for (auto& e : array_) {
        ar << e;
    }
}

void Satisfactory3DMap::EnumArray::accept(Satisfactory3DMap::ArrayVisitor& v) {
    v.visit(*this);
}
