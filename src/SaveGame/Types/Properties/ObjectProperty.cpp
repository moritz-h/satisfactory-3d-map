#include "ObjectProperty.h"

#include <utility>

#include "PropertyVisitor.h"
#include "Utils/StreamUtils.h"

Satisfactory3DMap::ObjectProperty::ObjectProperty(std::string property_name, std::string property_type,
    std::istream& stream)
    : Property(std::move(property_name), std::move(property_type), stream) {
    read_assert_zero<int8_t>(stream);
    value_ = ObjectReference(stream);
}

void Satisfactory3DMap::ObjectProperty::serialize(std::ostream& stream) const {
    Property::serialize(stream);
    write<int8_t>(stream, 0);
    value_.serialize(stream);
}

void Satisfactory3DMap::ObjectProperty::accept(Satisfactory3DMap::PropertyVisitor& v) {
    v.visit(*this);
}
