#include "GameTypes/Structs/IntVectorStruct.h"

#include "GameTypes/Structs/Base/StructVisitor.h"

void SatisfactorySave::IntVectorStruct::serialize(Archive& ar) {
    ar << value_;
}

void SatisfactorySave::IntVectorStruct::accept(SatisfactorySave::StructVisitor& v) {
    v.visit(*this);
}
