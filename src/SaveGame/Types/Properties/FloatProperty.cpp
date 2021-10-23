#include "FloatProperty.h"

#include <utility>

#include "PropertyVisitor.h"
#include "Utils/StreamUtils.h"

Satisfactory3DMap::FloatProperty::FloatProperty(std::string property_name, std::string property_type,
    std::istream& stream)
    : Property(std::move(property_name), std::move(property_type), stream) {
    read_assert_zero<int8_t>(stream);
    value_ = read<float>(stream);
}

void Satisfactory3DMap::FloatProperty::accept(Satisfactory3DMap::PropertyVisitor& v) {
    v.visit(*this);
}
