#include "IntProperty.h"

#include <utility>

#include "../../Utils/StreamUtils.h"

SatisfactorySaveGame::IntProperty::IntProperty(
    std::string property_name, std::string property_type, std::istream& stream)
    : Property(std::move(property_name), std::move(property_type), stream) {
    auto unk = read<int8_t>(stream);
    if (unk != 0) {
        throw std::runtime_error("IntProperty: Unexpected none zero byte!");
    }
    auto value = read<int32_t>(stream);
}
