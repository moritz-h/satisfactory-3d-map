#include "EnumProperty.h"

#include <utility>

#include "PropertyVisitor.h"
#include "Utils/StreamUtils.h"

Satisfactory3DMap::EnumProperty::EnumProperty(PropertyTag tag, std::istream& stream) : Property(std::move(tag)) {
    value_ = read_length_string(stream);
}

void Satisfactory3DMap::EnumProperty::serialize(std::ostream& stream) const {
    Property::serialize(stream);
    write_length_string(stream, value_);
}

void Satisfactory3DMap::EnumProperty::accept(Satisfactory3DMap::PropertyVisitor& v) {
    v.visit(*this);
}
