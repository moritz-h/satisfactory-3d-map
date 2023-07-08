#include "GameTypes/Arrays/EnumArray.h"

#include "GameTypes/Arrays/ArrayVisitor.h"

void SatisfactorySave::EnumArray::serialize(Archive& ar) {
    ar << array_;
}

void SatisfactorySave::EnumArray::accept(SatisfactorySave::ArrayVisitor& v) {
    v.visit(*this);
}
