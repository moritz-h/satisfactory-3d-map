#include "GameTypes/Structs/QuatStruct.h"

#include "GameTypes/Structs/Base/StructVisitor.h"

void SatisfactorySave::QuatStruct::serialize(Archive& ar) {
    ar << x_;
    ar << y_;
    ar << z_;
    ar << w_;
}

void SatisfactorySave::QuatStruct::accept(SatisfactorySave::StructVisitor& v) {
    v.visit(*this);
}
