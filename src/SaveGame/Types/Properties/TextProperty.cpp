#include "TextProperty.h"

#include <utility>

#include "PropertyVisitor.h"
#include "Utils/StreamUtils.h"

Satisfactory3DMap::TextProperty::TextProperty(std::string property_name, std::string property_type,
    std::istream& stream)
    : Property(std::move(property_name), std::move(property_type), stream) {
    read_assert_zero<int8_t>(stream);

    buf_ = read_vector<char>(stream, size_); // TODO
}

void Satisfactory3DMap::TextProperty::accept(Satisfactory3DMap::PropertyVisitor& v) {
    v.visit(*this);
}
