#include "GameTypes/Arrays/Int64Array.h"

#include "GameTypes/Arrays/ArrayVisitor.h"

void SatisfactorySave::Int64Array::serialize(Archive& ar) {
    ar << array_;
}

void SatisfactorySave::Int64Array::accept(SatisfactorySave::ArrayVisitor& v) {
    v.visit(*this);
}
