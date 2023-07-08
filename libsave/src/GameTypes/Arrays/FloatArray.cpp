#include "GameTypes/Arrays/FloatArray.h"

#include "GameTypes/Arrays/ArrayVisitor.h"

void SatisfactorySave::FloatArray::serialize(Archive& ar) {
    ar << array_;
}

void SatisfactorySave::FloatArray::accept(SatisfactorySave::ArrayVisitor& v) {
    v.visit(*this);
}
