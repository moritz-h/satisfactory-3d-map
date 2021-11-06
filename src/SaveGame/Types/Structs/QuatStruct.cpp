#include "QuatStruct.h"

#include "StructVisitor.h"
#include "Utils/StreamUtils.h"

Satisfactory3DMap::QuatStruct::QuatStruct(std::string struct_name, std::istream& stream)
    : Struct(std::move(struct_name)) {
    x_ = read<float>(stream);
    y_ = read<float>(stream);
    z_ = read<float>(stream);
    w_ = read<float>(stream);
}

void Satisfactory3DMap::QuatStruct::serialize(std::ostream& stream) const {
    write(stream, x_);
    write(stream, y_);
    write(stream, z_);
    write(stream, w_);
}

void Satisfactory3DMap::QuatStruct::accept(Satisfactory3DMap::StructVisitor& v) {
    v.visit(*this);
}
