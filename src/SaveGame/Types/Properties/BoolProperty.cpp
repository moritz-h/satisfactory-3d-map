#include "BoolProperty.h"

#include <utility>

#include "PropertyVisitor.h"
#include "Utils/StreamUtils.h"

Satisfactory3DMap::BoolProperty::BoolProperty(std::string property_name, std::string property_type,
    std::istream& stream)
    : Property(std::move(property_name), std::move(property_type), stream) {
    value_ = read<int8_t>(stream);
    read_assert_zero<int8_t>(stream);
}

void Satisfactory3DMap::BoolProperty::serialize(std::ostream& stream) const {
    Property::serialize(stream);
    write(stream, value_);
    write<int8_t>(stream, 0);
}

void Satisfactory3DMap::BoolProperty::accept(Satisfactory3DMap::PropertyVisitor& v) {
    v.visit(*this);
}
