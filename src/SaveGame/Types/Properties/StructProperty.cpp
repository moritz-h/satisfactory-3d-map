#include "StructProperty.h"

#include <utility>

#include "../../Utils/StreamUtils.h"

Satisfactory3DMap::StructProperty::StructProperty(
    std::string property_name, std::string property_type, std::istream& stream)
    : Property(std::move(property_name), std::move(property_type), stream) {
    struct_name_ = read_length_string(stream);
    unk1_ = read<int32_t>(stream);
    unk2_ = read<int32_t>(stream);
    unk3_ = read<int32_t>(stream);
    unk4_ = read<int32_t>(stream);
    read_assert_zero<int8_t>(stream);
    stream.ignore(size_); // TODO
}
