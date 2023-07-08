#include "GameTypes/Structs/FluidBoxStruct.h"

#include "GameTypes/Structs/StructVisitor.h"

void SatisfactorySave::FluidBoxStruct::serialize(Archive& ar) {
    ar << value_;
}

void SatisfactorySave::FluidBoxStruct::accept(SatisfactorySave::StructVisitor& v) {
    v.visit(*this);
}
