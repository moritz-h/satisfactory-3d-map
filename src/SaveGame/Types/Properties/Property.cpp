#include "Property.h"

#include <utility>

#include "Utils/StreamUtils.h"

Satisfactory3DMap::Property::Property(std::string property_name, std::string property_type, std::istream& stream)
    : property_name_(std::move(property_name)), property_type_(std::move(property_type)) {
    size_ = read<int32_t>(stream);
    index_ = read<int32_t>(stream);
}
