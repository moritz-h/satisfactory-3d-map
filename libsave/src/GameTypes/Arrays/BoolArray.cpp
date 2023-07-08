#include "GameTypes/Arrays/BoolArray.h"

#include "GameTypes/Arrays/ArrayVisitor.h"

void SatisfactorySave::BoolArray::serialize(Archive& ar) {
    ar << array_;
}

void SatisfactorySave::BoolArray::accept(SatisfactorySave::ArrayVisitor& v) {
    v.visit(*this);
}
