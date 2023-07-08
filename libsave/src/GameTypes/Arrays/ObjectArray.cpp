#include "GameTypes/Arrays/ObjectArray.h"

#include "GameTypes/Arrays/ArrayVisitor.h"

void SatisfactorySave::ObjectArray::serialize(Archive& ar) {
    ar << array_;
}

void SatisfactorySave::ObjectArray::accept(SatisfactorySave::ArrayVisitor& v) {
    v.visit(*this);
}
