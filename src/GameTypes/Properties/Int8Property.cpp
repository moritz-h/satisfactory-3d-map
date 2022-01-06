#include "Int8Property.h"

#include <utility>

#include "PropertyVisitor.h"
#include "Utils/StreamUtils.h"

Satisfactory3DMap::Int8Property::Int8Property(PropertyTag tag, std::istream& stream) : Property(std::move(tag)) {
    value_ = read<int8_t>(stream);
}

void Satisfactory3DMap::Int8Property::serialize(std::ostream& stream) const {
    Property::serialize(stream);
    write(stream, value_);
}

void Satisfactory3DMap::Int8Property::accept(Satisfactory3DMap::PropertyVisitor& v) {
    v.visit(*this);
}
