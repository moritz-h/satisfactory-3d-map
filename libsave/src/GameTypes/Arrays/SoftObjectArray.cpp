#include "GameTypes/Arrays/SoftObjectArray.h"

#include "GameTypes/Arrays/ArrayVisitor.h"

void SatisfactorySave::SoftObjectArray::serialize(Archive& ar) {
    ar << array_;
}

void SatisfactorySave::SoftObjectArray::accept(SatisfactorySave::ArrayVisitor& v) {
    v.visit(*this);
}
