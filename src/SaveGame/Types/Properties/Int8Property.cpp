#include "Int8Property.h"

#include <utility>

#include "PropertyVisitor.h"
#include "Utils/StreamUtils.h"

Satisfactory3DMap::Int8Property::Int8Property(std::string property_name, std::string property_type,
    std::istream& stream)
    : Property(std::move(property_name), std::move(property_type), stream) {
    read_assert_zero<int8_t>(stream);
    value_ = read<int8_t>(stream);
}

void Satisfactory3DMap::Int8Property::serialize(std::ostream& stream) const {
    Property::serialize(stream);
    write<int8_t>(stream, 0);
    write(stream, value_);
}

void Satisfactory3DMap::Int8Property::accept(Satisfactory3DMap::PropertyVisitor& v) {
    v.visit(*this);
}
