#include "StrProperty.h"

#include <utility>

#include "../../Utils/StreamUtils.h"

SatisfactorySaveGame::StrProperty::StrProperty(
    std::string property_name, std::string property_type, std::istream& stream)
    : Property(std::move(property_name), std::move(property_type), stream) {
    read_assert_zero<int8_t>(stream);
    value_ = read_length_string(stream);
}
