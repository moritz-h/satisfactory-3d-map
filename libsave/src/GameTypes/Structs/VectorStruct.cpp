#include "GameTypes/Structs/VectorStruct.h"

#include "GameTypes/Structs/StructVisitor.h"

void SatisfactorySave::VectorStruct::serialize(Archive& ar) {
    ar << value_;
}

void SatisfactorySave::VectorStruct::accept(SatisfactorySave::StructVisitor& v) {
    v.visit(*this);
}
