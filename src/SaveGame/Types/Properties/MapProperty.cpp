#include "MapProperty.h"

#include <utility>

#include "../../Utils/StreamUtils.h"

SatisfactorySaveGame::MapProperty::MapProperty(
    std::string property_name, std::string property_type, std::istream& stream)
    : Property(std::move(property_name), std::move(property_type), stream) {
    auto key_type = read_length_string(stream);
    auto value_type = read_length_string(stream);
    auto unk = read<int8_t>(stream);
    if (unk != 0) {
        throw std::runtime_error("MapProperty: Unexpected none zero byte!");
    }
    stream.ignore(size_); // TODO
}
