#include "EnumProperty.h"

#include <utility>

#include "../../Utils/StreamUtils.h"

SatisfactorySaveGame::EnumProperty::EnumProperty(
    std::string property_name, std::string property_type, std::istream& stream)
    : Property(std::move(property_name), std::move(property_type), stream) {
    auto enum_type = read_length_string(stream);
    auto unk = read<int8_t>(stream);
    if (unk != 0) {
        throw std::runtime_error("EnumProperty: Unexpected none zero byte!");
    }
    auto value = read_length_string(stream);
}
