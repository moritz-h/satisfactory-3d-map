#include "FluidBoxStruct.h"

#include "StructVisitor.h"

void Satisfactory3DMap::FluidBoxStruct::serialize(Archive& ar) {
    ar << value_;
}

void Satisfactory3DMap::FluidBoxStruct::accept(Satisfactory3DMap::StructVisitor& v) {
    v.visit(*this);
}
