#include "GameTypes/Arrays/ByteArray.h"

#include "GameTypes/Arrays/ArrayVisitor.h"

void SatisfactorySave::ByteArray::serialize(Archive& ar) {
    ar << array_;
}

void SatisfactorySave::ByteArray::accept(SatisfactorySave::ArrayVisitor& v) {
    v.visit(*this);
}
