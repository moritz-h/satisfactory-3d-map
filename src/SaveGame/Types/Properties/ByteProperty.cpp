#include "ByteProperty.h"

#include <utility>

#include "PropertyVisitor.h"
#include "Utils/StreamUtils.h"

Satisfactory3DMap::ByteProperty::ByteProperty(std::string property_name, std::string property_type,
    std::istream& stream)
    : Property(std::move(property_name), std::move(property_type), stream) {
    byte_type_ = read_length_string(stream);
    read_assert_zero<int8_t>(stream);

    if (byte_type_ == "None") {
        value_ = static_cast<char>(read<int8_t>(stream));
    } else {
        value_ = read_length_string(stream);
    }
}

void Satisfactory3DMap::ByteProperty::serialize(std::ostream& stream) const {
    Property::serialize(stream);
    write_length_string(stream, byte_type_);
    write<int8_t>(stream, 0);
    if (byte_type_ == "None") {
        write(stream, static_cast<int8_t>(value_[0]));
    } else {
        write_length_string(stream, value_);
    }
}

void Satisfactory3DMap::ByteProperty::accept(Satisfactory3DMap::PropertyVisitor& v) {
    v.visit(*this);
}
