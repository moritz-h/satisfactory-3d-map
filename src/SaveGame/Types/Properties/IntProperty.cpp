#include "IntProperty.h"

#include <utility>

#include "PropertyVisitor.h"
#include "Utils/StreamUtils.h"

Satisfactory3DMap::IntProperty::IntProperty(std::string property_name, std::string property_type, std::istream& stream)
    : Property(std::move(property_name), std::move(property_type), stream) {
    read_assert_zero<int8_t>(stream);
    value_ = read<int32_t>(stream);
}

void Satisfactory3DMap::IntProperty::accept(Satisfactory3DMap::PropertyVisitor& v) {
    v.visit(*this);
}
