#include "ArrayProperty.h"

#include <utility>

#include "../Structs/Struct.h"
#include "SaveGame/Types/Guid.h"
#include "SaveGame/Types/ObjectReference.h"
#include "Utils/StreamUtils.h"

Satisfactory3DMap::ArrayProperty::ArrayProperty(
    std::string property_name, std::string property_type, std::istream& stream)
    : Property(std::move(property_name), std::move(property_type), stream) {
    array_type_ = read_length_string(stream);
    read_assert_zero<int8_t>(stream);

    auto count = read<int32_t>(stream);

    if (array_type_ == "ByteProperty") {
        read_vector<int8_t>(stream, count);
    } else if (array_type_ == "EnumProperty") {
        for (int32_t i = 0; i < count; i++) {
            read_length_string(stream);
        }
    } else if (array_type_ == "InterfaceProperty") {
        for (int32_t i = 0; i < count; i++) {
            ObjectReference ref(stream);
        }
    } else if (array_type_ == "IntProperty") {
        read_vector<int32_t>(stream, count);
    } else if (array_type_ == "ObjectProperty") {
        for (int32_t i = 0; i < count; i++) {
            ObjectReference ref(stream);
        }
    } else if (array_type_ == "StructProperty") {
        auto name = read_length_string(stream);
        auto type = read_length_string(stream);
        auto size = read<int32_t>(stream);
        auto index = read<int32_t>(stream);
        if (type != "StructProperty") {
            throw std::runtime_error("Invalid StructProperty array!");
        }

        auto structName = read_length_string(stream);
        Guid guid(stream);
        read_assert_zero<int8_t>(stream);

        for (int32_t i = 0; i < count; i++) {
            Struct::parse(structName, stream);
        }
    } else {
        throw std::runtime_error("Array type \"" + array_type_ + "\" not implemented!");
    }
}
