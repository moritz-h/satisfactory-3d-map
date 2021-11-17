#include "FloatProperty.h"

#include <utility>

#include "PropertyVisitor.h"
#include "Utils/StreamUtils.h"

Satisfactory3DMap::FloatProperty::FloatProperty(PropertyTag tag, std::istream& stream) : Property(std::move(tag)) {
    value_ = read<float>(stream);
}

void Satisfactory3DMap::FloatProperty::serialize(std::ostream& stream) const {
    Property::serialize(stream);
    write(stream, value_);
}

void Satisfactory3DMap::FloatProperty::accept(Satisfactory3DMap::PropertyVisitor& v) {
    v.visit(*this);
}
