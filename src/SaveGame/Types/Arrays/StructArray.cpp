#include "StructArray.h"

#include "ArrayVisitor.h"
#include "Utils/StreamUtils.h"

Satisfactory3DMap::StructArray::StructArray(std::string array_type, int32_t count, std::istream& stream)
    : Array(std::move(array_type)) {
    name_ = read_length_string(stream);
    auto type = read_length_string(stream);
    auto size = read<int32_t>(stream);
    auto index = read<int32_t>(stream);
    if (type != "StructProperty") {
        throw std::runtime_error("Invalid StructProperty array!");
    }

    struct_name_ = read_length_string(stream);
    guid_ = Guid(stream);
    read_assert_zero<int8_t>(stream);

    for (int32_t i = 0; i < count; i++) {
        array_.emplace_back(Struct::parse(struct_name_, stream));
    }
}

void Satisfactory3DMap::StructArray::accept(Satisfactory3DMap::ArrayVisitor& v) {
    v.visit(*this);
}
