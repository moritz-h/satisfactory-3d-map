#include "GameTypes/Structs/ColorStruct.h"

#include "GameTypes/Structs/StructVisitor.h"

void SatisfactorySave::ColorStruct::serialize(Archive& ar) {
    ar << b_;
    ar << g_;
    ar << r_;
    ar << a_;
}

void SatisfactorySave::ColorStruct::accept(SatisfactorySave::StructVisitor& v) {
    v.visit(*this);
}
