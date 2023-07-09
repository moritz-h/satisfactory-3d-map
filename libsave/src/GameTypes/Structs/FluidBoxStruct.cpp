#include "GameTypes/Structs/FluidBoxStruct.h"

#include "GameTypes/Structs/Base/StructVisitor.h"

void SatisfactorySave::FluidBoxStruct::serialize(Archive& ar) {
    ar << value_;
}

void SatisfactorySave::FluidBoxStruct::accept(SatisfactorySave::StructVisitor& v) {
    v.visit(*this);
}
