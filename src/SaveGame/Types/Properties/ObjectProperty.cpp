#include "ObjectProperty.h"

#include <utility>

#include "../../Utils/StreamUtils.h"
#include "../ObjectReference.h"

SatisfactorySaveGame::ObjectProperty::ObjectProperty(
    std::string property_name, std::string property_type, std::istream& stream)
    : Property(std::move(property_name), std::move(property_type), stream) {
    auto unk = read<int8_t>(stream);
    if (unk != 0) {
        throw std::runtime_error("ObjectProperty: Unexpected none zero byte!");
    }
    ObjectReference ref;
    ref.level_name = read_length_string(stream);
    ref.path_name = read_length_string(stream);
}
