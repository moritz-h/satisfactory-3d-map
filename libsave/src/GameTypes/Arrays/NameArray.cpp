#include "GameTypes/Arrays/NameArray.h"

#include "GameTypes/Arrays/ArrayVisitor.h"

void SatisfactorySave::NameArray::serialize(Archive& ar) {
    ar << array_;
}

void SatisfactorySave::NameArray::accept(SatisfactorySave::ArrayVisitor& v) {
    v.visit(*this);
}
