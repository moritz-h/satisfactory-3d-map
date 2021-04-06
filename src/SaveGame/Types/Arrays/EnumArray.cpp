#include "EnumArray.h"

#include "ArrayVisitor.h"
#include "Utils/StreamUtils.h"

Satisfactory3DMap::EnumArray::EnumArray(std::string array_type, int32_t count, std::istream& stream)
    : Array(std::move(array_type)) {
    for (int32_t i = 0; i < count; i++) {
        array_.emplace_back(read_length_string(stream));
    }
}

void Satisfactory3DMap::EnumArray::accept(Satisfactory3DMap::ArrayVisitor& v) {
    v.visit(*this);
}
