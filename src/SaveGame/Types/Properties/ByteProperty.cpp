#include "ByteProperty.h"

#include <utility>

#include "Utils/StreamUtils.h"

Satisfactory3DMap::ByteProperty::ByteProperty(
    std::string property_name, std::string property_type, std::istream& stream)
    : Property(std::move(property_name), std::move(property_type), stream) {
    byte_type_ = read_length_string(stream);
    read_assert_zero<int8_t>(stream);

    if (byte_type_ == "None") {
        value_ = static_cast<char>(read<int8_t>(stream));
    } else {
        value_ = read_length_string(stream);
    }
}
