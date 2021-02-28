#include "ArrayProperty.h"

#include <utility>

#include "../../Utils/StreamUtils.h"

SatisfactorySaveGame::ArrayProperty::ArrayProperty(
    std::string property_name, std::string property_type, std::istream& stream)
    : Property(std::move(property_name), std::move(property_type), stream) {
    auto array_type = read_length_string(stream);
    auto unk = read<int8_t>(stream);
    if (unk != 0) {
        throw std::runtime_error("ArrayProperty: Unexpected none zero byte!");
    }
    stream.ignore(size_); // TODO
}
