#include "LinearColorStruct.h"

#include "StructVisitor.h"
#include "Utils/StreamUtils.h"

Satisfactory3DMap::LinearColorStruct::LinearColorStruct(std::string struct_name, std::istream& stream)
    : Struct(std::move(struct_name)) {
    r_ = read<float>(stream);
    g_ = read<float>(stream);
    b_ = read<float>(stream);
    a_ = read<float>(stream);
}

void Satisfactory3DMap::LinearColorStruct::accept(Satisfactory3DMap::StructVisitor& v) {
    v.visit(*this);
}
