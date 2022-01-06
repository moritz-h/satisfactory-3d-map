#include "StrProperty.h"

#include <utility>

#include "PropertyVisitor.h"
#include "Utils/StreamUtils.h"

Satisfactory3DMap::StrProperty::StrProperty(PropertyTag tag, std::istream& stream) : Property(std::move(tag)) {
    value_ = read_length_string(stream);
}

void Satisfactory3DMap::StrProperty::serialize(std::ostream& stream) const {
    Property::serialize(stream);
    write_length_string(stream, value_);
}

void Satisfactory3DMap::StrProperty::accept(Satisfactory3DMap::PropertyVisitor& v) {
    v.visit(*this);
}
