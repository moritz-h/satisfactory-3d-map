#include "Int64Property.h"

#include <utility>

#include "PropertyVisitor.h"
#include "Utils/StreamUtils.h"

Satisfactory3DMap::Int64Property::Int64Property(PropertyTag tag, std::istream& stream) : Property(std::move(tag)) {
    value_ = read<int64_t>(stream);
}

void Satisfactory3DMap::Int64Property::serialize(std::ostream& stream) const {
    Property::serialize(stream);
    write(stream, value_);
}

void Satisfactory3DMap::Int64Property::accept(Satisfactory3DMap::PropertyVisitor& v) {
    v.visit(*this);
}
