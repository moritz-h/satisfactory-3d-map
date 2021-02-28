#include "BoolProperty.h"

#include <utility>

#include "../../Utils/StreamUtils.h"

SatisfactorySaveGame::BoolProperty::BoolProperty(
    std::string property_name, std::string property_type, std::istream& stream)
    : Property(std::move(property_name), std::move(property_type), stream) {
    auto value = read<int8_t>(stream);
    auto unk = read<int8_t>(stream);
    if (unk != 0) {
        throw std::runtime_error("BoolProperty: Unexpected none zero byte!");
    }
}
