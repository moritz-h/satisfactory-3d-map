#include "ArrayProperty.h"

#include <utility>

#include "../Structs/Struct.h"
#include "GameTypes/Guid.h"
#include "GameTypes/ObjectReference.h"
#include "PropertyVisitor.h"
#include "Utils/StreamUtils.h"

Satisfactory3DMap::ArrayProperty::ArrayProperty(PropertyTag tag, std::istream& stream) : Property(std::move(tag)) {
    auto count = read<int32_t>(stream);
    array_ = Array::parse(tag_.InnerType, count, stream);
}

void Satisfactory3DMap::ArrayProperty::serialize(std::ostream& stream) const {
    Property::serialize(stream);
    array_->serialize(stream);
}

void Satisfactory3DMap::ArrayProperty::accept(Satisfactory3DMap::PropertyVisitor& v) {
    v.visit(*this);
}
