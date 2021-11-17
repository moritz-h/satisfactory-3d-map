#include "IntProperty.h"

#include <utility>

#include "PropertyVisitor.h"
#include "Utils/StreamUtils.h"

Satisfactory3DMap::IntProperty::IntProperty(PropertyTag tag, std::istream& stream) : Property(std::move(tag)) {
    value_ = read<int32_t>(stream);
}

void Satisfactory3DMap::IntProperty::serialize(std::ostream& stream) const {
    Property::serialize(stream);
    write(stream, value_);
}

void Satisfactory3DMap::IntProperty::accept(Satisfactory3DMap::PropertyVisitor& v) {
    v.visit(*this);
}
