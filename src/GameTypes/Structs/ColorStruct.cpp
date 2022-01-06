#include "ColorStruct.h"

#include "StructVisitor.h"
#include "Utils/StreamUtils.h"

Satisfactory3DMap::ColorStruct::ColorStruct(std::string struct_name, std::istream& stream)
    : Struct(std::move(struct_name)) {
    b_ = read<int8_t>(stream);
    g_ = read<int8_t>(stream);
    r_ = read<int8_t>(stream);
    a_ = read<int8_t>(stream);
}

void Satisfactory3DMap::ColorStruct::serialize(std::ostream& stream) const {
    write(stream, b_);
    write(stream, g_);
    write(stream, r_);
    write(stream, a_);
}

void Satisfactory3DMap::ColorStruct::accept(Satisfactory3DMap::StructVisitor& v) {
    v.visit(*this);
}
