#include "GameTypes/Structs/IntPointStruct.h"

#include "GameTypes/Structs/StructVisitor.h"

void SatisfactorySave::IntPointStruct::serialize(Archive& ar) {
    ar << x_;
    ar << y_;
}

void SatisfactorySave::IntPointStruct::accept(SatisfactorySave::StructVisitor& v) {
    v.visit(*this);
}
