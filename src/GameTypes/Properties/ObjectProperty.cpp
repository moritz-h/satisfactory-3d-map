#include "ObjectProperty.h"

#include <utility>

#include "PropertyVisitor.h"
#include "Utils/StreamUtils.h"

Satisfactory3DMap::ObjectProperty::ObjectProperty(PropertyTag tag, std::istream& stream) : Property(std::move(tag)) {
    value_ = ObjectReference(stream);
}

void Satisfactory3DMap::ObjectProperty::serialize(std::ostream& stream) const {
    Property::serialize(stream);
    value_.serialize(stream);
}

void Satisfactory3DMap::ObjectProperty::accept(Satisfactory3DMap::PropertyVisitor& v) {
    v.visit(*this);
}
