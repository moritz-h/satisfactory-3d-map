#include "GameTypes/Structs/LinearColorStruct.h"

#include "GameTypes/Structs/StructVisitor.h"

void SatisfactorySave::LinearColorStruct::serialize(Archive& ar) {
    ar << r_;
    ar << g_;
    ar << b_;
    ar << a_;
}

void SatisfactorySave::LinearColorStruct::accept(SatisfactorySave::StructVisitor& v) {
    v.visit(*this);
}
