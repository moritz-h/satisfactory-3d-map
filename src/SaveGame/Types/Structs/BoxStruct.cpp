#include "BoxStruct.h"

#include "StructVisitor.h"
#include "Utils/StreamUtils.h"

Satisfactory3DMap::BoxStruct::BoxStruct(std::string struct_name, std::istream& stream)
    : Struct(std::move(struct_name)) {
    min_ = read<glm::vec3>(stream);
    max_ = read<glm::vec3>(stream);
    is_valid_ = read<uint8_t>(stream);
}

void Satisfactory3DMap::BoxStruct::accept(Satisfactory3DMap::StructVisitor& v) {
    v.visit(*this);
}
