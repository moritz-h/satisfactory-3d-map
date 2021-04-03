#include "MapProperty.h"

#include <utility>

#include "Utils/StreamUtils.h"

Satisfactory3DMap::MapProperty::MapProperty(std::string property_name, std::string property_type, std::istream& stream)
    : Property(std::move(property_name), std::move(property_type), stream) {
    key_type_ = read_length_string(stream);
    value_type_ = read_length_string(stream);
    read_assert_zero<int8_t>(stream);

    buf_ = read_vector<char>(stream, size_); // TODO
}
