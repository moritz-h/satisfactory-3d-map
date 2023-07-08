#include "GameTypes/Arrays/StrArray.h"

#include "GameTypes/Arrays/ArrayVisitor.h"

void SatisfactorySave::StrArray::serialize(Archive& ar) {
    ar << array_;
}

void SatisfactorySave::StrArray::accept(SatisfactorySave::ArrayVisitor& v) {
    v.visit(*this);
}
