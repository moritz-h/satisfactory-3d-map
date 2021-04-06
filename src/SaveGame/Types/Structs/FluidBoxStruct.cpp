#include "FluidBoxStruct.h"

#include "StructVisitor.h"
#include "Utils/StreamUtils.h"

Satisfactory3DMap::FluidBoxStruct::FluidBoxStruct(std::string struct_name, std::istream& stream)
    : Struct(std::move(struct_name)) {
    value_ = read<float>(stream);
}

void Satisfactory3DMap::FluidBoxStruct::accept(Satisfactory3DMap::StructVisitor& v) {
    v.visit(*this);
}
