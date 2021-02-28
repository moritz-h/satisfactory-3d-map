#include "TextProperty.h"

#include <utility>

#include "../../Utils/StreamUtils.h"

SatisfactorySaveGame::TextProperty::TextProperty(
    std::string property_name, std::string property_type, std::istream& stream)
    : Property(std::move(property_name), std::move(property_type), stream) {
    auto unk = read<int8_t>(stream);
    if (unk != 0) {
        throw std::runtime_error("TextProperty: Unexpected none zero byte!");
    }
    stream.ignore(size_); // TODO
}
