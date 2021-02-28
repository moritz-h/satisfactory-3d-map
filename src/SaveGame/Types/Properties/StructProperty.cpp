#include "StructProperty.h"

#include <utility>

#include "../../Utils/StreamUtils.h"

SatisfactorySaveGame::StructProperty::StructProperty(
    std::string property_name, std::string property_type, std::istream& stream)
    : Property(std::move(property_name), std::move(property_type), stream) {
    auto struct_name = read_length_string(stream);
    read<int32_t>(stream);
    read<int32_t>(stream);
    read<int32_t>(stream);
    read<int32_t>(stream);
    read<int8_t>(stream);
    stream.ignore(size_); // TODO
}
