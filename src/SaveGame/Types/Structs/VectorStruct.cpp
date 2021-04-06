#include "VectorStruct.h"

#include "StructVisitor.h"
#include "Utils/StreamUtils.h"

Satisfactory3DMap::VectorStruct::VectorStruct(std::string struct_name, std::istream& stream)
    : Struct(std::move(struct_name)) {
    value_ = read<glm::vec3>(stream);
}

void Satisfactory3DMap::VectorStruct::accept(Satisfactory3DMap::StructVisitor& v) {
    v.visit(*this);
}
