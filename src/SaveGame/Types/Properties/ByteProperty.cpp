#include "ByteProperty.h"

#include <utility>

#include "PropertyVisitor.h"
#include "Utils/StreamUtils.h"

Satisfactory3DMap::ByteProperty::ByteProperty(PropertyTag tag, std::istream& stream) : Property(std::move(tag)) {
    if (tag_.EnumName == "None") {
        value_ = static_cast<char>(read<int8_t>(stream));
    } else {
        value_ = read_length_string(stream);
    }
}

void Satisfactory3DMap::ByteProperty::serialize(std::ostream& stream) const {
    Property::serialize(stream);
    if (tag_.EnumName == "None") {
        write(stream, static_cast<int8_t>(value_[0]));
    } else {
        write_length_string(stream, value_);
    }
}

void Satisfactory3DMap::ByteProperty::accept(Satisfactory3DMap::PropertyVisitor& v) {
    v.visit(*this);
}
