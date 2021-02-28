#include "Int64Property.h"

#include <utility>

#include "../../Utils/StreamUtils.h"

SatisfactorySaveGame::Int64Property::Int64Property(
    std::string property_name, std::string property_type, std::istream& stream)
    : Property(std::move(property_name), std::move(property_type), stream) {
    auto unk = read<int8_t>(stream);
    if (unk != 0) {
        throw std::runtime_error("Int64Property: Unexpected none zero byte!");
    }
    auto value = read<int64_t>(stream);
}
