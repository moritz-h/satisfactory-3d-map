#include "MapProperty.h"

#include <utility>

#include "../ObjectReference.h"
#include "../Structs/PropertyStruct.h"
#include "Utils/StreamUtils.h"

Satisfactory3DMap::MapProperty::MapProperty(std::string property_name, std::string property_type, std::istream& stream)
    : Property(std::move(property_name), std::move(property_type), stream) {
    key_type_ = read_length_string(stream);
    value_type_ = read_length_string(stream);
    read_assert_zero<int8_t>(stream);

    // TODO unknown
    read_assert_zero<int32_t>(stream);

    auto count = read<int32_t>(stream);

    for (int32_t i = 0; i < count; i++) {
        if (key_type_ == "EnumProperty") {
            auto key = read_length_string(stream);
        } else if (key_type_ == "IntProperty") {
            auto key = read<int32_t>(stream);
        } else if (key_type_ == "ObjectProperty") {
            ObjectReference key(stream);
        } else {
            throw std::runtime_error("Map key type \"" + key_type_ + "\" not implemented!");
        }

        if (value_type_ == "ByteProperty") {
            auto value = read<int8_t>(stream);
        } else if (value_type_ == "IntProperty") {
            auto value = read<int32_t>(stream);
        } else if (value_type_ == "StructProperty") {
            auto s = std::make_unique<PropertyStruct>("", stream);
        } else {
            throw std::runtime_error("Map value type \"" + value_type_ + "\" not implemented!");
        }
    }
}
