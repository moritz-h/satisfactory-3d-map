#include "StructProperty.h"

#include <utility>

#include <glm/glm.hpp>

#include "../ObjectReference.h"
#include "PropertyVisitor.h"
#include "Utils/StreamUtils.h"

Satisfactory3DMap::StructProperty::StructProperty(std::string property_name, std::string property_type,
    std::istream& stream)
    : Property(std::move(property_name), std::move(property_type), stream) {
    struct_name_ = read_length_string(stream);
    guid_ = Guid(stream);
    read_assert_zero<int8_t>(stream);

    struct_ = Struct::parse(struct_name_, stream);
}

void Satisfactory3DMap::StructProperty::serialize(std::ostream& stream) const {
    Property::serialize(stream);
    write_length_string(stream, struct_name_);
    guid_.serialize(stream);
    write<int8_t>(stream, 0);
    struct_->serialize(stream);
}

void Satisfactory3DMap::StructProperty::accept(Satisfactory3DMap::PropertyVisitor& v) {
    v.visit(*this);
}
