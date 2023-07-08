#include "GameTypes/Arrays/IntArray.h"

#include "GameTypes/Arrays/ArrayVisitor.h"

void SatisfactorySave::IntArray::serialize(Archive& ar) {
    ar << array_;
}

void SatisfactorySave::IntArray::accept(SatisfactorySave::ArrayVisitor& v) {
    v.visit(*this);
}
