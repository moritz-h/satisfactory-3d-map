#include "StructProperty.h"

#include <utility>

#include <glm/glm.hpp>

#include "../ObjectReference.h"
#include "PropertyVisitor.h"
#include "Utils/StreamUtils.h"

Satisfactory3DMap::StructProperty::StructProperty(PropertyTag tag, std::istream& stream) : Property(std::move(tag)) {
    struct_ = Struct::parse(tag_.StructName, stream);
}

void Satisfactory3DMap::StructProperty::serialize(std::ostream& stream) const {
    Property::serialize(stream);
    struct_->serialize(stream);
}

void Satisfactory3DMap::StructProperty::accept(Satisfactory3DMap::PropertyVisitor& v) {
    v.visit(*this);
}
