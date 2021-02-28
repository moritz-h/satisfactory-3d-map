#include "BoolProperty.h"

#include <utility>

#include "../../Utils/StreamUtils.h"

SatisfactorySaveGame::BoolProperty::BoolProperty(
    std::string property_name, std::string property_type, std::istream& stream)
    : Property(std::move(property_name), std::move(property_type), stream) {
    value_ = read<int8_t>(stream);
    read_assert_zero<int8_t>(stream);
}
