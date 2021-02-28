#include "TextProperty.h"

#include <utility>

#include "../../Utils/StreamUtils.h"

SatisfactorySaveGame::TextProperty::TextProperty(
    std::string property_name, std::string property_type, std::istream& stream)
    : Property(std::move(property_name), std::move(property_type), stream) {
    read_assert_zero<int8_t>(stream);
    stream.ignore(size_); // TODO
}
